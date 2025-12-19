/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include "csuit/suit_common.h"
#include "csuit/suit_manifest_decode.h"
#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_manifest_print.h"
#include "csuit/suit_cose.h"
#include "suit_examples_common.h"
#if defined(SUIT_MANIFEST_SIGNER_TRUST_ANCHOR)
#include "trust_anchor_esp256_cose_key_private.h"
UsefulBufC private_key = trust_anchor_esp256_cose_key_private;
UsefulBufC public_key = NULLUsefulBufC;
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_SIGN1;
#elif defined(SUIT_MANIFEST_SIGNER_TAM)
#include "tam_esp256_cose_key_private.h"
UsefulBufC private_key = tam_esp256_cose_key_private;
#include "trust_anchor_esp256_cose_key_public.h"
UsefulBufC public_key = trust_anchor_esp256_cose_key_public;
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_SIGN1;
#elif defined(SUIT_MANIFEST_SIGNER_MAC)
#include "trust_anchor_hmac256_cose_key_secret.h"
UsefulBufC private_key = trust_anchor_hmac256_cose_key_secret;
UsefulBufC public_key = NULLUsefulBufC;
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_MAC0;
#else
#error Signing key is not specified
UsefulBufC private_key = NULLUsefulBufC;
UsefulBufC public_key = NULLUsefulBufC;
cbor_tag_key_t cose_tag = 0;
#endif

#define MAX_FILE_BUFFER_SIZE            (8 * 1024 * 1024)

int main(int argc,
         char *argv[])
{
    // check arguments.
    if (argc < 3) {
        printf("%s <manifest file path> <signed manifest file path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    suit_err_t result = 0;
    int ret = EXIT_FAILURE;
    uint8_t indent = 4;
    uint8_t tabstop = 2;
    char *input_file = argv[1];
    char *output_file = argv[2];
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM] = {0};
    UsefulBuf manifest;
    uint8_t *encode_buf = NULL;

    result = suit_set_suit_key_from_cose_key(private_key, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create signing key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[0].cose_tag = cose_tag;
    mechanisms[0].use = true;

    if (!UsefulBuf_IsNULLOrEmptyC(public_key)) {
        result = suit_set_suit_key_from_cose_key(public_key, &mechanisms[1].key);
        if (result != SUIT_SUCCESS) {
            printf("main : Failed to create verification key of trust anchor. %s(%d)\n", suit_err_to_str(result), result);
            return EXIT_FAILURE;
        }
        mechanisms[1].cose_tag = cose_tag;
        mechanisms[1].use = true;
    }

    // Read manifest file.
    printf("main : Read Manifest file.\n");
    manifest.ptr = malloc(SUIT_MAX_DATA_SIZE);
    if (manifest.ptr == NULL) {
        printf("main : Failed to allocate memory.\n");
        goto out;
    }
    manifest.len = read_from_file(input_file, manifest.ptr, SUIT_MAX_DATA_SIZE);
    if (manifest.len == 0) {
        printf("main : Failed to read Manifest file.\n");
        goto out;
    }
    suit_print_hex(manifest.ptr, manifest.len);
    printf("\n\n");

    // Decode manifest file.
    printf("main : Decode Manifest file.\n");
    suit_envelope_t envelope = {0};
    result = suit_decode_envelope(UsefulBuf_Const(manifest), &envelope, mechanisms);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to parse Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    // Print manifest.
    printf("\nmain : Print Decoded Manifest.\n");
    result = suit_print_envelope(&envelope, indent, tabstop);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to print Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    // Encode manifest.
    encode_buf = malloc(SUIT_MAX_DATA_SIZE);
    if (encode_buf == NULL) {
        printf("main : Failed to allocate memory.\n");
        goto out;
    }
    size_t encode_len = SUIT_MAX_DATA_SIZE;
    uint8_t *ret_pos = encode_buf;
    printf("\nmain : Encode Manifest.\n");
    result = suit_encode_envelope(&envelope, mechanisms, &ret_pos, &encode_len);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to encode. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    printf("main : Total buffer memory usage was %ld/%d bytes\n", ret_pos + encode_len - encode_buf, SUIT_MAX_DATA_SIZE);

    write_to_file(output_file, ret_pos, encode_len);
    ret = EXIT_SUCCESS;
out:
    if (manifest.ptr != NULL) {
        free(manifest.ptr);
        manifest.ptr = NULL;
    }
    if (encode_buf != NULL) {
        free(encode_buf);
    }
    suit_free_key(&mechanisms[0].key);
    printf("\n");
    return ret;
}
