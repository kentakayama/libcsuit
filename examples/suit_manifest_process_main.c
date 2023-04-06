/*
 * Copyright (c) 2020 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process_main.c

    \brief  A sample to use libcsuit processing
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // chmod
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <unistd.h> // fork
#include <fcntl.h> // AT_FDCWD
#include "qcbor/qcbor.h"
#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_print.h"
#include "csuit/suit_cose.h"
#include "csuit/suit_digest.h"
#include "suit_examples_common.h"
#include "trust_anchor_prime256v1_pub.h"
#include "trust_anchor_a128_secret_key.h"

bool is_available_char_for_filename(const char c)
{
    return (('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            ('_' == c) ||
            ('.' == c) ||
            ('-' == c));
}

suit_err_t suit_component_identifier_to_filename(const suit_component_identifier_t *comp_id,
                                                 const size_t max_filename_len,
                                                 char filename[])
{
    size_t pos = 0;

    pos += sprintf(&filename[pos], "./tmp");
    for (size_t i = 0; i < comp_id->len; i++) {
        if (pos + 1 + 1 > max_filename_len) {
            return SUIT_ERR_NO_MEMORY;
        }
        pos += sprintf(&filename[pos], "/");

        if (pos + comp_id->identifier[i].len + 1 > max_filename_len) {
            return SUIT_ERR_NO_MEMORY;
        }
        for (size_t j = 0; j < comp_id->identifier[i].len; j++) {
            if (is_available_char_for_filename(comp_id->identifier[i].ptr[j])) {
                filename[pos++] = comp_id->identifier[i].ptr[j];
            }
            else {
                pos += sprintf(&filename[pos], "%02x", comp_id->identifier[i].ptr[j]);
            }
        }
    }
    filename[pos] = '\0';

    return SUIT_SUCCESS;
}

#define NUM_PUBLIC_KEYS                 1
/* TC signer's public_key */
const unsigned char *public_keys[NUM_PUBLIC_KEYS] = {
    trust_anchor_prime256v1_public_key,
};
#define NUM_SECRET_KEYS                 1
/* TC signer's secret_key */
const unsigned char *secret_keys[NUM_SECRET_KEYS] = {
    trust_anchor_a128_secret_key,
};

