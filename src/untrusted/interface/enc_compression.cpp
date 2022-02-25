//
// Created by scott on 2/25/22.
//

#include "untrusted/interface/interface.h"
#include "untrusted/interface/stdafx.h"
#include <unistd.h>


extern sgx_enclave_id_t global_eid;
extern Queue *inQueue;
extern bool status;


int enc_compress(char *pSrc, size_t src_len, char *pDst) {
    if (!status) {
        int resp = initMultithreading();
        resp = loadKey(0);
        if (resp != SGX_SUCCESS)
            return resp;
    }
    int compBytes = 0;
    int resp = 0;
    size_t comp_len = LZ4_compressBound(src_len);
    char *comp_buffer = NULL;
    request *comp_req = new request;
    comp_req->ocall_index = CMD_COMPRESS;
    comp_req->is_done = -1;
    memcpy(comp_req->buffer, &src_len, sizeof(size_t));
    memcpy(comp_req->buffer + sizeof(size_t), &comp_len, sizeof(size_t));
    memcpy(comp_req->buffer + 2 * sizeof(size_t), pSrc, src_len);
    inQueue->enqueue(comp_req);

    while (true) {
        if (comp_req->is_done == -1) {
            __asm__("pause");
        } else {
            compBytes = comp_req->resp;
            comp_buffer = (char *) (malloc(compBytes * sizeof(char) + 2 * sizeof(int)));
            memcpy(comp_buffer + 2 * sizeof(int), comp_req->buffer + 2 * sizeof(size_t) + src_len,
                   compBytes * sizeof(char));
            spin_unlock(&comp_req->is_done);
            break;
        }
    }
    return resp;
}

int enc_decompress(char *pSrc, size_t src_len, char *pDst) {
    if (!status) {
        int resp = initMultithreading();
        resp = loadKey(0);
        if (resp != SGX_SUCCESS)
            return resp;
    }
    int dec_len;
    // buffer format: (int) decrypted length, (int) expected no. of raw bytes, decrypted data
    request *req = new request;
    req->ocall_index = CMD_DECOMPRESS;
    req->is_done = -1;
    memcpy(req->buffer, &src_len, sizeof(size_t));
    memcpy(req->buffer + sizeof(size_t), &src_len, sizeof(size_t));
    memcpy(req->buffer + 2 * sizeof(size_t), pSrc, src_len);
    inQueue->enqueue(req);

    while (true) {
        if (req->is_done == -1) {
            __asm__("pause");
        } else {
            dec_len = req->resp;
            memcpy(pDst, req->buffer + 2 * sizeof(size_t) + dec_len,
                   dec_len * sizeof(char));
            spin_unlock(&req->is_done);
            break;
        }
    }
    return dec_len;
}
