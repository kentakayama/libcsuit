/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include "csuit/suit_common.h"
#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_manifest_print.h"
#include "csuit/suit_cose.h"
#include "suit_examples_common.h"
#include "trust_anchor_esp256_cose_key_private.h"

#define ENCODE_BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    // check arguments.
    if (argc < 2) {
        printf("%s <output manifest file path>", argv[0]);
        return EXIT_FAILURE;
    }
    char *manifest_file = argv[1];

    suit_key_t signing_key;
    UsefulBufC private_key = trust_anchor_esp256_cose_key_private;
    suit_err_t result = suit_set_suit_key_from_cose_key(private_key, &signing_key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create ESP256 key pair. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    // Initialize manifest encoder.
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
    result = suit_encode_add_sender_key(encoder_context, CBOR_TAG_COSE_SIGN1, T_COSE_ALGORITHM_ESP256, &signing_key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to assign a signing key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    // Generate manifest
    suit_envelope_t envelope = {0};
    suit_manifest_t *manifest = &envelope.manifest;
    manifest->version = 1;
    manifest->sequence_number = 2;

    uint8_t component_id[] = {0x00};
    suit_common_t *common = &manifest->common;
    common->components_len = 1;
    common->components[0].component.len = 1;
    common->components[0].component.identifier[0] = (UsefulBufC){.ptr = component_id, .len = sizeof(component_id)};

    uint8_t vendor_id[] = {0xFA, 0x6B, 0x4A, 0x53, 0xD5, 0xAD, 0x5F, 0xDF, 0xBE, 0x9D, 0xE6, 0x63, 0xE4, 0xD4, 0x1F, 0xFE};
    uint8_t class_id[] = {0x14, 0x92, 0xAF, 0x14, 0x25, 0x69, 0x5E, 0x48, 0xBF, 0x42, 0x9B, 0x2D, 0x51, 0xF2, 0xAB, 0x45};
    suit_command_sequence_t *shared_seq = &common->shared_seq;
    shared_seq->len = 3;

    shared_seq->commands[0].label = SUIT_DIRECTIVE_OVERRIDE_PARAMETERS;
    suit_parameters_list_t *params_list = &shared_seq->commands[0].value.params_list;
    params_list->len = 4;

    params_list->params[0].label = SUIT_CONDITION_VENDOR_IDENTIFIER;
    params_list->params[0].value.string.ptr = vendor_id;
    params_list->params[0].value.string.len = sizeof(vendor_id);

    params_list->params[1].label = SUIT_CONDITION_CLASS_IDENTIFIER;
    params_list->params[1].value.string.ptr = class_id;
    params_list->params[1].value.string.len = sizeof(class_id);

    uint8_t image_digest[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    params_list->params[2].label = SUIT_PARAMETER_IMAGE_DIGEST,
    params_list->params[2].value.digest.algorithm_id = SUIT_ALGORITHM_ID_SHA256;
    params_list->params[2].value.digest.bytes.ptr = image_digest;
    params_list->params[2].value.digest.bytes.len = sizeof(image_digest);

    params_list->params[3].label = SUIT_PARAMETER_IMAGE_SIZE;
    params_list->params[3].value.uint64 = 34768;

    shared_seq->commands[1].label = SUIT_CONDITION_VENDOR_IDENTIFIER;
    shared_seq->commands[1].value.uint64 = 15;

    shared_seq->commands[2].label = SUIT_CONDITION_CLASS_IDENTIFIER;
    shared_seq->commands[2].value.uint64 = 15;

    manifest->sev_man_mem.install_status = SUIT_SEVERABLE_IN_MANIFEST;
    suit_command_sequence_t *install = &manifest->sev_man_mem.install;
    install->len = 3;
    install->commands[0].label = SUIT_DIRECTIVE_SET_PARAMETERS;
    params_list = &install->commands[0].value.params_list;
    params_list->len = 1;

    uint8_t uri[] = "http://example.com/file.bin";
    params_list->params[0].label = SUIT_PARAMETER_URI;
    params_list->params[0].value.string.ptr = uri;
    params_list->params[0].value.string.len = sizeof(uri) - 1;

    install->commands[1].label = SUIT_DIRECTIVE_FETCH;
    install->commands[1].value.uint64 = 15;

    install->commands[2].label = SUIT_CONDITION_IMAGE_MATCH;
    install->commands[2].value.uint64 = 15;

    suit_command_sequence_t *validate = &manifest->unsev_mem.validate;
    validate->len = 1;
    validate->commands[0].label = SUIT_CONDITION_IMAGE_MATCH;
    validate->commands[0].value.uint64 = 15;

    // Print manifest.
    printf("\nmain : Print Manifest.\n");
    result = suit_print_envelope(&envelope, 4, 4);
    if (result != SUIT_SUCCESS) {
        printf("main : Can't print Manifest file.\n");
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

    size_t w_len = write_to_file(manifest_file, encoded_manifest.ptr, encoded_manifest.len);
    if (w_len != encoded_manifest.len) {
        printf("main : Failed to write to %s\n", manifest_file);
    }
    printf("main : Wrote SUIT manifest to %s.\n", manifest_file);

    suit_free_key(&signing_key);
    return EXIT_SUCCESS;
}