const uint8_t tc_uri[] = {
    0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x65, 0x78,
    0x61, 0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x6F, 0x72, 0x67, 0x2F,
    0x38, 0x64, 0x38, 0x32, 0x35, 0x37, 0x33, 0x61, 0x2D, 0x39,
    0x32, 0x36, 0x64, 0x2D, 0x34, 0x37, 0x35, 0x34, 0x2D, 0x39,
    0x33, 0x35, 0x33, 0x2D, 0x33, 0x32, 0x64, 0x63, 0x32, 0x39,
    0x39, 0x39, 0x37, 0x66, 0x37, 0x34, 0x2E, 0x74, 0x61
}; // "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.ta";
const uint8_t tc_data[] = {
    0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x53, 0x65, 0x63,
    0x75, 0x72, 0x65, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21
}; // "Hello, Secure World!"
const uint8_t depend_uri[] = {
    0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x65, 0x78,
    0x61, 0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x6F, 0x72, 0x67, 0x2F,
    0x38, 0x64, 0x38, 0x32, 0x35, 0x37, 0x33, 0x61, 0x2D, 0x39,
    0x32, 0x36, 0x64, 0x2D, 0x34, 0x37, 0x35, 0x34, 0x2D, 0x39,
    0x33, 0x35, 0x33, 0x2D, 0x33, 0x32, 0x64, 0x63, 0x32, 0x39,
    0x39, 0x39, 0x37, 0x66, 0x37, 0x34, 0x2E, 0x73, 0x75, 0x69,
    0x74
}; // "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit";
const uint8_t depend_suit[] = {
    0xD8, 0x6B, 0xA2, 0x02, 0x58, 0x73, 0x82, 0x58, 0x24, 0x82,
    0x2F, 0x58, 0x20, 0xB8, 0x92, 0xB7, 0x5B, 0x26, 0x2B, 0x72,
    0x8E, 0xC2, 0xC1, 0x6C, 0x27, 0x74, 0xA7, 0x30, 0xDC, 0x80,
    0x4B, 0xC8, 0x72, 0x52, 0xDA, 0xFC, 0x57, 0xA0, 0x09, 0x82,
    0x84, 0xC3, 0xE7, 0x35, 0x87, 0x58, 0x4A, 0xD2, 0x84, 0x43,
    0xA1, 0x01, 0x26, 0xA0, 0xF6, 0x58, 0x40, 0x36, 0xB4, 0x77,
    0xDB, 0x29, 0x8D, 0x1A, 0x55, 0x4A, 0x52, 0xB3, 0x41, 0x4C,
    0xDC, 0xCE, 0x85, 0x69, 0xD8, 0xB2, 0xBD, 0xE6, 0xC0, 0xD3,
    0xA2, 0x37, 0xDC, 0x08, 0x8B, 0x74, 0x9F, 0x22, 0x8C, 0xE0,
    0xD0, 0x3E, 0xFD, 0xF0, 0x89, 0x47, 0x47, 0x88, 0x54, 0x93,
    0x46, 0xF5, 0xDE, 0x21, 0xAB, 0x03, 0x06, 0x6D, 0xA2, 0xD1,
    0xC4, 0xD1, 0xF4, 0xB1, 0x06, 0xD6, 0x7A, 0xCE, 0x3B, 0x07,
    0x26, 0x03, 0x59, 0x01, 0x01, 0xA5, 0x01, 0x01, 0x02, 0x03,
    0x03, 0x58, 0x84, 0xA2, 0x02, 0x81, 0x84, 0x4B, 0x54, 0x45,
    0x45, 0x50, 0x2D, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x48,
    0x53, 0x65, 0x63, 0x75, 0x72, 0x65, 0x46, 0x53, 0x50, 0x8D,
    0x82, 0x57, 0x3A, 0x92, 0x6D, 0x47, 0x54, 0x93, 0x53, 0x32,
    0xDC, 0x29, 0x99, 0x7F, 0x74, 0x42, 0x74, 0x61, 0x04, 0x58,
    0x54, 0x86, 0x14, 0xA4, 0x01, 0x50, 0xC0, 0xDD, 0xD5, 0xF1,
    0x52, 0x43, 0x56, 0x60, 0x87, 0xDB, 0x4F, 0x5B, 0x0A, 0xA2,
    0x6C, 0x2F, 0x02, 0x50, 0xDB, 0x42, 0xF7, 0x09, 0x3D, 0x8C,
    0x55, 0xBA, 0xA8, 0xC5, 0x26, 0x5F, 0xC5, 0x82, 0x0F, 0x4E,
    0x03, 0x58, 0x24, 0x82, 0x2F, 0x58, 0x20, 0x8C, 0xF7, 0x1A,
    0xC8, 0x6A, 0xF3, 0x1B, 0xE1, 0x84, 0xEC, 0x7A, 0x05, 0xA4,
    0x11, 0xA8, 0xC3, 0xA1, 0x4F, 0xD9, 0xB7, 0x7A, 0x30, 0xD0,
    0x46, 0x39, 0x74, 0x81, 0x46, 0x94, 0x68, 0xEC, 0xE8, 0x0E,
    0x14, 0x01, 0x0F, 0x02, 0x0F, 0x05, 0x84, 0x4B, 0x54, 0x45,
    0x45, 0x50, 0x2D, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x48,
    0x53, 0x65, 0x63, 0x75, 0x72, 0x65, 0x46, 0x53, 0x50, 0x8D,
    0x82, 0x57, 0x3A, 0x92, 0x6D, 0x47, 0x54, 0x93, 0x53, 0x32,
    0xDC, 0x29, 0x99, 0x7F, 0x74, 0x44, 0x73, 0x75, 0x69, 0x74,
    0x11, 0x58, 0x45, 0x86, 0x14, 0xA1, 0x15, 0x78, 0x3B, 0x68,
    0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x65, 0x78, 0x61,
    0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x38,
    0x64, 0x38, 0x32, 0x35, 0x37, 0x33, 0x61, 0x2D, 0x39, 0x32,
    0x36, 0x64, 0x2D, 0x34, 0x37, 0x35, 0x34, 0x2D, 0x39, 0x33,
    0x35, 0x33, 0x2D, 0x33, 0x32, 0x64, 0x63, 0x32, 0x39, 0x39,
    0x39, 0x37, 0x66, 0x37, 0x34, 0x2E, 0x74, 0x61, 0x15, 0x0F,
    0x03, 0x0F
}; // suit_manifest_expU.md
const uint8_t config_uri[] = {
    0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x65, 0x78,
    0x61, 0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x6F, 0x72, 0x67, 0x2F,
    0x63, 0x6F, 0x6E, 0x66, 0x69, 0x67, 0x2E, 0x6A, 0x73, 0x6F,
    0x6E
}; // "https://example.org/config.json";
const uint8_t config_data[] = {
    0x7B, 0x22, 0x6E, 0x61, 0x6D, 0x65, 0x22, 0x3A, 0x20, 0x22,
    0x46, 0x4F, 0x4F, 0x20, 0x42, 0x61, 0x72, 0x22, 0x2C, 0x20,
    0x22, 0x74, 0x6F, 0x6B, 0x65, 0x6E, 0x22, 0x3A, 0x20, 0x22,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x61, 0x62, 0x66, 0x63, 0x64, 0x65, 0x66, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63,
    0x64, 0x65, 0x22, 0x7D
};// "{\"name\": \"FOO Bar\", \"token\": \"0123456789abfcdef0123456789abcde\"}";
const uint8_t dependent_uri[] = {
    0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x65, 0x78, 0x61,
    0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x64,
    0x65, 0x70, 0x65, 0x6E, 0x64, 0x65, 0x6E, 0x74, 0x2E, 0x73,
    0x75, 0x69, 0x74
}; // "http://example.com/dependent.suit
const uint8_t dependent_data[] = {
    0xD8, 0x6B, 0xA2, 0x02, 0x58, 0x73, 0x82, 0x58, 0x24, 0x82,
    0x2F, 0x58, 0x20, 0xF5, 0xCC, 0x80, 0x9E, 0xDF, 0x46, 0x61,
    0xA5, 0x2C, 0xD0, 0x40, 0x25, 0xB1, 0xDF, 0x06, 0xA8, 0x03,
    0xF7, 0x5E, 0xB6, 0x19, 0xDE, 0x7A, 0x7C, 0x54, 0x72, 0x0B,
    0xFB, 0xA6, 0x82, 0x8C, 0x63, 0x58, 0x4A, 0xD2, 0x84, 0x43,
    0xA1, 0x01, 0x26, 0xA0, 0xF6, 0x58, 0x40, 0x33, 0xF2, 0x11,
    0x43, 0xF9, 0x78, 0x9B, 0xD4, 0xAD, 0x61, 0xCA, 0x79, 0x28,
    0xDD, 0xC0, 0xCB, 0x94, 0x85, 0x52, 0x79, 0x0A, 0x64, 0x1F,
    0x5E, 0xFD, 0xD4, 0x5E, 0xAE, 0x5A, 0x4A, 0x61, 0x1A, 0xD4,
    0xBB, 0x7D, 0x58, 0x3F, 0xFD, 0x43, 0xBC, 0x2F, 0xF6, 0x3B,
    0x45, 0x8B, 0xD0, 0x92, 0xEE, 0xB3, 0xBA, 0x36, 0x1C, 0xFE,
    0x19, 0x32, 0xDC, 0xA7, 0x45, 0x38, 0xB3, 0x96, 0x01, 0xEB,
    0x82, 0x03, 0x58, 0x42, 0xA6, 0x01, 0x01, 0x02, 0x00, 0x03,
    0x47, 0xA1, 0x02, 0x81, 0x81, 0x42, 0x30, 0x30, 0x05, 0x81,
    0x4E, 0x64, 0x65, 0x70, 0x65, 0x6E, 0x64, 0x65, 0x6E, 0x74,
    0x2E, 0x73, 0x75, 0x69, 0x74, 0x11, 0x52, 0x84, 0x14, 0xA1,
    0x12, 0x4B, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F,
    0x72, 0x6C, 0x64, 0x12, 0x0F, 0x17, 0x4D, 0x84, 0x14, 0xA1,
    0x17, 0x46, 0x63, 0x61, 0x74, 0x20, 0x30, 0x30, 0x17, 0x0F
}; // suit_manifest_expD00.suit
const uint8_t encrypted_firmware_uri[] = {
    0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x61, 0x75,
    0x74, 0x68, 0x6F, 0x72, 0x2E, 0x65, 0x78, 0x61, 0x6D, 0x70,
    0x6C, 0x65, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x65, 0x6E, 0x63,
    0x72, 0x79, 0x70, 0x74, 0x65, 0x64, 0x2D, 0x66, 0x69, 0x72,
    0x6D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x62, 0x69, 0x6E
}; // "https://author.example.com/encrypted-firmware.bin"
const uint8_t encrypted_firmware_data[] = {
    0x42, 0x98, 0xCC, 0xB3, 0x20, 0xD1, 0x2C, 0x0A, 0xE2, 0xA8,
    0x52, 0x4E, 0x34, 0xC9, 0x26, 0xCA, 0x17, 0xFD, 0x33, 0xE5,
    0x62, 0x77, 0xC0, 0x8F, 0xE8, 0x82, 0x82, 0x0B, 0x8A, 0xCF,
    0x52, 0xED, 0x4C, 0x52, 0x9A, 0xD1, 0xBB, 0xF8, 0x7B, 0xBD,
    0xF8, 0xA2, 0x89, 0x70, 0x43, 0xE8, 0xAF
};

