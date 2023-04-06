/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#include "csuit/suit_cose.h"

/*!
    \file   suit_cose.c

    \brief  This implements Sign and Verify the COSE.
 */

/*
    Public function. See suit_cose.h
 */
cose_tag_key_t suit_judge_cose_tag_from_buf(const UsefulBufC signed_cose)
{
    /* judge authentication object
     * [ COSE_Sign_Tagged, COSE_Sign1_Tagged, COSE_Mac_Tagged, COSE_Mac0_Tagged ]
     */
    cose_tag_key_t result = COSE_TAG_INVALID;
    uint8_t tag0 = ((uint8_t *)signed_cose.ptr)[0];
    uint8_t tag1;
    switch (tag0) {
    case 0xd1: // Tag(17)
        result = COSE_MAC0_TAGGED;
        break;
    case 0xd2: // Tag(18)
        result = COSE_SIGN1_TAGGED;
        break;
    case 0xe8:
        tag1 = ((uint8_t *)signed_cose.ptr)[1];
        switch (tag1) {
        case 0x61: // Tag(97)
            result = COSE_MAC_TAGGED;
            break;
        case 0x62: // Tag(98)
            result = COSE_SIGN_TAGGED;
            break;
        }
    default:
        break;
    }
    return result;
}

suit_err_t suit_verify_cose_sign1(const UsefulBufC signed_cose,
                                  const suit_key_t *public_key,
                                  UsefulBufC returned_payload)
{
    suit_err_t result = SUIT_SUCCESS;
    struct t_cose_sign1_verify_ctx verify_ctx;
    struct t_cose_parameters parameters;
    enum t_cose_err_t cose_result;

    if (public_key == NULL) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }

    t_cose_sign1_verify_init(&verify_ctx, 0);
    t_cose_sign1_set_verification_key(&verify_ctx, public_key->cose_key);
    cose_result = t_cose_sign1_verify_detached(&verify_ctx,
                                               signed_cose,
                                               NULL_Q_USEFUL_BUF_C,
                                               returned_payload,
                                               &parameters);
    if (cose_result != T_COSE_SUCCESS) {
        result = SUIT_ERR_FAILED_TO_VERIFY;
    }
    return result;
}

suit_err_t suit_sign_cose_sign1(const UsefulBufC raw_cbor,
                                const suit_key_t *key_pair,
                                UsefulBuf *returned_payload)
{
    // Create cose signed buffer.
    struct t_cose_sign1_sign_ctx sign_ctx;
    enum t_cose_err_t cose_result;
    UsefulBufC tmp_signed_cose;

    t_cose_sign1_sign_init(&sign_ctx, 0, key_pair->cose_algorithm_id);
    t_cose_sign1_set_signing_key(&sign_ctx, key_pair->cose_key, NULL_Q_USEFUL_BUF_C);
    cose_result = t_cose_sign1_sign_detached(&sign_ctx, NULL_Q_USEFUL_BUF_C, raw_cbor, *returned_payload, &tmp_signed_cose);
    if (cose_result != T_COSE_SUCCESS) {
        returned_payload->len = 0;
        return SUIT_ERR_FAILED_TO_SIGN;
    }
    returned_payload->len = tmp_signed_cose.len;
    return SUIT_SUCCESS;
}

suit_err_t suit_decrypt_cose_encrypt(const UsefulBufC encrypted_payload,
                                     const UsefulBufC encryption_info,
                                     UsefulBuf working_buf,
                                     const suit_mechanism_t *mechanism,
                                     UsefulBufC *returned_payload)
{
    if (!mechanism->use ||
        (mechanism->cose_tag != CBOR_TAG_COSE_ENCRYPT0 &&
         mechanism->cose_tag != CBOR_TAG_COSE_ENCRYPT)) {
        return SUIT_ERR_FAILED_TO_DECRYPT;
    }

    struct t_cose_recipient_dec_keywrap kw_unwrap_recipient;
    struct t_cose_encrypt_dec_ctx decrypt_context;

    t_cose_encrypt_dec_init(&decrypt_context, T_COSE_OPT_MESSAGE_TYPE_ENCRYPT);
    t_cose_recipient_dec_keywrap_init(&kw_unwrap_recipient);
    t_cose_recipient_dec_keywrap_set_kek(&kw_unwrap_recipient, mechanism->key.cose_key, NULL_Q_USEFUL_BUF_C);
    t_cose_encrypt_dec_add_recipient(&decrypt_context, (struct t_cose_recipient_dec *)&kw_unwrap_recipient);
    UsefulBufC tmp;
    enum t_cose_err_t err = t_cose_encrypt_dec_detached(&decrypt_context, encryption_info, NULL_Q_USEFUL_BUF_C, encrypted_payload, working_buf, &tmp, NULL);
    if (err != T_COSE_SUCCESS) {
        return SUIT_ERR_FAILED_TO_DECRYPT;
    }
    *returned_payload = tmp;
    return SUIT_SUCCESS;
}

suit_err_t suit_encrypt_cose_encrypt(const UsefulBufC plaintext_payload,
                                     const suit_mechanism_t *mechanism,
                                     UsefulBuf encrypted_payload_buf,
                                     UsefulBuf encryption_info_buf,
                                     UsefulBufC *encrypted_payload,
                                     UsefulBufC *encryption_info)
{
    enum t_cose_err_t t_cose_err;
    struct t_cose_recipient_enc_keywrap kw_recipient;
    struct t_cose_encrypt_enc encrypt_context;

    t_cose_recipient_enc_keywrap_init(&kw_recipient, mechanism->key.cose_algorithm_id);
    t_cose_recipient_enc_keywrap_set_key(&kw_recipient, mechanism->key.cose_key, mechanism->kid);

    t_cose_encrypt_enc_init(&encrypt_context, T_COSE_OPT_MESSAGE_TYPE_ENCRYPT, T_COSE_ALGORITHM_A128GCM);
    t_cose_encrypt_add_recipient(&encrypt_context, (struct t_cose_recipient_enc *)&kw_recipient);

    t_cose_err = t_cose_encrypt_enc_detached(&encrypt_context,
                                             plaintext_payload,
                                             NULL_Q_USEFUL_BUF_C,
                                             encrypted_payload_buf,
                                             encryption_info_buf,
                                             encrypted_payload,
                                             encryption_info);
    if (t_cose_err != T_COSE_SUCCESS) {
        return SUIT_ERR_FAILED_TO_ENCRYPT;
    }
    return SUIT_SUCCESS;
}
