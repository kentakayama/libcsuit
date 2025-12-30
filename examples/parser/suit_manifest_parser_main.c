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
#include "trust_anchor_esp256_cose_key_private.h"
#include "delegated_authority_cose_key_private.h"
#include "trust_anchor_hmac256_cose_key_secret.h"

#define MAX_FILE_BUFFER_SIZE            (8 * 1024 * 1024)
#define ENCODE_BUFFER_SIZE 4096

int main(int argc,
         char *argv[])
{
    int exit_code = EXIT_SUCCESS;
    suit_envelope_t *envelope = NULL;

    // check arguments.
    if (argc < 1) {
        printf("%s <manifest file path> [tabstop 2] [indent 4] [output]\n", argv[0]);
        return EXIT_FAILURE;
    }
    uint16_t tabstop = 2;
    if (argc >= 3) {
        tabstop = atoi(argv[2]);
    }
    uint16_t indent = 4;
    if (argc >= 4) {
        indent = atoi(argv[3]);
    }
    char *output = NULL;
    if (argc >= 5) {
        output = argv[4];
    }
    suit_err_t result = 0;
    char *manifest_file = argv[1];
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM] = {0};

    result = suit_set_suit_key_from_cose_key(trust_anchor_esp256_cose_key_private, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[0].key.cose_algorithm_id = T_COSE_ALGORITHM_ESP256;
    mechanisms[0].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[0].use = false;

    result = suit_set_suit_key_from_cose_key(trust_anchor_esp256_cose_key_private, &mechanisms[1].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[1].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[1].use = false;

    result = suit_set_suit_key_from_cose_key(delegated_authority_esp256_cose_key_private, &mechanisms[2].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[2].key.cose_algorithm_id = T_COSE_ALGORITHM_ESP256;
    mechanisms[2].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[2].use = false;

    result = suit_set_suit_key_from_cose_key(delegated_authority_esp256_cose_key_private, &mechanisms[3].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[3].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[3].use = false;

    result = suit_set_suit_key_from_cose_key(trust_anchor_hmac256_cose_key_secret, &mechanisms[4].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create secret key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[4].cose_tag = CBOR_TAG_COSE_MAC0;
    mechanisms[4].use = false;

    // Read manifest file.
    printf("main : Read Manifest file.\n");
    UsefulBuf manifest;
    manifest.ptr = malloc(MAX_FILE_BUFFER_SIZE);
    if (manifest.ptr == NULL) {
        printf("main : Failed to allocate memory.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    manifest.len = read_from_file(manifest_file, manifest.ptr, MAX_FILE_BUFFER_SIZE);
    if (manifest.len == 0) {
        printf("main : Failed to read Manifest file.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    suit_print_hex(manifest.ptr, manifest.len);
    printf("\n\n");

    // Decode manifest file.
    printf("main : Decode Manifest file.\n");
    envelope = calloc(1, sizeof(suit_envelope_t));
    result = suit_decode_envelope(UsefulBuf_Const(manifest), envelope, mechanisms);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to parse Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        exit_code = EXIT_FAILURE;
        goto end;
    }

    // Print manifest.
    printf("\nmain : Print Manifest.\n");
    result = suit_print_envelope(envelope, indent, tabstop);
    printf("\n\n");
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to print Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        exit_code = EXIT_FAILURE;
        goto end;
    }

    // Initialize manifest encoder.
    suit_encoder_context_t *encoder_context = malloc(sizeof(suit_encoder_context_t) + ENCODE_BUFFER_SIZE);
    result = suit_encode_init(encoder_context, ENCODE_BUFFER_SIZE);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to initialize encoder context. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < SUIT_MAX_KEY_NUM; i++) {
        if (mechanisms[i].use) {
            result = suit_encode_add_sender_key(encoder_context, mechanisms[i].cose_tag, T_COSE_ALGORITHM_RESERVED, &mechanisms[i].key);
            if (result != SUIT_SUCCESS) {
                printf("main : Failed to assign a signing key. %s(%d)\n", suit_err_to_str(result), result);
                return EXIT_FAILURE;
            }
            printf("main : Assigned %s with %s key.", suit_cose_alg_to_str(mechanisms[i].key.cose_algorithm_id), suit_cbor_tag_to_str(mechanisms[i].cose_tag));
        }
    }

    printf("\nmain : Encode Manifest.\n");
    UsefulBufC encoded_manifest;
    result = suit_encode_envelope(encoder_context, envelope, &encoded_manifest);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to encode. %s(%d)\n", suit_err_to_str(result), result);
        if (mechanisms[1].use) {
            printf("main : Due to delegated public key. Skip encoding test.\n");
            // success
            goto end;
        }
        exit_code = EXIT_FAILURE;
        goto end;
    }
    printf("main : Total buffer memory usage was %ld/%d bytes\n",
        (uint8_t *)encoded_manifest.ptr + encoded_manifest.len - ((uint8_t *)encoder_context + sizeof(suit_encoder_context_t)),
        ENCODE_BUFFER_SIZE);

    if (output != NULL) {
        write_to_file(output, encoded_manifest.ptr, encoded_manifest.len);
    }

    if (manifest.len != encoded_manifest.len) {
        printf("main : The manifest length is changed %ld => %ld\n", manifest.len, encoded_manifest.len);
        printf("#### ORIGINAL ####\n");
        suit_print_hex(manifest.ptr, manifest.len);
        printf("\n#### ENCODED ####\n");
        suit_print_hex(encoded_manifest.ptr, encoded_manifest.len);
        printf("\n\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    else if (memcmp(manifest.ptr, encoded_manifest.ptr, manifest.len) != 0) {
        size_t signature_pos = (envelope->tagged ? 2 : 0) + 55;
        if (memcmp((uint8_t *)manifest.ptr + 0, (uint8_t *)encoded_manifest.ptr + 0, signature_pos) != 0 ||
            memcmp((uint8_t *)manifest.ptr + (signature_pos + 64), (uint8_t *)encoded_manifest.ptr + (signature_pos + 64), manifest.len - (signature_pos + 64)) != 0) {
            printf("main : encoded binary is differ from the original one\n");
            printf("#### ORIGINAL ####\n");
            suit_print_hex(manifest.ptr, manifest.len);
            printf("\n#### ENCODED ####\n");
            suit_print_hex(encoded_manifest.ptr, encoded_manifest.len);
            printf("\n\n");
            exit_code = EXIT_FAILURE;
            goto end;
        }
        else {
            printf("main : Whole binaries but COSE_Sign1 signature match.\n\n");
        }
    }
    else {
        printf("main : Whole binaries match.\n\n");
    }

end:
    if (manifest.ptr != NULL) {
        free(manifest.ptr);
        manifest.ptr = NULL;
    }
    if (envelope != NULL) {
        free(envelope);
        envelope = NULL;
    }
    for (size_t i = 0; i < SUIT_MAX_KEY_NUM; i++) {
        suit_free_key(&mechanisms[i].key);
    }
    return exit_code;
}