struct name_data {
    const uint8_t *name;
    size_t name_len;
    const uint8_t *data;
    size_t data_len;
};
#define SUIT_NAME_DATA_LEN 5
const struct name_data name_data[] = {
    {.name = tc_uri, .name_len = sizeof(tc_uri), .data = tc_data, .data_len = sizeof(tc_data)},
    {.name = depend_uri, .name_len = sizeof(depend_uri), .data = depend_suit, .data_len = sizeof(depend_suit)},
    {.name = config_uri, .name_len = sizeof(config_uri), .data = config_data, .data_len = sizeof(config_data)},
    {.name = dependent_uri, .name_len = sizeof(dependent_uri), .data = dependent_data, .data_len = sizeof(dependent_data)},
    {.name = encrypted_firmware_uri, .name_len = sizeof(encrypted_firmware_uri), .data = encrypted_firmware_data, .data_len = sizeof(encrypted_firmware_data)},
};

suit_err_t __real_suit_fetch_callback(suit_fetch_args_t fetch_args, suit_fetch_ret_t *fetch_ret);
suit_err_t __wrap_suit_fetch_callback(suit_fetch_args_t fetch_args, suit_fetch_ret_t *fetch_ret)
{
    suit_err_t result = __real_suit_fetch_callback(fetch_args, fetch_ret);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    char filename[SUIT_MAX_NAME_LENGTH];
    result = suit_component_identifier_to_filename(&fetch_args.dst, SUIT_MAX_NAME_LENGTH, filename);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    size_t i = 0;
    for (i = 0; i < SUIT_NAME_DATA_LEN; i++) {
        if (name_data[i].name_len == fetch_args.uri_len &&
            memcmp(name_data[i].name, fetch_args.uri, fetch_args.uri_len) == 0) {
            if (fetch_args.buf_len < name_data[i].data_len) {
                return SUIT_ERR_NO_MEMORY;
            }
            if (fetch_args.ptr != NULL) {
                memcpy(fetch_args.ptr, name_data[i].data, name_data[i].data_len);
                fetch_ret->buf_len = name_data[i].data_len;
            }
            write_to_file(filename, fetch_args.ptr, name_data[i].data_len);
            printf("fetched %s\n\n", name_data[i].name);
            break;
        }
    }
    if (i == SUIT_NAME_DATA_LEN) {
        /* not found */
        /* ignore this for testing example 0-5 only */
        //return SUIT_ERR_NOT_FOUND;
        fetch_ret->buf_len = fetch_args.buf_len;
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_condition_check_content(const suit_component_identifier_t *dst,
                                        UsefulBufC content)
{
    char filename[SUIT_MAX_NAME_LENGTH];
    suit_err_t result = suit_component_identifier_to_filename(dst, SUIT_MAX_NAME_LENGTH, filename);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    UsefulBuf buf;
    buf.ptr = malloc(content.len + 1);
    if (buf.ptr == NULL) {
        return SUIT_ERR_NO_MEMORY;
    }
    buf.len = read_from_file(filename, buf.ptr, content.len + 1);

    /* see https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest-22#name-suit-condition-check-conten */
    uint8_t residual = 0;
    for (size_t i = 0; i < content.len; i++) {
        residual |= ((uint8_t *)content.ptr)[i] ^ ((uint8_t *)buf.ptr)[i];
    }
    return (residual == 0) ? SUIT_SUCCESS : SUIT_ERR_CONDITION_MISMATCH;
}

suit_err_t suit_condition_image_match(const suit_component_identifier_t *dst,
                                      const suit_digest_t *image_digest,
                                      const uint64_t image_size,
                                      bool condition_match)
{
    char filename[SUIT_MAX_NAME_LENGTH];
    suit_err_t result = suit_component_identifier_to_filename(dst, SUIT_MAX_NAME_LENGTH, filename);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    suit_buf_t buf;
    if (image_size == 0) {
        buf.ptr = malloc(SUIT_MAX_DATA_SIZE);
        if (buf.ptr == NULL) {
            return SUIT_ERR_NO_MEMORY;
        }
        buf.len = read_from_file(filename, buf.ptr, SUIT_MAX_DATA_SIZE);
    }
    else {
        buf.ptr = malloc(image_size + 1);
        if (buf.ptr == NULL) {
            return SUIT_ERR_NO_MEMORY;
        }
        buf.len = read_from_file(filename, buf.ptr, image_size + 1);
        if (buf.len != image_size) {
            return SUIT_ERR_CONDITION_MISMATCH;
        }
    }
    result = suit_verify_digest(&buf, image_digest);
    free(buf.ptr);
    if (result == SUIT_ERR_FAILED_TO_VERIFY) {
        result = SUIT_ERR_CONDITION_MISMATCH;
    }
    return result;
}

suit_err_t __real_suit_condition_callback(suit_condition_args_t condition_args);
suit_err_t __wrap_suit_condition_callback(suit_condition_args_t condition_args)
{
    suit_err_t result = __real_suit_condition_callback(condition_args);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    bool match = true;
    switch (condition_args.condition) {
    /* bstr */
    case SUIT_CONDITION_VENDOR_IDENTIFIER:
    case SUIT_CONDITION_CLASS_IDENTIFIER:
    case SUIT_CONDITION_DEVICE_IDENTIFIER:
        result = SUIT_ERR_NOT_IMPLEMENTED;
    case SUIT_CONDITION_CHECK_CONTENT:
        result = suit_condition_check_content(&condition_args.dst, condition_args.expected.str);
        break;

    /* SUIT_Digest */
    case SUIT_CONDITION_IMAGE_NOT_MATCH:
        match = false;
    case SUIT_CONDITION_IMAGE_MATCH:
        result = suit_condition_image_match(&condition_args.dst, &condition_args.expected.image_digest, condition_args.expected.image_size, match);
        break;

    case SUIT_CONDITION_COMPONENT_SLOT:
    case SUIT_CONDITION_ABORT:
    case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
    case SUIT_CONDITION_IS_DEPENDENCY:
    case SUIT_CONDITION_USE_BEFORE:
    case SUIT_CONDITION_MINIMUM_BATTERY:
    case SUIT_CONDITION_UPDATE_AUTHORIZED:
    case SUIT_CONDITION_VERSION:
    default:
        result = SUIT_ERR_NOT_IMPLEMENTED;
    }

    if (result != SUIT_SUCCESS) {
        printf("main : error = %s(%d)\n", suit_err_to_str(result), result);
        printf("main : suppress it for testing.\n\n");
        result = SUIT_SUCCESS;
    }
    return result;
}

suit_err_t __real_suit_invoke_callback(suit_invoke_args_t invoke_args);
suit_err_t __wrap_suit_invoke_callback(suit_invoke_args_t invoke_args)
{
    suit_err_t result = __real_suit_invoke_callback(invoke_args);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    char cd[] = "./tmp";
    char command[SUIT_MAX_NAME_LENGTH];
    snprintf(command, invoke_args.args_len + 1, "%s", (char *)invoke_args.args);

    pid_t pid = fork();
    if (pid == 0) {
        /* child */
        int ret;
        ret = chdir(cd);
        if (ret != 0) {
            printf("(sub) Failed to set working directory at \"%s\"\n", cd);
            return SUIT_ERR_FATAL;
        }
        printf("<sub>$ cd %s\n", cd);
        printf("<sub>$ %s\n", command);
        ret = system(command);
        printf("\n");
        fflush(stdout);
        exit(ret);
    }
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("<sub> Command exited with %d\n", WEXITSTATUS(status));
            return SUIT_SUCCESS;
        }
        else {
            printf("<sub> Command terminated %u\n", status);
            return SUIT_ERR_FATAL;
        }
    }
    /* XXX: DO NOT REACH HERE */
    return SUIT_ERR_FATAL;
}

