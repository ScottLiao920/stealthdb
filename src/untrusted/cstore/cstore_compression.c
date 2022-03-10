/*-------------------------------------------------------------------------
 *
 * cstore_compression.c
 *
 * This file contains compression/decompression functions definitions
 * used in cstore_fdw.
 *
 * Copyright (c) 2016, Citus Data, Inc.
 *
 * $Id$
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "cstore/cstore_fdw.h"
#include "lz4.h" // for lz4 compression

#if PG_VERSION_NUM >= 90500
#include "common/pg_lzcompress.h"
#else

#include "utils/pg_lzcompress.h"

#endif


#if PG_VERSION_NUM >= 90500
/*
 *	The information at the start of the compressed data. This decription is taken
 *	from pg_lzcompress in pre-9.5 version of PostgreSQL.
 */
typedef struct CStoreCompressHeader {
    int32 vl_len_;        /* varlena header (do not touch directly!) */
    int32 rawsize;
} CStoreCompressHeader;
/*
 * Utilities for manipulation of header information for compressed data
 */

#define CSTORE_COMPRESS_HDRSZ        ((int32) sizeof(CStoreCompressHeader))
#define CSTORE_COMPRESS_RAWSIZE(ptr) (((CStoreCompressHeader *) (ptr))->rawsize)
#define CSTORE_COMPRESS_RAWDATA(ptr) (((char *) (ptr)) + CSTORE_COMPRESS_HDRSZ)
#define CSTORE_COMPRESS_SET_RAWSIZE(ptr, len) (((CStoreCompressHeader *) (ptr))->rawsize = (len))

#else

#define CSTORE_COMPRESS_HDRSZ        (0)
#define CSTORE_COMPRESS_RAWSIZE(ptr) (PGLZ_RAW_SIZE((PGLZ_Header *) buffer->data))
#define CSTORE_COMPRESS_RAWDATA(ptr) (((PGLZ_Header *) (ptr)))
#define CSTORE_COMPRESS_SET_RAWSIZE(ptr, len) (((PGLZ_Header *) (ptr))->rawsize = (len))

typedef struct LZ4CompressHeader {
    size_t src_len; // original string length
    size_t comp_len; // length of compressed string
} LZ4CompressHeader;
#define CSTORE_COMPRESS_HDRSZ_LZ4       (sizeof(LZ4CompressHeader))
#define CSTORE_COMPRESS_RAWSIZE_LZ4(ptr) (((LZ4CompressHeader *) (ptr))->src_len)
#define CSTORE_COMPRESS_RAWDATA_LZ4(ptr) (((char *) (ptr)) + CSTORE_COMPRESS_HDRSZ_LZ4)
#define CSTORE_COMPRESS_SET_RAWSIZE_LZ4(ptr, len) (((LZ4CompressHeader *) (ptr))->src_len = (len))

#endif


/*
 * CompressBuffer compresses the given buffer with the given compression type
 * outputBuffer enlarged to contain compressed data. The function returns true
 * if compression is done, returns false if compression is not done.
 * outputBuffer is valid only if the function returns true.
 */
bool
CompressBuffer(StringInfo inputBuffer, StringInfo outputBuffer,
               CompressionType compressionType) {
    uint64 maximumLength = PGLZ_MAX_OUTPUT(inputBuffer->len) + CSTORE_COMPRESS_HDRSZ;
    bool compressionResult = false;
#if PG_VERSION_NUM >= 90500
    int32 compressedByteCount = 0;
#endif

    if (compressionType != COMPRESSION_PG_LZ && compressionType != COMPRESSION_LZ4) {
        return false;
    }


    if (compressionType == COMPRESSION_PG_LZ) {
        resetStringInfo(outputBuffer);
        enlargeStringInfo(outputBuffer, maximumLength);
#if PG_VERSION_NUM >= 90500
        compressedByteCount = pglz_compress((const char *) inputBuffer->data,
                                            inputBuffer->len,
                                            CSTORE_COMPRESS_RAWDATA(outputBuffer->data),
                                            PGLZ_strategy_always);
        if (compressedByteCount >= 0) {
            CSTORE_COMPRESS_SET_RAWSIZE(outputBuffer->data, inputBuffer->len);
            SET_VARSIZE_COMPRESSED(outputBuffer->data,
                                   compressedByteCount + CSTORE_COMPRESS_HDRSZ);
            compressionResult = true;
        }
#else
        compressionResult = pglz_compress(inputBuffer->data, inputBuffer->len,
                                          CSTORE_COMPRESS_RAWDATA(outputBuffer->data),
                                          PGLZ_strategy_always);
#endif
    } else {
        int compressed_bytes = 0; // for lz4 compression
        size_t min_dst_len = LZ4_compressBound(inputBuffer->len) + CSTORE_COMPRESS_HDRSZ_LZ4;
        resetStringInfo(outputBuffer);
        enlargeStringInfo(outputBuffer, min_dst_len);
        Assert(outputBuffer->maxlen >= min_src_len);
        compressed_bytes = LZ4_compress_default(inputBuffer->data, CSTORE_COMPRESS_RAWDATA_LZ4(outputBuffer->data),
                                                inputBuffer->len,
                                                outputBuffer->maxlen);
        if (compressed_bytes <= 0) {
            ereport(ERROR, (errmsg("\"LZ4 Compression Failed!"),
                    errdetail("Expected at least %u bytes, but received %u bytes",
                              min_dst_len, compressed_bytes)));
        } else {
            compressionResult = true;
            printf("We successfully compressed some data! Ratio: %.2f\n",
                   (float) compressed_bytes / inputBuffer->len);
            CSTORE_COMPRESS_SET_RAWSIZE_LZ4(outputBuffer->data, inputBuffer->len);
            ((LZ4CompressHeader *) outputBuffer->data)->comp_len = compressed_bytes;
            outputBuffer->len = compressed_bytes + CSTORE_COMPRESS_HDRSZ_LZ4;
//            SET_VARSIZE_COMPRESSED(outputBuffer->data,
//                                   compressed_bytes + CSTORE_COMPRESS_HDRSZ_LZ4); //not used since we not using postgres vl length
        }
    }
    if (compressionResult && compressionType != COMPRESSION_LZ4) {
        outputBuffer->len = VARSIZE(outputBuffer->data);
    }

    return compressionResult;
}


