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

#define MAX_FILE_BUFFER_SIZE            (8 * 1024 * 1024)
#define ENCODE_BUFFER_SIZE 4096

#if defined(SUIT_MANIFEST_SIGNER_TAM)
#include "tam_esp256_cose_key_private.h"
#include "trust_anchor_esp256_cose_key_public.h"
#define PRIVATE_KEY tam_esp256_cose_key_private
#define PUBLIC_KEY trust_anchor_esp256_cose_key_public
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_SIGN1;
#elif defined(SUIT_MANIFEST_SIGNER_MAC)
#include "trust_anchor_hmac256_cose_key_secret.h"
#define PRIVATE_KEY trust_anchor_hmac256_cose_key_secret
#define PUBLIC_KEY NULLUsefulBufC
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_MAC0;
#else
#include "trust_anchor_esp256_cose_key_private.h"
#define PRIVATE_KEY trust_anchor_esp256_cose_key_private
#define PUBLIC_KEY NULLUsefulBufC
cbor_tag_key_t cose_tag = CBOR_TAG_COSE_SIGN1;
#endif

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

    result = suit_set_suit_key_from_cose_key(PRIVATE_KEY, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create signing key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[0].cose_tag = cose_tag;
    mechanisms[0].use = true;

    if (!UsefulBuf_IsNULLOrEmptyC(PUBLIC_KEY)) {
        result = suit_set_suit_key_from_cose_key(PUBLIC_KEY, &mechanisms[1].key);
        if (result != SUIT_SUCCESS) {
            printf("main : Failed to create verification key of trust anchor. %s(%d)\n", suit_err_to_str(result), result);
            return EXIT_FAILURE;
        }
        mechanisms[1].cose_tag = cose_tag;
        mechanisms[1].use = true;
    }

    // Read manifest file.
    printf("main : Read Manifest file.\n");
    manifest.ptr = malloc(MAX_FILE_BUFFER_SIZE);
    if (manifest.ptr == NULL) {
        printf("main : Failed to allocate memory.\n");
        goto out;
    }
    manifest.len = read_from_file(input_file, manifest.ptr, MAX_FILE_BUFFER_SIZE);
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
    suit_encoder_context_t *encoder_context = malloc(sizeof(suit_encoder_context_t) + ENCODE_BUFFER_SIZE);
    if (encoder_context == NULL) {
        printf("main : Failed to allocate encoder context. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    result = suit_encode_init(encoder_context, ENCODE_BUFFER_SIZE);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to initialize encoder context. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    result = suit_encode_add_sender_key(encoder_context, mechanisms[0].cose_tag, T_COSE_ALGORITHM_RESERVED, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to assign a signing key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    printf("\nmain : Encode Manifest.\n");
    UsefulBufC encoded_manifest;
    result = suit_encode_envelope(encoder_context, &envelope, &encoded_manifest);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to encode. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    printf("main : Total buffer memory usage was %ld/%d bytes\n",
        (uint8_t *)encoded_manifest.ptr + encoded_manifest.len - ((uint8_t *)encoder_context + sizeof(suit_encoder_context_t)),
        ENCODE_BUFFER_SIZE);

    write_to_file(output_file, encoded_manifest.ptr, encoded_manifest.len);
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