suit_err_t store_component(const char *dst,
                           UsefulBufC src,
                           UsefulBufC encryption_info,
                           suit_mechanism_t mechanisms[])
{
    suit_err_t result = SUIT_SUCCESS;
    UsefulBuf decrypted_payload_buf = NULLUsefulBuf;

    if (!UsefulBuf_IsNULLOrEmptyC(encryption_info)) {
        decrypted_payload_buf.ptr = malloc(SUIT_MAX_DATA_SIZE);
        decrypted_payload_buf.len = SUIT_MAX_DATA_SIZE;
        UsefulBufC tmp = NULLUsefulBufC;
        for (size_t i = 0; i < SUIT_MAX_KEY_NUM; i++) {
            result = suit_decrypt_cose_encrypt(src, encryption_info, decrypted_payload_buf, &mechanisms[i], &tmp);
            if (result == SUIT_SUCCESS) {
                break;
            }
        }
        if (result != SUIT_SUCCESS || UsefulBuf_IsNULLOrEmptyC(tmp)) {
            result = SUIT_ERR_FAILED_TO_DECRYPT;
            goto out;
        }
        src = tmp;
    }

    size_t len = write_to_file(dst, src.ptr, src.len);
    if (len != src.len) {
        result = SUIT_ERR_FATAL;
    }

out:
    if (decrypted_payload_buf.ptr != NULL) {
        free(decrypted_payload_buf.ptr);
    }
    return result;
}

