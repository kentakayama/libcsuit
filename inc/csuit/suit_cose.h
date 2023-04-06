/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#ifndef SUIT_COSE_H
#define SUIT_COSE_H

#include "csuit/suit_common.h"
#include "t_cose/t_cose_sign1_verify.h"
#include "t_cose/t_cose_sign1_sign.h"

#ifndef LIBCSUIT_DISABLE_ENCRYPTION
#include "t_cose/t_cose_encrypt_enc.h"
#include "t_cose/t_cose_encrypt_dec.h"
#include "t_cose/t_cose_recipient_enc_keywrap.h"
#include "t_cose/t_cose_recipient_dec_keywrap.h"
#endif

#if defined(LIBCSUIT_PSA_CRYPTO_C)
#include "psa/crypto.h"
#else
#include "openssl/opensslv.h"
#include "openssl/evp.h"
#include "openssl/ec.h"

#define OPENSSL_VERSION_111 0x10101000L
#define OPENSSL_VERSION_300 0x30000000L
#if OPENSSL_VERSION_NUMBER >= OPENSSL_VERSION_300
#include "openssl/param_build.h"
#endif

#endif /* LIBCSUIT_PSA_CRYPTO_C */

/*!
    \file   suit_cose.h

    \brief  Sign and Verify the COSE.

    SUIT supports only COSE_Sign, COSE_Sign1, COSE_Mac, and COSE_Mac0.
    Currently libcsuit only supports COSE_Sign1 signing and verification.
 */

typedef enum cose_tag_key {
    COSE_TAG_INVALID    = 0,
    COSE_SIGN_TAGGED    = 98,
    COSE_SIGN1_TAGGED   = 18,
    COSE_ENCRYPT        = 96,
    COSE_ENCRYPT0       = 16,
    COSE_MAC_TAGGED     = 97,
    COSE_MAC0_TAGGED    = 17,
    COSE_KEY            = 101,
    COSE_KEY_SET        = 102,
} cose_tag_key_t;




/*!
    \brief      Distinguish the TAG of the COSE binary.

    \param[in]  signed_cose     Pointer and length of COSE signed cbor.

    \return     This returns one of the error codes defined by \ref cose_tag_key_t.
 */
cose_tag_key_t suit_judge_cose_tag_from_buf(const UsefulBufC signed_cose);

typedef struct suit_key {
    const unsigned char *private_key;
    size_t private_key_len;
    const unsigned char *public_key;
    size_t public_key_len;
    int cose_algorithm_id;
    struct t_cose_key cose_key;
} suit_key_t;

typedef struct suit_mechanism {
    int cose_tag; // COSE_Sign1, COSE_Sign, COSE_Encrypt0, COSE_Encrypt, etc.
    suit_key_t key;
    UsefulBufC kid;
    bool use;
} suit_mechanism_t;

/*!
    \brief  Generate COSE_Sign1 sined payload.

    \param[in]  raw_cbor            Pointer and length of the target payload.
    \param[in]  key_pair            Pointer of private and public key pair.
    \param[out] returned_payload    Pointer and length of the resulting COSE_Sign1.

    \return     This returns SUIT_SUCCESS, SUIT_ERR_FAILED_TO_VERIFY or SUIT_ERR_FATAL.
 */
suit_err_t suit_sign_cose_sign1(const UsefulBufC raw_cbor,
                                const suit_key_t *key_pair,
                                UsefulBuf *returned_payload);

/*!
    \brief  Verify COSE_Sign signed payload.

    \param[in]  signed_cose             Pointer and length of the target signed payload.
    \param[in]  public_key              Pointer of public key.
    \param[in]  returned_payload        Pointer and length for verified payload.
                                        You can pass detached payload here.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.

    NOTE: Currently not implemented.
 */
suit_err_t suit_verify_cose_sign(const UsefulBufC signed_cose,
                                 const suit_key_t *public_key,
                                 UsefulBufC returned_payload);

/*!
    \brief  Verify COSE_Sign1 signed payload.

    \param[in]  signed_cose         Pointer and length of the target signed payload.
    \param[in]  public_key          Pointer of public key.
    \param[out] returned_payload    Pointer and length of the COSE_Sign1 signed target payload.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.

    COSE_Sign1 structure is like below.
    \code{.unparsed}
    COSE_Sign1 = [
        Headers,
        payload : bstr / nil,
        signature : bstr
    ]
    \endcode

    This function verifies whether the payload correspond to the signature,
    and then extracts payload to returned_payload if success.
 */
suit_err_t suit_verify_cose_sign1(const UsefulBufC signed_cose,
                                  const suit_key_t *public_key,
                                  UsefulBufC returned_payload);

/*!
    \brief  Verify COSE_Mac signed payload.

    \param[in]  signed_cose         Pointer and length of the target signed payload.
    \param[in]  public_key          Pointer of public key.
    \param[in]  returned_payload    Pointer and length for verified payload.
                                    You can pass detached payload here.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.
 */
suit_err_t suit_verify_cose_mac(const UsefulBufC signed_cose,
                                const suit_key_t *public_key,
                                UsefulBufC *returned_payload);

/*!
    \brief  Verify COSE_Mac0 signed payload.

    \param[in]  signed_cose         Pointer and length of the target signed payload.
    \param[in]  public_key          Pointer of public key.
    \param[out]  returned_payload    Pointer and length of the COSE_Mac0 signed target payload.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.
 */
suit_err_t suit_verify_cose_mac0(const UsefulBufC signed_cose,
                                 const suit_key_t *public_key,
                                 UsefulBufC *returned_payload);

/*!
    \brief  Decrypt COSE_Encrypt or COSE_Encrypt0 encrypted payload.

    \param[in]  encrypted_payload   Pointer and length of the target encrypted payload.
    \param[in]  encryption_info     Pointer and length of the COSE_Encrypt or COSE_Encrypt0.
    \param[in]  working_buf         Pointer and length of the working buffer for decryption.
    \param[in]  mechanism           Pointer of \t suit_mechanism_t.
    \param[out] returned_payload    Pointer and length of the plaintext payload.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_DECRYPT.
 */
suit_err_t suit_decrypt_cose_encrypt(const UsefulBufC encrypted_payload,
                                     const UsefulBufC encryption_info,
                                     UsefulBuf working_buf,
                                     const suit_mechanism_t *mechanism,
                                     UsefulBufC *retuned_payload);
/*!
    \brief  Encrypt COSE_Encrypt or COSE_Encrypt0 encrypted payload.

    \param[in]  plaintext_payload       Pointer and length of the plaintext payload.
    \param[in]  mechanism               Pointer of \t suit_mechanism_t.
    \param[in]  encrypted_payload_buf   Pointer and length of the working buffer for encrypted payload.
    \param[in]  encryption_info_buf     Pointer and length of the working buffer for encryption_info.
    \param[out] encrypted_payload       Pointer and length of the plaintext payload.
    \param[out] encryption_info         Pointer and length of the COSE_Encrypt or COSE_Encrypt0.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_DECRYPT.
 */
suit_err_t suit_encrypt_cose_encrypt(const UsefulBufC plaintext_payload,
                                     const suit_mechanism_t *mechanism,
                                     UsefulBuf encryped_payload_buf,
                                     UsefulBuf encryption_info_buf,
                                     UsefulBufC *encrypted_payload,
                                     UsefulBufC *encryption_info);

#endif  /* SUIT_COSE_H */

