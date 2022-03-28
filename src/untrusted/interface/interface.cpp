#define MAX_PATH FILENAME_MAX

#include "untrusted/interface/interface.h"
#include "untrusted/interface/stdafx.h"
#include <algorithm>
#include <fstream>

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

uint8_t INPUT_BUFFER[INPUT_BUFFER_SIZE];
uint8_t OUTPUT_BUFFER[INPUT_BUFFER_SIZE];
Queue *inQueue;
bool status = false;

int launch_enclave(sgx_launch_token_t *token, int *updated) {
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;

    ret = sgx_create_enclave(
            ENCLAVE_FILENAME, TRUE, token, updated, &global_eid, NULL);
    if (ret != SGX_SUCCESS)
        return ret;
    else
        return 0;
}

int init() {
    sgx_launch_token_t token = {0};
    int updated = 0;
    int resp = launch_enclave(&token, &updated);

    return resp;
}

// void *enclaveThread(void *) {
void enclaveThread() {
    int resp = 0;
    enclaveProcess(global_eid, &resp, inQueue);
}

int initMultithreading() {
    sgx_launch_token_t token = {0};
    int updated = 0;
    status = true;
    int ans = launch_enclave(&token, &updated);

    inQueue = new Queue();

    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
        INPUT_BUFFER[i] = OUTPUT_BUFFER[i] = 0;

    std::thread th = std::thread(&enclaveThread);

    th.detach();

    return ans;
}

int generateKey() {
    if (!status) {
        int resp = initMultithreading();
        if (resp != SGX_SUCCESS)
            return resp;
    }

    int resp, resp_enclave, flength;
    uint8_t *sealed_key_b = new uint8_t[SEALED_KEY_LENGTH];

    std::fstream data_file;
    data_file.open(DATA_FILENAME,
                   std::fstream::in | std::fstream::out | std::fstream::binary);
    if (data_file) {
        data_file.seekg(0, data_file.end);
        flength = data_file.tellg();

        if (flength == SEALED_KEY_LENGTH)
            return 0;

        else {
            resp = generateKeyEnclave(
                    global_eid, &resp_enclave, sealed_key_b, SEALED_KEY_LENGTH);
            if (resp != SGX_SUCCESS)
                return resp;
            data_file.write((char *) sealed_key_b, SEALED_KEY_LENGTH);
        }
    }
    else
        return NO_KEYS_STORAGE;

    data_file.close();
    delete[] sealed_key_b;

    return (int) flength / SEALED_KEY_LENGTH;
}

int loadKey(int item) {
    if (!status) {
        int resp = initMultithreading();
        if (resp != SGX_SUCCESS)
            return resp;
    }
    int resp, resp_enclave;
    uint8_t sealed_key_b[SEALED_KEY_LENGTH];

    std::fstream data_file;
    data_file.open(DATA_FILENAME, std::fstream::in | std::fstream::binary);
    if (data_file) {
        data_file.seekg(0, data_file.end);
        int flength = data_file.tellg();
        if (flength < item * SEALED_KEY_LENGTH + SEALED_KEY_LENGTH)
            return NO_KEY_ID;

        data_file.seekg(item * SEALED_KEY_LENGTH);
        data_file.read((char *) sealed_key_b, SEALED_KEY_LENGTH);
        resp = loadKeyEnclave(
                global_eid, &resp_enclave, sealed_key_b, SEALED_KEY_LENGTH);
        if (resp != SGX_SUCCESS)
            return resp;
    }
    else
        return NO_KEYS_STORAGE;

    data_file.close();
    return 0;
}

//int enc_text_compress_n_encrypt(char *pSrc, size_t src_len, char *pDst) {
//    if (!status)
//    {
//        int resp = initMultithreading();
//        resp = loadKey(0);
//        if (resp != SGX_SUCCESS)
//            return resp;
//    }
//    int compBytes = 0;
//    int resp = 0;
//    int comp_len = LZ4_compressBound(src_len) + 2 * sizeof(int);
//    char *comp_buffer = (char *) malloc(comp_len);
//    resp = compressBufferEnclave(global_eid, &compBytes, pSrc, comp_buffer + 2 * sizeof(int), src_len,
//                                 comp_len - 2 * sizeof(int));
//    if (resp != SGX_SUCCESS) {
//        return resp;
//    }
//    memcpy(comp_buffer, &src_len, sizeof(int));
//    memcpy(comp_buffer + sizeof(int), &compBytes, sizeof(int));
//    compBytes += 2 * sizeof(int);
//    size_t enc_len = compBytes + SGX_AESGCM_IV_SIZE + SGX_AESGCM_MAC_SIZE;
//    size_t enc_b64_len = ((int) (4 * (double) (enc_len) / 3) + 3) & ~3;
//    if (compBytes != 2 * sizeof(int)) {
//        resp = enc_text_encrypt(comp_buffer, compBytes, pDst, enc_b64_len);
//    }
//    free(comp_buffer);
//    return resp + (enc_b64_len << 4);
//}

