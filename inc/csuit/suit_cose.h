/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

/*!
    \file   suit_cose.h

    \brief  Sign and Verify the COSE.

    SUIT supports only COSE_Sign, COSE_Sign1, COSE_Mac, and COSE_Mac0.
    Currently libcsuit only supports COSE_Sign1 signing and verification.
 */

#ifndef SUIT_COSE_H
#define SUIT_COSE_H

#include "csuit/suit_common.h"
#include "qcbor/qcbor_decode.h"
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

#ifdef __cplusplus
extern "C" {
#endif

#define SUIT_COSE_CNF                   (8)
#define SUIT_COSE_COSE_KEY              (1)
#define SUIT_COSE_ENCRYPTED_COSE_KEY    (2)
#define SUIT_COSE_KID                   (3)
#define SUIT_COSE_CRV_P256              (1)
#define SUIT_COSE_CRV_P384              (2)
#define SUIT_COSE_CRV_P521              (3)
#define SUIT_COSE_CRV_X25519            (4)
#define SUIT_COSE_CRV_X448              (5)
#define SUIT_COSE_CRV_ED25519           (6)
#define SUIT_COSE_CRV_ED448             (7)
#define SUIT_COSE_KTY                   (1)
#define SUIT_COSE_KTY_EC2               (2)
#define SUIT_COSE_KTY_SYMMETRIC         (4)
#define SUIT_COSE_KTY_HSS_LMS           (5)

#define PRIME256V1_PRIVATE_KEY_LENGTH       32
#define PRIME256V1_PUBLIC_KEY_LENGTH        65
#define PRIME256V1_PUBLIC_KEY_X_LENGTH      32
#define PRIME256V1_PUBLIC_KEY_Y_LENGTH      32
#define SECP384R1_PRIVATE_KEY_LENGTH        48
#define SECP384R1_PUBLIC_KEY_LENGTH         97
#define SECP521R1_PRIVATE_KEY_LENGTH        66
#define SECP521R1_PUBLIC_KEY_LENGTH         133
#define A128GCM_KEY_CHAR_LENGTH             16

#define SUIT_MAX_PRIVATE_KEY_LEN            SECP521R1_PRIVATE_KEY_LENGTH
#define SUIT_MAX_PUBLIC_KEY_LEN             SECP521R1_PUBLIC_KEY_LENGTH

/*!
    \brief  Generate COSE_Sign1 signed payload.

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
                                 UsefulBufC *returned_payload);

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

    This function verifies whether the payload corresponds to the signature,
    and then extracts payload to returned_payload if success.
 */
suit_err_t suit_verify_cose_sign1(const UsefulBufC signed_cose,
                                  const suit_key_t *public_key,
                                  UsefulBufC *returned_payload);

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

suit_err_t suit_set_mechanism_from_cose_key_from_item(QCBORDecodeContext *context,
                                                      QCBORItem *item,
                                                      suit_mechanism_t *mechanism);
suit_err_t suit_set_mechanism_from_cose_key(UsefulBufC buf,
                                            suit_mechanism_t *mechanism);
suit_err_t suit_set_mechanism_from_cwt_payload(UsefulBufC payload,
                                               suit_mechanism_t *mechanism);

#ifdef __cplusplus
}
#endif

#endif  /* SUIT_COSE_H */
