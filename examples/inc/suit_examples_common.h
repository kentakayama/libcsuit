/*
 * Copyright (c) 2020 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#ifndef SUIT_EXAMPLES_COMMON_H
#define SUIT_EXAMPLES_COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <csuit/suit_cose.h>

#define PRIME256V1_PRIVATE_KEY_LENGTH       32
#define PRIME256V1_PUBLIC_KEY_LENGTH        65
#define SECP384R1_PRIVATE_KEY_LENGTH        48
#define SECP384R1_PUBLIC_KEY_LENGTH         97
#define SECP521R1_PRIVATE_KEY_LENGTH        66
#define SECP521R1_PUBLIC_KEY_LENGTH         133
#define A128GCM_KEY_CHAR_LENGTH             16

size_t read_from_file(const char *file_path, uint8_t *buf, const size_t buf_len);
size_t write_to_file(const char *file_path, const void *buf, const size_t buf_len);

/*!
    \brief  Create ES256 public key

    \param[in]  public_key          Pointer of char array type of public key.
    \param[out] cose_public_key     Pointer of suit_key_t type of public key.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FATAL.

    The length of the char array public key is estimated from the algorithm and library.
 */

suit_err_t suit_key_init_es256_key_pair(const unsigned char *private_key, const unsigned char *public_key, suit_key_t *cose_key_pair);

/*!
    \brief  Create ES256 key pair

    \param[in]  public_key          Pointer of char array type of public key.
    \param[out] cose_public_key     Pointer of suit_key_t type of public key.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.

    The length of the char array public key is estimated from the algorithm and library.
 */
suit_err_t suit_key_init_es256_public_key(const unsigned char *public_key, suit_key_t *cose_key_pair);

suit_err_t suit_key_init_a128kw_secret_key(const unsigned char *secret_key, suit_key_t *cose_secret_key);

suit_err_t suit_free_key(const suit_key_t *key);
#endif  /* SUIT_EXAMPLES_COMMON_H */
