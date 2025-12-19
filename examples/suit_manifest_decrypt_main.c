/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include "csuit/suit_manifest_print.h"
#include "suit_examples_common.h"
#include "trust_anchor_a128_cose_key_secret.h"
#include "device_esdh_cose_key_private.h"

#define MAX_FILE_BUFFER_SIZE            2048

int main(int argc, char *argv[]) {
    // check arguments.
    if (argc < 4) {
        printf("%s <raw image> <encrypted image> <encryption info>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *src_file = argv[1];
    char *enc_file = argv[2];
    char *encryption_info_file = argv[3];
    suit_err_t result = SUIT_SUCCESS;

    // Load raw image
    UsefulBuf_MAKE_STACK_UB(raw_buf, MAX_FILE_BUFFER_SIZE);
    raw_buf.len = read_from_file(src_file, raw_buf.ptr, raw_buf.len);

    // Prepare
    UsefulBuf_MAKE_STACK_UB(encrypted_payload_buf, MAX_FILE_BUFFER_SIZE);
    encrypted_payload_buf.len = read_from_file(enc_file, encrypted_payload_buf.ptr, encrypted_payload_buf.len);
    UsefulBuf_MAKE_STACK_UB(encryption_info_buf, MAX_FILE_BUFFER_SIZE);
    encryption_info_buf.len = read_from_file(encryption_info_file, encryption_info_buf.ptr, encryption_info_buf.len);

    // Decrypt
    // Load receiver's private key
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM] = {0};

    result = suit_set_suit_key_from_cose_key(trust_anchor_a128_cose_key_secret, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create symmetric key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[0].cose_tag = CBOR_TAG_COSE_ENCRYPT;
    mechanisms[0].use = true;

    result = suit_set_suit_key_from_cose_key(device_ecdh_es_a128kw_cose_key_private, &mechanisms[1].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create receiver key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[1].cose_tag = CBOR_TAG_COSE_ENCRYPT;
    mechanisms[1].use = true;

    UsefulBufC plaintext_payload;
    UsefulBuf_MAKE_STACK_UB(plaintext_payload_buf, MAX_FILE_BUFFER_SIZE);

    int i = 0;
    for (i = 0; i < 2; i++) {
        result = suit_decrypt_cose_encrypt(UsefulBuf_Const(encrypted_payload_buf), UsefulBuf_Const(encryption_info_buf), plaintext_payload_buf, &mechanisms[i], &plaintext_payload);
        if (result != SUIT_SUCCESS) {
            continue;
        }
        if (raw_buf.len != plaintext_payload.len) {
            //printf("\nmain : Assertion failed. length %lu != %lu\n", raw_buf.len, plaintext_payload.len);
            continue;
        }
        if (memcmp(raw_buf.ptr, plaintext_payload.ptr, raw_buf.len) != 0) {
            /*
            suit_print_hex(raw_buf.ptr, raw_buf.len);
            printf("\n");
            suit_print_hex(plaintext_payload.ptr, plaintext_payload.len);
            printf("\n");
            */
            continue;
        }
        printf("main : Succeed to decrypt.\n");
        break;
    }
    if (i == 2) {
        if (result != SUIT_SUCCESS) {
            printf("main : Failed to decrypt. %s(%d)\n", suit_err_to_str(result), result);
        }
        else {
            printf("main : Assertion failed. raw_buf != plaintext_buf\n");
        }
        return EXIT_FAILURE;
    }

    suit_free_key(&mechanisms[0].key);
    suit_free_key(&mechanisms[1].key);
    return EXIT_SUCCESS;
}