/*
 * DecompressBuffer decompresses the given buffer with the given compression
 * type. This function returns the buffer as-is when no compression is applied.
 */
StringInfo
DecompressBuffer(StringInfo buffer, CompressionType compressionType) {
    StringInfo decompressedBuffer = NULL;

    Assert(compressionType == COMPRESSION_NONE || compressionType == COMPRESSION_PG_LZ ||
           compressionType == COMPRESSION_LZ4);

    if (compressionType == COMPRESSION_NONE) {
        /* in case of no compression, return buffer */
        decompressedBuffer = buffer;
    } else {
        if (compressionType == COMPRESSION_PG_LZ) {
            uint32 compressedDataSize = VARSIZE(buffer->data) - CSTORE_COMPRESS_HDRSZ;
            uint32 decompressedDataSize = CSTORE_COMPRESS_RAWSIZE(buffer->data);
            char *decompressedData = NULL;
#if PG_VERSION_NUM >= 90500
            int32 decompressedByteCount = 0;
#endif

            if (compressedDataSize + CSTORE_COMPRESS_HDRSZ != buffer->len) {
                ereport(ERROR, (errmsg("cannot decompress the buffer"),
                        errdetail("Expected %u bytes, but received %u bytes",
                                  compressedDataSize, buffer->len)));
            }

            decompressedData = palloc0(decompressedDataSize);

#if PG_VERSION_NUM >= 90500

#if PG_VERSION_NUM >= 120000
            decompressedByteCount = pglz_decompress(CSTORE_COMPRESS_RAWDATA(buffer->data),
                                                    compressedDataSize, decompressedData,
                                                    decompressedDataSize, true);
#else
            decompressedByteCount = pglz_decompress(CSTORE_COMPRESS_RAWDATA(buffer->data),
                                                    compressedDataSize, decompressedData,
                                                    decompressedDataSize);
#endif

            if (decompressedByteCount < 0)
            {
                ereport(ERROR, (errmsg("cannot decompress the buffer"),
                                errdetail("compressed data is corrupted")));
            }
#else
            pglz_decompress((PGLZ_Header *) buffer->data, decompressedData);
#endif

            decompressedBuffer = palloc0(sizeof(StringInfoData));
            decompressedBuffer->data = decompressedData;
            decompressedBuffer->len = decompressedDataSize;
            decompressedBuffer->maxlen = decompressedDataSize;
        } else {
            size_t compressedDataSize = ((LZ4CompressHeader *) (buffer->data))->comp_len;
            int decompressedDataSize_expected = (int) CSTORE_COMPRESS_RAWSIZE_LZ4(buffer->data);
            int decompressedDataSize_real = 0;
            char *decompressedData = NULL;
            if (compressedDataSize + CSTORE_COMPRESS_HDRSZ_LZ4 != buffer->len) {
                ereport(ERROR, (errmsg("cannot decompress the buffer"),
                        errdetail("Expected %u bytes, but received %u bytes",
                                  buffer->len, compressedDataSize + CSTORE_COMPRESS_HDRSZ_LZ4)));
            }
            decompressedData = palloc0(decompressedDataSize_expected);
            decompressedDataSize_real = LZ4_decompress_safe(CSTORE_COMPRESS_RAWDATA_LZ4(buffer->data), decompressedData,
                                                            compressedDataSize,
                                                            decompressedDataSize_expected);
            if (decompressedDataSize_real < 0) {
                ereport(ERROR, (errmsg("lz4 cannot decompress the buffer, malformed source string"),
                        errdetail("Expected %u bytes, but received %u bytes",
                                  compressedDataSize, buffer->len)));
            }
            decompressedBuffer = palloc0(sizeof(StringInfoData));
            decompressedBuffer->data = decompressedData;
            decompressedBuffer->len = decompressedDataSize_real;
            decompressedBuffer->maxlen = decompressedDataSize_real;
        }
    }
    return decompressedBuffer;
}
