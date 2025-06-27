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
#include "trust_anchor_prime256v1_cose_key_private.h"
#include "delegated_authority_cose_key_private.h"
#include "trust_anchor_hmac256_cose_key_secret.h"

#define MAX_FILE_BUFFER_SIZE            (8 * 1024 * 1024)

int main(int argc,
         char *argv[])
{
    int exit_code = EXIT_SUCCESS;
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

    mechanisms[0].key.cose_algorithm_id = T_COSE_ALGORITHM_ESP256;
    result = suit_set_suit_key_from_cose_key(trust_anchor_prime256v1_cose_key_private, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[0].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[0].use = false;

    mechanisms[1].key.cose_algorithm_id = T_COSE_ALGORITHM_ES256;
    result = suit_set_suit_key_from_cose_key(trust_anchor_prime256v1_cose_key_private, &mechanisms[0].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[1].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[1].use = false;

    mechanisms[2].key.cose_algorithm_id = T_COSE_ALGORITHM_ESP256;
    result = suit_set_suit_key_from_cose_key(delegated_authority_es256_cose_key_private, &mechanisms[1].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[2].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[2].use = false;

    mechanisms[3].key.cose_algorithm_id = T_COSE_ALGORITHM_ES256;
    result = suit_set_suit_key_from_cose_key(delegated_authority_es256_cose_key_private, &mechanisms[1].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create public key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[3].cose_tag = CBOR_TAG_COSE_SIGN1;
    mechanisms[3].use = false;

    mechanisms[4].key.cose_algorithm_id = T_COSE_ALGORITHM_HMAC256;
    result = suit_set_suit_key_from_cose_key(trust_anchor_hmac256_cose_key_secret, &mechanisms[2].key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create secret key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanisms[4].cose_tag = CBOR_TAG_COSE_MAC0;
    mechanisms[4].use = false;

    // Read manifest file.
    printf("main : Read Manifest file.\n");
    uint8_t *manifest_buf = malloc(SUIT_MAX_DATA_SIZE);
    if (manifest_buf == NULL) {
        printf("main : Failed to allocate memory.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    size_t manifest_len = read_from_file(manifest_file, manifest_buf, SUIT_MAX_DATA_SIZE);
    if (manifest_len == 0) {
        printf("main : Failed to read Manifest file.\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    suit_print_hex(manifest_buf, manifest_len);
    printf("\n\n");

    // Decode manifest file.
    printf("main : Decode Manifest file.\n");
    suit_decode_mode_t mode = SUIT_DECODE_MODE_STRICT;
#ifdef SKIP_ERROR
    mode = SUIT_DECODE_MODE_SKIP_ANY_ERROR;
#endif
    suit_envelope_t *envelope = calloc(1, sizeof(suit_envelope_t));
    suit_buf_t buf = {.ptr = manifest_buf, .len = manifest_len};
    result = suit_decode_envelope(mode, &buf, envelope, mechanisms);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to parse Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        exit_code = EXIT_FAILURE;
        goto end;
    }

    // Print manifest.
    printf("\nmain : Print Manifest.\n");
    result = suit_print_envelope(mode, envelope, indent, tabstop);
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
    result = suit_encode_envelope(mode, envelope, mechanisms, &ret_pos, &encode_len);
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

    if (manifest_len != encode_len) {
        printf("main : The manifest length is changed %ld => %ld\n", manifest_len, encode_len);
        printf("#### ORIGINAL ####\n");
        suit_print_hex_in_max(manifest_buf, manifest_len, manifest_len);
        printf("\n#### ENCODED ####\n");
        suit_print_hex_in_max(ret_pos, encode_len, encode_len);
        printf("\n\n");
        exit_code = EXIT_FAILURE;
        goto end;
    }
    else if (memcmp(manifest_buf, ret_pos, manifest_len) != 0) {
        size_t signature_pos = (envelope->tagged ? 2 : 0) + 55;
        if (memcmp(&manifest_buf[0], &ret_pos[0], signature_pos) != 0 ||
            memcmp(&manifest_buf[signature_pos + 64], &ret_pos[signature_pos + 64], manifest_len - (signature_pos + 64)) != 0) {
            printf("main : encoded binary is differ from original\n");
            printf("#### ORIGINAL ####\n");
            suit_print_hex_in_max(manifest_buf, manifest_len, manifest_len);
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
    if (manifest_buf != NULL) {
        free(manifest_buf);
        manifest_buf = NULL;
    }
    if (envelope != NULL) {
        free(envelope);
        envelope = NULL;
    }
    suit_free_key(&mechanisms[0].key);
    return exit_code;
}
