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
    manifest.ptr = malloc(SUIT_MAX_DATA_SIZE);
    if (manifest.ptr == NULL) {
        printf("main : Failed to allocate memory.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    manifest.len = read_from_file(manifest_file, manifest.ptr, SUIT_MAX_DATA_SIZE);
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
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to print Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        exit_code = EXIT_FAILURE;
        goto end;
    }

    // Encode manifest.
    uint8_t *encode_buf = malloc(SUIT_MAX_DATA_SIZE);
    if (encode_buf == NULL) {
        printf("main : Failed to allocate memory.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    size_t encode_len = SUIT_MAX_DATA_SIZE;
    uint8_t *ret_pos = encode_buf;
    printf("\nmain : Encode Manifest.\n");
    result = suit_encode_envelope(envelope, mechanisms, &ret_pos, &encode_len);
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
    printf("main : Total buffer memory usage was %ld/%d bytes\n", ret_pos + encode_len - encode_buf, SUIT_MAX_DATA_SIZE);

    if (output != NULL) {
        write_to_file(output, ret_pos, encode_len);
    }

    if (manifest.len != encode_len) {
        printf("main : The manifest length is changed %ld => %ld\n", manifest.len, encode_len);
        printf("#### ORIGINAL ####\n");
        suit_print_hex_in_max(manifest.ptr, manifest.len, manifest.len);
        printf("\n#### ENCODED ####\n");
        suit_print_hex_in_max(ret_pos, encode_len, encode_len);
        printf("\n\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    else if (memcmp(manifest.ptr, ret_pos, manifest.len) != 0) {
        size_t signature_pos = (envelope->tagged ? 2 : 0) + 55;
        if (memcmp((uint8_t *)manifest.ptr + 0, &ret_pos[0], signature_pos) != 0 ||
            memcmp((uint8_t *)manifest.ptr + (signature_pos + 64), &ret_pos[signature_pos + 64], manifest.len - (signature_pos + 64)) != 0) {
            printf("main : encoded binary is differ from original\n");
            printf("#### ORIGINAL ####\n");
            suit_print_hex_in_max(manifest.ptr, manifest.len, manifest.len);
            printf("\n#### ENCODED ####\n");
            suit_print_hex_in_max(ret_pos, encode_len, encode_len);
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
    suit_free_key(&mechanisms[0].key);
    return exit_code;
}