suit_err_t copy_component(const char *dst,
                          const char *src,
                          UsefulBufC encryption_info,
                          suit_mechanism_t mechanisms[])
{
    UsefulBuf buf;
    buf.ptr = malloc(SUIT_MAX_DATA_SIZE);
    if (buf.ptr == NULL) {
        return SUIT_ERR_NO_MEMORY;
    }
    buf.len = SUIT_MAX_DATA_SIZE;
    size_t len = read_from_file(src, buf.ptr, buf.len);
    if (len >= buf.len) {
        return SUIT_ERR_NO_MEMORY;
    }
    buf.len = len;
    suit_err_t result = store_component(dst, UsefulBuf_Const(buf), encryption_info, mechanisms);
    free(buf.ptr);
    return result;
}

suit_err_t swap_component(const char *dst,
                          const char *src)
{
    char tmp[SUIT_MAX_NAME_LENGTH];
    size_t len = snprintf(tmp, SUIT_MAX_NAME_LENGTH, "%s.tmp", dst);
    if (len == SUIT_MAX_NAME_LENGTH) {
        return SUIT_ERR_NO_MEMORY;
    }
    if (rename(tmp, dst) != 0 || rename(dst, src) != 0 || rename(src, tmp)) {
        return SUIT_ERR_FATAL;
    }
    return SUIT_SUCCESS;
}

