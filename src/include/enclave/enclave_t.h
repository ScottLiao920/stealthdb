#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

int generateKeyEnclave(uint8_t* sealed_key, size_t sealedkey_len);
int loadKeyEnclave(uint8_t* key, size_t len);
int enclaveProcess(void* inQueue);
int compressBufferEnclave(char* pSrc, char* pDst, size_t src_len, size_t dst_len);
int decompressBufferEnclave(char* pSrc, char* pDst, size_t src_len, size_t dst_len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
