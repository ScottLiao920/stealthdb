//
// Created by scott on 27/3/22.
//
#include "untrusted/extensions/stdafx.h"

extern bool debugMode;

/*
 * The function compress a string in enclave.
 * @input: string
 * @return: compressed string
 */
PG_FUNCTION_INFO_V1(pg_enc_compress);

Datum
pg_enc_compress(PG_FUNCTION_ARGS) {
    char *pSrc = PG_GETARG_CSTRING(0);
    int enc_len = 0;
    int src_len = strlen(pSrc);
    int dst_len = 2 * src_len;
    char *pDst = (char *) palloc0(dst_len * sizeof(char));

    enc_len = enc_compress(pSrc, src_len, pDst, dst_len);
    if (enc_len < 0) {
        sgxErrorHandler(SGX_ERROR_UNEXPECTED);
    }
    char *str = (char *) palloc0(sizeof(char) * enc_len);
    memcpy(str, pDst, enc_len);
    str[enc_len] = '\0';
    pfree(pDst);
    PG_RETURN_CSTRING(str);
}


/*
 * The function compress a string in enclave.
 * @input: string
 * @return: compressed string
 */
PG_FUNCTION_INFO_V1(pg_enc_decompress);

Datum
pg_enc_decompress(PG_FUNCTION_ARGS) {
    char *pSrc = PG_GETARG_CSTRING(0);
    int dec_len = 0;
    int src_len = strlen(pSrc);
    char *pDst = (char *) palloc0(2 * src_len * sizeof(char));

    dec_len = enc_decompress(pSrc, src_len, pDst, 2 * src_len);
    if (dec_len < 0) {
        sgxErrorHandler(SGX_ERROR_UNEXPECTED);
    }
    char *str = (char *) palloc0(sizeof(char) * dec_len);
    memcpy(str, pDst, dec_len);
    str[dec_len] = '\0';
    pfree(pDst);
    PG_RETURN_CSTRING(str);
}

/*
 * The function compress a string in enclave.
 * @input: string
 * @return: compressed string
 */
PG_FUNCTION_INFO_V1(pg_enc_int_sum_bulk);

Datum
pg_enc_int_sum_bulk(PG_FUNCTION_ARGS) {
    char *pSrc = PG_GETARG_CSTRING(0);
    int resp;
    int src_len;
    memcpy(&src_len, pSrc, sizeof(int));
    char *pDst = (char *) palloc0(ENC_INT32_LENGTH_B64);

    resp = enc_int_sum_bulk(pSrc + sizeof(int), src_len, pDst);
    sgxErrorHandler(resp);
    PG_RETURN_CSTRING(pDst);
}