suit_err_t __real_suit_store_callback(suit_store_args_t store_args);
suit_err_t __wrap_suit_store_callback(suit_store_args_t store_args)
{
    suit_err_t result = __real_suit_store_callback(store_args);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    char dst[SUIT_MAX_NAME_LENGTH];
    char src[SUIT_MAX_NAME_LENGTH];
    result = suit_component_identifier_to_filename(&store_args.dst, SUIT_MAX_NAME_LENGTH, dst);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    switch (store_args.operation) {
    case SUIT_STORE:
        result = store_component(dst, store_args.src_buf, store_args.encryption_info, store_args.mechanisms);
        break;
    case SUIT_COPY:
        result = suit_component_identifier_to_filename(&store_args.src, SUIT_MAX_NAME_LENGTH, src);
        if (result == SUIT_SUCCESS) {
            result = copy_component(dst, src, store_args.encryption_info, store_args.mechanisms);
        }
        break;
    case SUIT_SWAP:
        result = suit_component_identifier_to_filename(&store_args.src, SUIT_MAX_NAME_LENGTH, src);
        if (result == SUIT_SUCCESS) {
            result = swap_component(dst, src);
            //result = (renameat2(AT_FDCWD, dst, AT_FDCWD, src, RENAME_EXCHANGE) == 0) ? SUIT_SUCCESS : SUIT_ERR_FATAL;
        }
        break;
    case SUIT_UNLINK:
        result = (unlink(dst) == 0) ? SUIT_SUCCESS : SUIT_ERR_FATAL;
        break;
    }
    if (result != SUIT_SUCCESS) {
        printf("main : error = %s(%d)\n", suit_err_to_str(result), result);
        printf("main : suppress it for testing.\n\n");
        result = SUIT_SUCCESS;
    }
    return result;
}