//int enc_text_decrypt_n_decompress(char *pSrc, size_t src_len, char *pDst) {
//    if (!status)
//    {
//        int resp = initMultithreading();
//        resp = loadKey(0);
//        if (resp != SGX_SUCCESS)
//            return resp;
//    }
//    int resp;
//    char *decry_buffer = (char *) malloc(2 * src_len * sizeof(char));
//    resp = enc_text_decrypt(pSrc, src_len, decry_buffer, 2 * src_len);
//    int dec_len = (resp >> 4);
//    int raw_bytes, comp_bytes;
//    memcpy(&raw_bytes, decry_buffer, sizeof(int));
//    memcpy(&comp_bytes, decry_buffer + sizeof(int), sizeof(int));
//
//    resp = decompressBufferEnclave(global_eid, &dec_len, decry_buffer + 2 * sizeof(int), pDst, comp_bytes,
//                                   raw_bytes);
//    if (dec_len < 0 || dec_len != (int) raw_bytes) {
//        printf("unable to decompress it properly!");
//    }
//    return resp + (dec_len << 4);
//}

int enc_text_compress_n_encrypt(char *pSrc, size_t src_len, char *pDst, size_t dst_len = 0) {
    int compBytes, resp;

    compBytes = enc_compress(pSrc, src_len, pDst, dst_len);
    if (compBytes < 0) {
        return SGX_ERROR_UNEXPECTED;
    }
    // comp_buffer format: (size_t) src_len, (size_t) compressed_len, (char*) compressed data
    char *comp_buffer = (char *) malloc(sizeof(size_t) * 2 + compBytes);
    memcpy(comp_buffer, &src_len, sizeof(size_t));
    size_t compBytes_sizeT = (size_t) compBytes;
    memcpy(comp_buffer + sizeof(size_t), &compBytes_sizeT, sizeof(size_t));
    memcpy(comp_buffer + 2 * sizeof(size_t), pDst, compBytes);
    compBytes += 2 * sizeof(size_t);
    size_t enc_len = compBytes + SGX_AESGCM_IV_SIZE + SGX_AESGCM_MAC_SIZE;
    size_t enc_b64_len = ((int) (4 * (double) (enc_len) / 3) + 3) & ~3;
    resp = enc_text_encrypt(comp_buffer, compBytes, pDst, enc_b64_len);
    free(comp_buffer);

    return resp + (enc_b64_len << 4);
}

int enc_text_decrypt_n_decompress(char *pSrc, size_t src_len, char *pDst, size_t dst_len = 0) {
    if (!status) {
        int resp = initMultithreading();
        resp = loadKey(0);
        if (resp != SGX_SUCCESS)
            return resp;
    }
    int resp;
    char *decry_buffer = (char *) malloc(2 * src_len * sizeof(char));
    if (dst_len == 0) {
        dst_len = 2 * src_len;
    }
    resp = enc_text_decrypt(pSrc, src_len, decry_buffer, dst_len);


    int dec_len = (resp >> 4); // length of decrypted data (length of compress data + 2 * sizeof(int) )
    resp -= (dec_len << 4);
    size_t raw_bytes, comp_bytes;
    memcpy(&raw_bytes, decry_buffer, sizeof(size_t)); // length of raw data
    memcpy(&comp_bytes, decry_buffer + sizeof(size_t), sizeof(size_t)); // length of compressed data
    assert(dec_len == (comp_bytes + 2 * sizeof(size_t)));

    dec_len = enc_decompress(decry_buffer + 2 * sizeof(size_t), comp_bytes, pDst, dst_len);

    if (dec_len < 0 || dec_len != (int) raw_bytes) {
        printf("unable to decompress it properly!");
    }

    return resp + (dec_len << 4);
}