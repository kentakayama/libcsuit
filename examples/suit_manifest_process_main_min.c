/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process_main_min.c

    \brief  Minimum sample to use libcsuit processing
 */

#define MAX_DATA_SIZE 128

#include "csuit/suit_manifest_process.h"
#include "csuit/suit_cose.h"
#include "csuit/suit_digest.h"
#include "suit_examples_common.h"
#include "trust_anchor_prime256v1_cose_key_public.h"

uint8_t manifest_buf[] = {
    0xd8, 0x6b, 0xa2, 0x02, 0x58, 0x73, 0x82, 0x58, 0x24, 0x82, 0x2f, 0x58, 0x20, 0x66, 0x58, 0xea, 0x56, 0x02, 0x62, 0x69, 0x6d, 0xd1, 0xf1, 0x3b, 0x78, 0x22, 0x39, 0xa0, 0x64, 0xda, 
    0x7c, 0x6c, 0x5c, 0xba, 0xf5, 0x2f, 0xde, 0xd4, 0x28, 0xa6, 0xfc, 0x83, 0xc7, 0xe5, 0xaf, 0x58, 0x4a, 0xd2, 0x84, 0x43, 0xa1, 0x01, 0x26, 0xa0, 0xf6, 0x58, 0x40, 0x68, 0x11, 0x3f, 
    0x1e, 0x76, 0x73, 0x13, 0x30, 0xfe, 0xa1, 0x61, 0x36, 0xce, 0xad, 0x2b, 0xe4, 0xfa, 0x8d, 0x4b, 0xb3, 0xe3, 0x27, 0xe3, 0xf9, 0x23, 0x2e, 0xb4, 0xf1, 0x3b, 0x5d, 0x11, 0x8f, 0x2e, 
    0xa6, 0xe3, 0x1d, 0x9a, 0x09, 0xa2, 0x40, 0xf6, 0xf5, 0x77, 0x07, 0x76, 0x91, 0x6e, 0x15, 0xa4, 0xeb, 0xca, 0x1b, 0x62, 0x62, 0xcf, 0x6e, 0x40, 0xa4, 0x43, 0x53, 0xbd, 0x81, 0x61, 
    0xd9, 0x03, 0x58, 0x71, 0xa5, 0x01, 0x01, 0x02, 0x00, 0x03, 0x58, 0x5f, 0xa2, 0x02, 0x81, 0x81, 0x41, 0x00, 0x04, 0x58, 0x56, 0x86, 0x14, 0xa4, 0x01, 0x50, 0xfa, 0x6b, 0x4a, 0x53, 
    0xd5, 0xad, 0x5f, 0xdf, 0xbe, 0x9d, 0xe6, 0x63, 0xe4, 0xd4, 0x1f, 0xfe, 0x02, 0x50, 0x14, 0x92, 0xaf, 0x14, 0x25, 0x69, 0x5e, 0x48, 0xbf, 0x42, 0x9b, 0x2d, 0x51, 0xf2, 0xab, 0x45, 
    0x03, 0x58, 0x24, 0x82, 0x2f, 0x58, 0x20, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 
    0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x0e, 0x19, 0x87, 0xd0, 0x01, 0x0f, 0x02, 0x0f, 0x07, 0x43, 0x82, 0x03, 0x0f, 0x09, 0x43, 0x82, 0x17, 0x02, 
};

suit_err_t __wrap_suit_fetch_callback(suit_fetch_args_t fetch_args, suit_fetch_ret_t *fetch_ret)
{
    (void)fetch_args;
    (void)fetch_ret;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_condition_callback(suit_condition_args_t condition_args)
{
    (void)condition_args;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_invoke_callback(suit_invoke_args_t invoke_args)
{
    (void)invoke_args;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_store_callback(suit_store_args_t store_args)
{
    (void)store_args;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_wait_callback(suit_wait_args_t wait_args)
{
    (void)wait_args;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_report_callback(suit_report_args_t report_args)
{
    (void)report_args;
    return SUIT_SUCCESS;
}

int main(void)
{
    suit_err_t result = SUIT_SUCCESS;

    int i;
    #define NUM_PUBLIC_KEYS                 1
    UsefulBufC public_keys[NUM_PUBLIC_KEYS] = {
        trust_anchor_prime256v1_cose_key_public
    };

    suit_inputs_t *suit_inputs = calloc(1, sizeof(suit_inputs_t) + SUIT_MAX_DATA_SIZE);
    if (suit_inputs == NULL) {
        return EXIT_FAILURE;
    }
    suit_inputs->left_len = MAX_DATA_SIZE;
    suit_inputs->ptr = suit_inputs->buf;
    suit_inputs->key_len = NUM_PUBLIC_KEYS;

    for (i = 0; i < NUM_PUBLIC_KEYS; i++) {
        result = suit_set_mechanism_from_cose_key(public_keys[i], &suit_inputs->mechanisms[i]);
        if (result != SUIT_SUCCESS) {
            return EXIT_FAILURE;
        }
        suit_inputs->mechanisms[i].use = true;
        suit_inputs->mechanisms[i].cose_tag = CBOR_TAG_COSE_SIGN1;
    }

    // Read manifest file.
    suit_inputs->manifest.ptr = manifest_buf;
    suit_inputs->manifest.len = sizeof(manifest_buf);

    // Process manifest file.
    suit_inputs->process_flags.all = UINT16_MAX;
    result = suit_process_envelope(suit_inputs);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    free(suit_inputs);

    return EXIT_SUCCESS;
}