int main(int argc, char *argv[])
{
    // check arguments.
    if (argc < 2) {
        printf("%s <manifest file path>", argv[0]);
        return EXIT_FAILURE;
    }
    int32_t result = 0;
    int i;

    suit_inputs_t *suit_inputs = calloc(1, sizeof(suit_inputs_t) + SUIT_MAX_DATA_SIZE);
    if (suit_inputs == NULL) {
        printf("main : Failed to allocate memory for suit_inputs\n");
        return EXIT_FAILURE;
    }
    suit_inputs->left_len = SUIT_MAX_DATA_SIZE;
    suit_inputs->ptr = suit_inputs->buf;
    suit_inputs->key_len = NUM_PUBLIC_KEYS;

    printf("\nmain : Read public keys.\n");
    for (i = 0; i < NUM_PUBLIC_KEYS; i++) {
        result = suit_key_init_es256_public_key(public_keys[i], &suit_inputs->mechanisms[i].key);
        if (result != SUIT_SUCCESS) {
            printf("\nmain : Failed to initialize public key. %s(%d)\n", suit_err_to_str(result), result);
            return EXIT_FAILURE;
        }
        suit_inputs->mechanisms[i].use = true;
        suit_inputs->mechanisms[i].cose_tag = CBOR_TAG_COSE_SIGN1;
    }

#ifndef LIBCSUIT_DISABLE_ENCRYPTION
    printf("\nmain : Read secret keys.\n");
    for (size_t j = 0; j < NUM_SECRET_KEYS; j++) {
        result = suit_key_init_a128kw_secret_key(secret_keys[j], &suit_inputs->mechanisms[i + j].key);
        if (result != SUIT_SUCCESS) {
            printf("\nmain : Failed to initialize sycret key. %s(%d)\n", suit_err_to_str(result), result);
            return EXIT_FAILURE;
        }
        suit_inputs->mechanisms[i].use = true;
        suit_inputs->mechanisms[i].cose_tag = CBOR_TAG_COSE_ENCRYPT;
    }
#endif

    // Read manifest file.
    printf("\nmain : Read Manifest file.\n");
    suit_inputs->manifest.ptr = suit_inputs->buf;
    suit_inputs->manifest.len = read_from_file(argv[1], suit_inputs->buf, SUIT_MAX_DATA_SIZE);
    if (suit_inputs->manifest.len <= 0) {
        printf("main : Failed to read Manifest file.\n");
        return EXIT_FAILURE;
    }
    suit_inputs->left_len -= suit_inputs->manifest.len;

    // Process manifest file.
    printf("\nmain : Process Manifest file.\n");
    suit_inputs->process_flags.all = UINT16_MAX;
    result = suit_process_envelope(suit_inputs);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to install and invoke a Manifest file. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    free(suit_inputs);

    return EXIT_SUCCESS;
}

