/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: untrusted/cstore/cstore.proto */

#ifndef PROTOBUF_C_untrusted_2fcstore_2fcstore_2eproto__INCLUDED
#define PROTOBUF_C_untrusted_2fcstore_2fcstore_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Protobuf__ColumnBlockSkipNode Protobuf__ColumnBlockSkipNode;
typedef struct _Protobuf__ColumnBlockSkipList Protobuf__ColumnBlockSkipList;
typedef struct _Protobuf__StripeFooter Protobuf__StripeFooter;
typedef struct _Protobuf__StripeMetadata Protobuf__StripeMetadata;
typedef struct _Protobuf__TableFooter Protobuf__TableFooter;
typedef struct _Protobuf__PostScript Protobuf__PostScript;


/* --- enums --- */

typedef enum _Protobuf__CompressionType {
    /*
     * Values should match with the corresponding struct in cstore_fdw.h
     */
    PROTOBUF__COMPRESSION_TYPE__NONE = 0,
    PROTOBUF__COMPRESSION_TYPE__PG_LZ = 1 PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(PROTOBUF__COMPRESSION_TYPE)
} Protobuf__CompressionType;

/* --- messages --- */

struct _Protobuf__ColumnBlockSkipNode {
    ProtobufCMessage base;
    protobuf_c_boolean has_rowcount;
    uint64_t rowcount;
    protobuf_c_boolean has_minimumvalue;
    ProtobufCBinaryData minimumvalue;
    protobuf_c_boolean has_maximumvalue;
    ProtobufCBinaryData maximumvalue;
    protobuf_c_boolean has_valueblockoffset;
    uint64_t valueblockoffset;
    protobuf_c_boolean has_valuelength;
    uint64_t valuelength;
    protobuf_c_boolean has_valuecompressiontype;
    Protobuf__CompressionType valuecompressiontype;
    protobuf_c_boolean has_existsblockoffset;
    uint64_t existsblockoffset;
    protobuf_c_boolean has_existslength;
    uint64_t existslength;
};
#define PROTOBUF__COLUMN_BLOCK_SKIP_NODE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__column_block_skip_node__descriptor) \
    , 0,0, 0,{0,NULL}, 0,{0,NULL}, 0,0, 0,0, 0,0, 0,0, 0,0 }


struct _Protobuf__ColumnBlockSkipList {
    ProtobufCMessage base;
    size_t n_blockskipnodearray;
    Protobuf__ColumnBlockSkipNode **blockskipnodearray;
};
#define PROTOBUF__COLUMN_BLOCK_SKIP_LIST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__column_block_skip_list__descriptor) \
    , 0,NULL }


struct _Protobuf__StripeFooter {
    ProtobufCMessage base;
    size_t n_skiplistsizearray;
    uint64_t *skiplistsizearray;
    size_t n_existssizearray;
    uint64_t *existssizearray;
    size_t n_valuesizearray;
    uint64_t *valuesizearray;
};
#define PROTOBUF__STRIPE_FOOTER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__stripe_footer__descriptor) \
    , 0,NULL, 0,NULL, 0,NULL }


struct _Protobuf__StripeMetadata {
    ProtobufCMessage base;
    protobuf_c_boolean has_fileoffset;
    uint64_t fileoffset;
    protobuf_c_boolean has_skiplistlength;
    uint64_t skiplistlength;
    protobuf_c_boolean has_datalength;
    uint64_t datalength;
    protobuf_c_boolean has_footerlength;
    uint64_t footerlength;
};
#define PROTOBUF__STRIPE_METADATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__stripe_metadata__descriptor) \
    , 0,0, 0,0, 0,0, 0,0 }


struct _Protobuf__TableFooter {
    ProtobufCMessage base;
    size_t n_stripemetadataarray;
    Protobuf__StripeMetadata **stripemetadataarray;
    protobuf_c_boolean has_blockrowcount;
    uint32_t blockrowcount;
};
#define PROTOBUF__TABLE_FOOTER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__table_footer__descriptor) \
    , 0,NULL, 0,0 }


struct _Protobuf__PostScript {
    ProtobufCMessage base;
    protobuf_c_boolean has_tablefooterlength;
    uint64_t tablefooterlength;
    protobuf_c_boolean has_versionmajor;
    uint64_t versionmajor;
    protobuf_c_boolean has_versionminor;
    uint64_t versionminor;
    /*
     * Leave this last in the record
     */
    char *magicnumber;
};
#define PROTOBUF__POST_SCRIPT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf__post_script__descriptor) \
    , 0,0, 0,0, 0,0, NULL }


/* Protobuf__ColumnBlockSkipNode methods */
void protobuf__column_block_skip_node__init
        (Protobuf__ColumnBlockSkipNode *message);

size_t protobuf__column_block_skip_node__get_packed_size
        (const Protobuf__ColumnBlockSkipNode *message);

size_t protobuf__column_block_skip_node__pack
        (const Protobuf__ColumnBlockSkipNode *message,
         uint8_t *out);

size_t protobuf__column_block_skip_node__pack_to_buffer
        (const Protobuf__ColumnBlockSkipNode *message,
         ProtobufCBuffer *buffer);

Protobuf__ColumnBlockSkipNode *
protobuf__column_block_skip_node__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__column_block_skip_node__free_unpacked
        (Protobuf__ColumnBlockSkipNode *message,
         ProtobufCAllocator *allocator);

/* Protobuf__ColumnBlockSkipList methods */
void protobuf__column_block_skip_list__init
        (Protobuf__ColumnBlockSkipList *message);

size_t protobuf__column_block_skip_list__get_packed_size
        (const Protobuf__ColumnBlockSkipList *message);

size_t protobuf__column_block_skip_list__pack
        (const Protobuf__ColumnBlockSkipList *message,
         uint8_t *out);

size_t protobuf__column_block_skip_list__pack_to_buffer
        (const Protobuf__ColumnBlockSkipList *message,
         ProtobufCBuffer *buffer);

Protobuf__ColumnBlockSkipList *
protobuf__column_block_skip_list__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__column_block_skip_list__free_unpacked
        (Protobuf__ColumnBlockSkipList *message,
         ProtobufCAllocator *allocator);

/* Protobuf__StripeFooter methods */
void protobuf__stripe_footer__init
        (Protobuf__StripeFooter *message);

size_t protobuf__stripe_footer__get_packed_size
        (const Protobuf__StripeFooter *message);

size_t protobuf__stripe_footer__pack
        (const Protobuf__StripeFooter *message,
         uint8_t *out);

size_t protobuf__stripe_footer__pack_to_buffer
        (const Protobuf__StripeFooter *message,
         ProtobufCBuffer *buffer);

Protobuf__StripeFooter *
protobuf__stripe_footer__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__stripe_footer__free_unpacked
        (Protobuf__StripeFooter *message,
         ProtobufCAllocator *allocator);

/* Protobuf__StripeMetadata methods */
void protobuf__stripe_metadata__init
        (Protobuf__StripeMetadata *message);

size_t protobuf__stripe_metadata__get_packed_size
        (const Protobuf__StripeMetadata *message);

size_t protobuf__stripe_metadata__pack
        (const Protobuf__StripeMetadata *message,
         uint8_t *out);

size_t protobuf__stripe_metadata__pack_to_buffer
        (const Protobuf__StripeMetadata *message,
         ProtobufCBuffer *buffer);

Protobuf__StripeMetadata *
protobuf__stripe_metadata__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__stripe_metadata__free_unpacked
        (Protobuf__StripeMetadata *message,
         ProtobufCAllocator *allocator);

/* Protobuf__TableFooter methods */
void protobuf__table_footer__init
        (Protobuf__TableFooter *message);

size_t protobuf__table_footer__get_packed_size
        (const Protobuf__TableFooter *message);

size_t protobuf__table_footer__pack
        (const Protobuf__TableFooter *message,
         uint8_t *out);

size_t protobuf__table_footer__pack_to_buffer
        (const Protobuf__TableFooter *message,
         ProtobufCBuffer *buffer);

Protobuf__TableFooter *
protobuf__table_footer__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__table_footer__free_unpacked
        (Protobuf__TableFooter *message,
         ProtobufCAllocator *allocator);

/* Protobuf__PostScript methods */
void protobuf__post_script__init
        (Protobuf__PostScript *message);

size_t protobuf__post_script__get_packed_size
        (const Protobuf__PostScript *message);

size_t protobuf__post_script__pack
        (const Protobuf__PostScript *message,
         uint8_t *out);

size_t protobuf__post_script__pack_to_buffer
        (const Protobuf__PostScript *message,
         ProtobufCBuffer *buffer);

Protobuf__PostScript *
protobuf__post_script__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void protobuf__post_script__free_unpacked
        (Protobuf__PostScript *message,
         ProtobufCAllocator *allocator);

/* --- per-message closures --- */

typedef void (*Protobuf__ColumnBlockSkipNode_Closure)
        (const Protobuf__ColumnBlockSkipNode *message,
         void *closure_data);

typedef void (*Protobuf__ColumnBlockSkipList_Closure)
        (const Protobuf__ColumnBlockSkipList *message,
         void *closure_data);

typedef void (*Protobuf__StripeFooter_Closure)
        (const Protobuf__StripeFooter *message,
         void *closure_data);

typedef void (*Protobuf__StripeMetadata_Closure)
        (const Protobuf__StripeMetadata *message,
         void *closure_data);

typedef void (*Protobuf__TableFooter_Closure)
        (const Protobuf__TableFooter *message,
         void *closure_data);

typedef void (*Protobuf__PostScript_Closure)
        (const Protobuf__PostScript *message,
         void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor protobuf__compression_type__descriptor;
extern const ProtobufCMessageDescriptor protobuf__column_block_skip_node__descriptor;
extern const ProtobufCMessageDescriptor protobuf__column_block_skip_list__descriptor;
extern const ProtobufCMessageDescriptor protobuf__stripe_footer__descriptor;
extern const ProtobufCMessageDescriptor protobuf__stripe_metadata__descriptor;
extern const ProtobufCMessageDescriptor protobuf__table_footer__descriptor;
extern const ProtobufCMessageDescriptor protobuf__post_script__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_untrusted_2fcstore_2fcstore_2eproto__INCLUDED */
