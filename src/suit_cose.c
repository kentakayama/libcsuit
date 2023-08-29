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
cbor_tag_key_t suit_judge_cose_tag_from_buf(const UsefulBufC signed_cose)
{
    /* judge authentication object
     * [ COSE_Sign, COSE_Sign1, COSE_Mac, COSE_Mac0, COSE_Encrypt, COSE_Encrypt0 ]
     */
    cbor_tag_key_t result = COSE_TAG_INVALID;
    uint8_t tag0 = ((uint8_t *)signed_cose.ptr)[0];
    uint8_t tag1;
    switch (tag0) {
    case 0xd0: // Tag(16)
        result = COSE_ENCRYPT0_TAG;
        break;
    case 0xd1: // Tag(17)
        result = COSE_MAC0_TAG;
        break;
    case 0xd2: // Tag(18)
        result = COSE_SIGN1_TAG;
        break;
    case 0xe8:
        tag1 = ((uint8_t *)signed_cose.ptr)[1];
        switch (tag1) {
        case COSE_ENCRYPT_TAG:
            result = COSE_ENCRYPT_TAG;
            break;
        case COSE_MAC_TAG:
            result = COSE_MAC_TAG;
            break;
        case COSE_SIGN_TAG:
            result = COSE_SIGN_TAG;
            break;
        }
    default:
        break;
    }
    return result;
}

suit_err_t suit_verify_cose_sign1(const UsefulBufC signed_cose,
                                  const suit_key_t *public_key,
                                  UsefulBufC *returned_payload)
{
    struct t_cose_sign1_verify_ctx verify_ctx;
    enum t_cose_err_t cose_result;

    if (public_key == NULL) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }

    t_cose_sign1_verify_init(&verify_ctx, 0);
    t_cose_sign1_set_verification_key(&verify_ctx, public_key->cose_key);
    if (UsefulBuf_IsNULLOrEmptyC(*returned_payload)) {
        cose_result = t_cose_sign1_verify(&verify_ctx,
                                           signed_cose,
                                           returned_payload,
                                           NULL);
    }
    else {
        cose_result = t_cose_sign1_verify_detached(&verify_ctx,
                                                    signed_cose,
                                                    NULL_Q_USEFUL_BUF_C,
                                                   *returned_payload,
                                                    NULL);
    }
    if (cose_result != T_COSE_SUCCESS) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }
    return SUIT_SUCCESS;
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

#ifndef LIBCSUIT_DISABLE_ENCRYPTION
enum t_cose_err_t suit_decrypt_cose_encrypt_esdh(const UsefulBufC encrypted_payload,
                                                 const UsefulBufC encryption_info,
                                                 UsefulBuf working_buf,
                                                 const suit_mechanism_t *mechanism,
                                                 UsefulBufC *returned_payload)
{
    struct t_cose_encrypt_dec_ctx    decrypt_context;
    struct t_cose_recipient_dec_esdh dec_recipient;
    struct t_cose_parameter         *params;

    t_cose_encrypt_dec_init(&decrypt_context, 0); 
    t_cose_recipient_dec_esdh_init(&dec_recipient);
    t_cose_recipient_dec_esdh_set_key(&dec_recipient, mechanism->key.cose_key, NULL_Q_USEFUL_BUF_C);
    t_cose_encrypt_dec_add_recipient(&decrypt_context,
                                     (struct t_cose_recipient_dec *)&dec_recipient);

    return t_cose_encrypt_dec_detached(&decrypt_context,
                                        encryption_info,
                                        NULL_Q_USEFUL_BUF_C, /* in/unused: AAD */
					encrypted_payload,
                                        working_buf,
                                        returned_payload,
                                        &params);
}

enum t_cose_err_t suit_decrypt_cose_encrypt_aes(const UsefulBufC encrypted_payload,
                                                const UsefulBufC encryption_info,
                                                UsefulBuf working_buf,
                                                const suit_mechanism_t *mechanism,
                                                UsefulBufC *returned_payload)
{
    struct t_cose_recipient_dec_keywrap kw_unwrap_recipient;
    struct t_cose_encrypt_dec_ctx decrypt_context;

    t_cose_encrypt_dec_init(&decrypt_context, T_COSE_OPT_MESSAGE_TYPE_ENCRYPT);
    t_cose_recipient_dec_keywrap_init(&kw_unwrap_recipient);
    t_cose_recipient_dec_keywrap_set_kek(&kw_unwrap_recipient, mechanism->key.cose_key, NULL_Q_USEFUL_BUF_C);
    t_cose_encrypt_dec_add_recipient(&decrypt_context, (struct t_cose_recipient_dec *)&kw_unwrap_recipient);
    return t_cose_encrypt_dec_detached(&decrypt_context, encryption_info, NULL_Q_USEFUL_BUF_C, encrypted_payload, working_buf, returned_payload, NULL);
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

    enum t_cose_err_t t_cose_err;
    if (mechanism->key.cose_algorithm_id == T_COSE_ALGORITHM_A128KW) {
         /* AES-KW */
         t_cose_err = suit_decrypt_cose_encrypt_aes(encrypted_payload, encryption_info, working_buf, mechanism, returned_payload);
    }
    else if (mechanism->key.cose_algorithm_id == T_COSE_ALGORITHM_ECDH_ES_A128KW) {
         /* ES-DH */
         t_cose_err = suit_decrypt_cose_encrypt_esdh(encrypted_payload, encryption_info, working_buf, mechanism, returned_payload);
    }
    else {
        return SUIT_ERR_NOT_IMPLEMENTED;
    }

    if (t_cose_err != T_COSE_SUCCESS) {
        return SUIT_ERR_FAILED_TO_DECRYPT;
    }
    return SUIT_SUCCESS;
}

enum t_cose_err_t suit_encrypt_cose_encrypt_esdh(const UsefulBufC plaintext_payload,
                                                 const suit_mechanism_t *mechanism,
                                                 UsefulBuf encrypted_payload_buf,
                                                 UsefulBuf encryption_info_buf,
                                                 UsefulBufC *encrypted_payload,
                                                 UsefulBufC *encryption_info)
{
    struct t_cose_encrypt_enc        encrypt_context;
    struct t_cose_recipient_enc_esdh recipient;

    t_cose_encrypt_enc_init(&encrypt_context,
                             T_COSE_OPT_MESSAGE_TYPE_ENCRYPT,
                             T_COSE_ALGORITHM_A128GCM);
    t_cose_recipient_enc_esdh_init(&recipient,
                                    T_COSE_ALGORITHM_ECDH_ES_A128KW, /* content key distribution id */
                                    T_COSE_ELLIPTIC_CURVE_P_256);    /* curve id */
    t_cose_recipient_enc_esdh_set_key(&recipient,
                                       mechanism->rkey.cose_key,
                                       mechanism->rkid);
    t_cose_encrypt_add_recipient(&encrypt_context,
                                 (struct t_cose_recipient_enc *)&recipient);

    /* Now do the actual encryption */
    return t_cose_encrypt_enc_detached(&encrypt_context, /* in: encryption context */
                               plaintext_payload, /* in: payload to encrypt */
                               NULL_Q_USEFUL_BUF_C, /* in/unused: AAD */
			       encrypted_payload_buf, /* in: buffer for encrypted binary */
                               encryption_info_buf, /* in: buffer for COSE_Encrypt */
			       encrypted_payload, /* out: encrypted binary */
                               encryption_info); /* out: COSE_Encrypt */
}

enum t_cose_err_t suit_encrypt_cose_encrypt_aeskw(const UsefulBufC plaintext_payload,
                                                  const suit_mechanism_t *mechanism,
                                                  UsefulBuf encrypted_payload_buf,
                                                  UsefulBuf encryption_info_buf,
                                                  UsefulBufC *encrypted_payload,
                                                  UsefulBufC *encryption_info)
{
    struct t_cose_recipient_enc_keywrap kw_recipient;
    struct t_cose_encrypt_enc encrypt_context;

    t_cose_recipient_enc_keywrap_init(&kw_recipient, mechanism->key.cose_algorithm_id);
    t_cose_recipient_enc_keywrap_set_key(&kw_recipient, mechanism->key.cose_key, mechanism->kid);

    t_cose_encrypt_enc_init(&encrypt_context, T_COSE_OPT_MESSAGE_TYPE_ENCRYPT, T_COSE_ALGORITHM_A128GCM);
    t_cose_encrypt_add_recipient(&encrypt_context, (struct t_cose_recipient_enc *)&kw_recipient);

    return t_cose_encrypt_enc_detached(&encrypt_context,
                                        plaintext_payload,
                                        NULL_Q_USEFUL_BUF_C,
                                        encrypted_payload_buf,
                                        encryption_info_buf,
                                        encrypted_payload,
                                        encryption_info);
}

suit_err_t suit_encrypt_cose_encrypt(const UsefulBufC plaintext_payload,
                                     const suit_mechanism_t *mechanism,
                                     UsefulBuf encrypted_payload_buf,
                                     UsefulBuf encryption_info_buf,
                                     UsefulBufC *encrypted_payload,
                                     UsefulBufC *encryption_info)
{
    if (!mechanism->use ||
        (mechanism->cose_tag != CBOR_TAG_COSE_ENCRYPT0 &&
         mechanism->cose_tag != CBOR_TAG_COSE_ENCRYPT)) {
        return SUIT_ERR_FAILED_TO_ENCRYPT;
    }

    enum t_cose_err_t t_cose_err;
    if (mechanism->key.cose_algorithm_id == T_COSE_ALGORITHM_A128KW) {
        /* AES-KW */
        t_cose_err = suit_encrypt_cose_encrypt_aeskw(plaintext_payload, mechanism, encrypted_payload_buf, encryption_info_buf, encrypted_payload, encryption_info);
    }
    else if (mechanism->key.cose_algorithm_id == T_COSE_ALGORITHM_ECDH_ES_A128KW) {
        /* ES-DH */
	t_cose_err = suit_encrypt_cose_encrypt_esdh(plaintext_payload, mechanism, encrypted_payload_buf, encryption_info_buf, encrypted_payload, encryption_info);
    }
    else {
	return SUIT_ERR_NOT_IMPLEMENTED;
    }
    if (t_cose_err != T_COSE_SUCCESS) {
        return SUIT_ERR_FAILED_TO_ENCRYPT;
    }
    return SUIT_SUCCESS;
}
#endif /* LIBCSUIT_DISABLE_ENCRYPTION */

#if defined(LIBCSUIT_PSA_CRYPTO_C)
suit_err_t suit_create_es_key(suit_key_t *key)
{
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_handle_t     key_handle = 0;
    psa_status_t         result;

    int nid;
    int hash;
    switch (key->cose_algorithm_id) {
    case T_COSE_ALGORITHM_ES256:
    case T_COSE_ALGORITHM_ECDH_ES_A128KW:
        nid = PSA_ECC_FAMILY_SECP_R1;
        hash = PSA_ALG_SHA_256;
        break;
    case T_COSE_ALGORITHM_ES384:
        nid = PSA_ECC_FAMILY_SECP_R1;
        hash = PSA_ALG_SHA_384;
        break;
    case T_COSE_ALGORITHM_ES512:
        nid = PSA_ECC_FAMILY_SECP_R1;
        hash = PSA_ALG_SHA_512;
        break;
    default:
        return SUIT_ERR_INVALID_VALUE;
    }

    result = psa_crypto_init();

    if (result != PSA_SUCCESS) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }

    psa_key_usage_t usage = 0;
    if (key->cose_algorithm_id == T_COSE_ALGORITHM_ECDH_ES_A128KW) {
        /* for COSE_Encrypt0 or COSE_Encrypt */
	usage = PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_COPY;
        psa_set_key_usage_flags(&key_attributes, usage);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDH);
    }
    else {
        /* for COSE_Sign1 or COSE_Sign */
        usage = PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_EXPORT;
        if (key->private_key != NULL) {
            usage |= PSA_KEY_USAGE_SIGN_HASH;
        }
        psa_set_key_usage_flags(&key_attributes, usage);
#if defined(LIBCSUIT_USE_DETERMINISTIC_ECDSA)
        psa_set_key_algorithm(&key_attributes, PSA_ALG_DETERMINISTIC_ECDSA(hash));
#else
        psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(hash));
#endif

    }

    if (key->private_key == NULL) {
        psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(nid));
        result = psa_import_key(&key_attributes,
                                (const unsigned char*)key->public_key,
                                key->public_key_len,
                                &key_handle);
    }
    else {
        psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(nid));
        result = psa_import_key(&key_attributes,
                                (const unsigned char*)key->private_key,
                                key->private_key_len,
                                &key_handle);
    }
    if (result != PSA_SUCCESS) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }

#if defined(LIBCSUIT_USE_T_COSE_1)
    key->cose_key.k.key_handle           = key_handle;
#else
    key->cose_key.key.handle    = key_handle;
#endif

    return SUIT_SUCCESS;
}

#else /* LIBCSUIT_PSA_CRYPTO_C */
#if OPENSSL_VERSION_NUMBER >= OPENSSL_VERSION_300
/*
    \brief      Internal function calls OpenSSL functions to create public key.

    \param[in]  nid                 EC network id.
    \param[in]  public_key          Pointer of char array type of public key.
    \param[out] cose_public_key     Pointer and length of the resulting key.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.
 */
suit_err_t suit_create_es_key(suit_key_t *key)
{
    suit_err_t      result = SUIT_SUCCESS;
    EVP_PKEY        *pkey = NULL;
    EVP_PKEY_CTX    *ctx = NULL;
    OSSL_PARAM_BLD  *param_bld = NULL;
    OSSL_PARAM      *params = NULL;
    BIGNUM          *priv = NULL;

    const char *group_name;
    switch (key->cose_algorithm_id) {
    case T_COSE_ALGORITHM_ES256:
    case T_COSE_ALGORITHM_ECDH_ES_A128KW:
        group_name = "prime256v1";
        break;
    case T_COSE_ALGORITHM_ES384:
        group_name = "secp384r1";
        break;
    case T_COSE_ALGORITHM_ES512:
        group_name = "secp521r1";
        break;
    default:
        return SUIT_ERR_INVALID_VALUE;
    }

    param_bld = OSSL_PARAM_BLD_new();
    if (param_bld == NULL) {
        return SUIT_ERR_FATAL;
    }
    if (!OSSL_PARAM_BLD_push_utf8_string(param_bld, "group", group_name, 0)
        || !OSSL_PARAM_BLD_push_octet_string(param_bld, "pub", key->public_key, key->public_key_len)) {
        result = SUIT_ERR_FATAL;
        goto out;
    }
    if (key->private_key != NULL) {
        priv = BN_bin2bn(key->private_key, key->private_key_len, NULL);
        if (priv == NULL) {
            result = SUIT_ERR_FATAL;
            goto out;
        }
        if (!OSSL_PARAM_BLD_push_BN(param_bld, "priv", priv)) {
            result = SUIT_ERR_FATAL;
            goto out;
        }
    }
    params = OSSL_PARAM_BLD_to_param(param_bld);

    if (params == NULL) {
        result = SUIT_ERR_FATAL;
        goto out;
    }
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (ctx == NULL) {
        result = SUIT_ERR_FATAL;
        goto out;
    }
    if (ctx == NULL
        || EVP_PKEY_fromdata_init(ctx) <= 0
        || EVP_PKEY_fromdata(ctx, &pkey, EVP_PKEY_KEYPAIR, params) <= 0) {
        result = SUIT_ERR_FATAL;
        goto out;
    }

    key->cose_key.key.ptr  = pkey;
    return SUIT_SUCCESS;

out:
    EVP_PKEY_CTX_free(ctx);
    OSSL_PARAM_free(params);
    BN_free(priv);
    OSSL_PARAM_BLD_free(param_bld);
    return result;
}
#else /* OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_300 */
suit_err_t suit_create_es_key(suit_key_t *key)
{
    /* ****************************************** */
    /* cose algorithm enum -> openssl group name  */
    /* ****************************************** */
    const char *group_name;
    switch (key->cose_algorithm_id) {
    case T_COSE_ALGORITHM_ES256:
    case T_COSE_ALGORITHM_ECDH_ES_A128KW:
        group_name = "prime256v1";
        break;
    case T_COSE_ALGORITHM_ES384:
        group_name = "secp384r1";
        break;
    case T_COSE_ALGORITHM_ES512:
        group_name = "secp521r1";
        break;
    default:
        return SUIT_ERR_INVALID_VALUE;
    }

    /* ********************************* */
    /* create EC_KEY based on group name */
    /* ********************************* */
    int curveID = OBJ_txt2nid(group_name);
    EC_KEY *pEC = EC_KEY_new_by_curve_name(curveID);
    if (!pEC) {
        return SUIT_ERR_FATAL;
    }

    /* ****************************************************************** */
    /* set a public key raw data and a private key raw data into EC_KEY   */
    /* ****************************************************************** */
    if(!EC_KEY_oct2key(pEC,(unsigned char*) key->public_key, key->public_key_len, NULL)) {
        goto err;
    }
    if (key->private_key != NULL) {
        if(!EC_KEY_oct2priv(pEC,(unsigned char*) key->private_key, key->private_key_len)) {
            goto err;
        }
    }

    /* ************************* */
    /* validity check of EC_KEY  */
    /* ************************* */
    if (!EC_KEY_check_key(pEC)){
        goto err;
    }

    /* *************************************** */
    /* EC_KEY -> EVP_PKEY and set out variable */
    /* *************************************** */
    EVP_PKEY *pkey = EVP_PKEY_new();
    if (!EVP_PKEY_set1_EC_KEY(pkey, pEC)) {
        goto err;
    } else {
        key->cose_key.key.ptr   = pkey;
        EC_KEY_free(pEC);
        return SUIT_SUCCESS;
    }
err:
    EC_KEY_free(pEC);
    return SUIT_ERR_FATAL;
}
#endif /* OPENSSL_VERSION_NUMBER */
#endif /* LIBCSUIT_PSA_CRYPTO_C */

suit_err_t suit_key_init_ecdh_p256_key_pair(const unsigned char *private_key,
                                            const unsigned char *public_key,
                                            suit_key_t *cose_key_pair)
{
    cose_key_pair->private_key = private_key;
    cose_key_pair->private_key_len = PRIME256V1_PRIVATE_KEY_LENGTH;
    cose_key_pair->public_key = public_key;
    cose_key_pair->public_key_len = PRIME256V1_PUBLIC_KEY_LENGTH;
    cose_key_pair->cose_algorithm_id = T_COSE_ALGORITHM_ECDH_ES_A128KW;
    return suit_create_es_key(cose_key_pair);
}

suit_err_t suit_key_init_es256_key_pair(const unsigned char *private_key,
                                        const unsigned char *public_key,
                                        suit_key_t *cose_key_pair)
{
    cose_key_pair->private_key = private_key;
    cose_key_pair->private_key_len = PRIME256V1_PRIVATE_KEY_LENGTH;
    cose_key_pair->public_key = public_key;
    cose_key_pair->public_key_len = PRIME256V1_PUBLIC_KEY_LENGTH;
    cose_key_pair->cose_algorithm_id = T_COSE_ALGORITHM_ES256;
    return suit_create_es_key(cose_key_pair);
}

suit_err_t suit_key_init_es384_key_pair(const unsigned char *private_key,
                                        const unsigned char *public_key,
                                        suit_key_t *cose_key_pair)
{
    cose_key_pair->private_key = private_key;
    cose_key_pair->private_key_len = SECP384R1_PRIVATE_KEY_LENGTH;
    cose_key_pair->public_key = public_key;
    cose_key_pair->public_key_len = SECP384R1_PUBLIC_KEY_LENGTH;
    cose_key_pair->cose_algorithm_id = T_COSE_ALGORITHM_ES384;
    return suit_create_es_key(cose_key_pair);
}

suit_err_t suit_key_init_es521_key_pair(const unsigned char *private_key,
                                        const unsigned char *public_key,
                                        suit_key_t *cose_key_pair)
{
    cose_key_pair->private_key = private_key;
    cose_key_pair->private_key_len = SECP521R1_PRIVATE_KEY_LENGTH;
    cose_key_pair->public_key = public_key;
    cose_key_pair->public_key_len = SECP521R1_PUBLIC_KEY_LENGTH;
    cose_key_pair->cose_algorithm_id = T_COSE_ALGORITHM_ES512;
    return suit_create_es_key(cose_key_pair);
}

suit_err_t suit_key_init_ecdh_p256_public_key(const unsigned char *public_key,
                                              suit_key_t *cose_public_key)
{
    cose_public_key->private_key = NULL;
    cose_public_key->private_key_len = 0;
    cose_public_key->public_key = public_key;
    cose_public_key->public_key_len = PRIME256V1_PUBLIC_KEY_LENGTH;
    cose_public_key->cose_algorithm_id = T_COSE_ALGORITHM_ECDH_ES_A128KW;
    return suit_create_es_key(cose_public_key);
}


suit_err_t suit_key_init_es256_public_key(const unsigned char *public_key,
                                          suit_key_t *cose_public_key)
{
    cose_public_key->private_key = NULL;
    cose_public_key->private_key_len = 0;
    cose_public_key->public_key = public_key;
    cose_public_key->public_key_len = PRIME256V1_PUBLIC_KEY_LENGTH;
    cose_public_key->cose_algorithm_id = T_COSE_ALGORITHM_ES256;
    return suit_create_es_key(cose_public_key);
}

suit_err_t suit_key_init_es384_public_key(const unsigned char *public_key,
                                          suit_key_t *cose_public_key)
{
    cose_public_key->private_key = NULL;
    cose_public_key->private_key_len = 0;
    cose_public_key->public_key = public_key;
    cose_public_key->public_key_len = SECP384R1_PUBLIC_KEY_LENGTH;
    cose_public_key->cose_algorithm_id = T_COSE_ALGORITHM_ES384;
    return suit_create_es_key(cose_public_key);
}

suit_err_t suit_key_init_es521_public_key(const unsigned char *public_key,
                                          suit_key_t *cose_public_key)
{
    cose_public_key->private_key = NULL;
    cose_public_key->private_key_len = 0;
    cose_public_key->public_key = public_key;
    cose_public_key->public_key_len = SECP521R1_PUBLIC_KEY_LENGTH;
    cose_public_key->cose_algorithm_id = T_COSE_ALGORITHM_ES512;
    return suit_create_es_key(cose_public_key);
}

#ifndef LIBCSUIT_DISABLE_ENCRYPTION
suit_err_t suit_create_aes_key(suit_key_t *cose_secret_key)
{
    UsefulBufC symmetric_key = (UsefulBufC) {.ptr = cose_secret_key->private_key, .len = cose_secret_key->private_key_len};
    enum t_cose_err_t err = t_cose_key_init_symmetric(cose_secret_key->cose_algorithm_id, symmetric_key, &cose_secret_key->cose_key);
    if (err != T_COSE_SUCCESS) {
        return SUIT_ERR_FATAL;
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_key_init_a128kw_secret_key(const unsigned char *secret_key,
                                           suit_key_t *cose_secret_key)
{
    cose_secret_key->private_key = secret_key;
    cose_secret_key->private_key_len = A128GCM_KEY_CHAR_LENGTH;
    cose_secret_key->public_key = NULL;
    cose_secret_key->public_key_len = 0;
    cose_secret_key->cose_algorithm_id = T_COSE_ALGORITHM_A128KW;
    return suit_create_aes_key(cose_secret_key);
}
#endif /* LIBCSUIT_DISABLE_ENCRYPTION */

suit_err_t suit_free_key(const suit_key_t *key)
{
#if defined(LIBCSUIT_PSA_CRYPTO_C)
#if defined(LIBCSUIT_USE_T_COSE_1)
    psa_destroy_key((psa_key_handle_t)key->cose_key.k.key_handle);
#else
    psa_destroy_key((psa_key_handle_t)key->cose_key.key.handle);
#endif /* LIBCSUIT_USE_T_COSE_1 */
#else /* LIBCSUIT_PSA_CRYPTO_C */
    EVP_PKEY_free(key->cose_key.key.ptr);
#endif
    return SUIT_SUCCESS;
}

suit_err_t suit_set_suit_key_from_cose_key_from_item(QCBORDecodeContext *context,
                                                     QCBORItem *item,
                                                     suit_key_t *suit_key)
{
    suit_err_t result;
    QCBORError error;
    if (item->uDataType != QCBOR_TYPE_MAP) {
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    UsefulBuf_MAKE_STACK_UB(public_key, SUIT_MAX_PUBLIC_KEY_LEN);

    int64_t crv = 0;
    int64_t kty = 0;
    union {
        UsefulBufC k; // k for Symmetric
        struct {
            UsefulBufC x; // x for EC2
            UsefulBufC y; // y for EC2
            UsefulBufC d; // d for EC2
        };
    } key_params = {0};
    QCBORDecode_EnterMap(context, item);
    /* allows only deterministic encoded COSE_Key */
    const size_t cose_key_map_len = item->val.uCount;
    for (size_t i = 0; i < cose_key_map_len; i++) {
        result = suit_qcbor_get(context, item, true, QCBOR_TYPE_ANY);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        if (item->uLabelType != QCBOR_TYPE_INT64) {
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        switch (item->label.int64) {
        case SUIT_COSE_KTY:
            if (item->uDataType != QCBOR_TYPE_INT64) {
                return SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            switch (item->val.int64) {
            case SUIT_COSE_KTY_EC2:
            case SUIT_COSE_KTY_SYMMETRIC:
                kty = item->val.int64;
                break;
            default:
                return SUIT_ERR_NOT_IMPLEMENTED;
            }
            break;
        case -1:
            if (kty == SUIT_COSE_KTY_EC2) {
                if (item->uDataType != QCBOR_TYPE_INT64) {
                    return SUIT_ERR_INVALID_TYPE_OF_VALUE;
                }
                crv = item->val.int64;
            }
            else if (kty == SUIT_COSE_KTY_SYMMETRIC) {
                if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                    return SUIT_ERR_INVALID_TYPE_OF_VALUE;
                }
                key_params.k = item->val.string;
            }
            else {
                return SUIT_ERR_NOT_IMPLEMENTED;
            }
            break;
        case -2:
            if (kty == SUIT_COSE_KTY_EC2) {
                if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                    return SUIT_ERR_INVALID_TYPE_OF_VALUE;
                }
                key_params.x = item->val.string;
            }
            else {
                return SUIT_ERR_NOT_IMPLEMENTED;
            }
            break;
        case -3:
            if (kty == SUIT_COSE_KTY_EC2) {
                if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                    return SUIT_ERR_INVALID_TYPE_OF_VALUE;
                }
                key_params.y = item->val.string;
            }
            else {
                return SUIT_ERR_NOT_IMPLEMENTED;
            }
            break;
        case -4:
            if (kty == SUIT_COSE_KTY_EC2) {
                if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                    return SUIT_ERR_INVALID_TYPE_OF_VALUE;
                }
                key_params.d = item->val.string;
            }
            else {
                return SUIT_ERR_NOT_IMPLEMENTED;
            }
            break;
        default:
            suit_qcbor_skip_any(context, item);
            break;
        }
    }
    QCBORDecode_ExitMap(context);
    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }

    /* check kty */
    switch (kty) {
    case SUIT_COSE_KTY_EC2:
        switch (crv) {
        case SUIT_COSE_CRV_P256:
            if ((key_params.x.len == PRIME256V1_PUBLIC_KEY_X_LENGTH) &&
                (key_params.y.len == PRIME256V1_PUBLIC_KEY_Y_LENGTH)) {
                /* POINT_CONVERSION_UNCOMPRESSED */
                ((uint8_t *)public_key.ptr)[0] = 0x04;
                memcpy(&((uint8_t *)public_key.ptr)[1], key_params.x.ptr, PRIME256V1_PUBLIC_KEY_X_LENGTH);
                memcpy(&((uint8_t *)public_key.ptr)[1 + PRIME256V1_PUBLIC_KEY_X_LENGTH], key_params.y.ptr, PRIME256V1_PUBLIC_KEY_Y_LENGTH);
                public_key.len = PRIME256V1_PUBLIC_KEY_LENGTH;
            }
            else {
                return SUIT_ERR_INVALID_VALUE;
            }
            if (key_params.d.len == PRIME256V1_PRIVATE_KEY_LENGTH) {
                // TODO: can we limit EC P-256 for ES256?
		if (suit_key->cose_algorithm_id == T_COSE_ALGORITHM_ECDH_ES_A128KW) {
		    result = suit_key_init_ecdh_p256_key_pair(key_params.d.ptr, public_key.ptr, suit_key);
		}
		else if (suit_key->cose_algorithm_id == T_COSE_ALGORITHM_ES256) {
                    result = suit_key_init_es256_key_pair(key_params.d.ptr, public_key.ptr, suit_key);
		}
		else {
	            result = SUIT_ERR_NOT_IMPLEMENTED;
		}
            }
            else if (key_params.d.len == 0) {
                // TODO: can we limit EC P-256 for ES256?
	        if (suit_key->cose_algorithm_id == T_COSE_ALGORITHM_ECDH_ES_A128KW) {
		    result = suit_key_init_ecdh_p256_public_key(public_key.ptr, suit_key);
		}
		else if (suit_key->cose_algorithm_id == T_COSE_ALGORITHM_ES256) {
                    result = suit_key_init_es256_public_key(public_key.ptr, suit_key);
		}
		else {
		    result = SUIT_ERR_NOT_IMPLEMENTED;
		}
            }
            else {
                return SUIT_ERR_INVALID_VALUE;
            }
            if (result != SUIT_SUCCESS) {
                return result;
            }
            break; /* COSE_KEY_CRV_P256 */

        default:
            return SUIT_ERR_NOT_IMPLEMENTED;
        }
        break; /* SUIT_COSE_KTY_EC2 */

#if !defined(LIBCSUIT_DISABLE_ENCRYPTION)
    case SUIT_COSE_KTY_SYMMETRIC:
        if (key_params.k.len == 16) {
            result = suit_key_init_a128kw_secret_key(key_params.k.ptr, suit_key);
        }
        break; /* SUIT_COSE_KTY_SYMMETRIC */
#endif /* LIBCSUIT_DISABLE_ENCRYPTION */

    default:
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    return result;
}

suit_err_t suit_set_suit_key_from_cose_key(UsefulBufC cose_key,
                                           suit_key_t *suit_key)
{
    QCBORDecodeContext decode_context;
    QCBORItem item;
    QCBORDecode_Init(&decode_context, cose_key, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_PeekNext(&decode_context, &item);
    suit_err_t result = suit_set_suit_key_from_cose_key_from_item(&decode_context, &item, suit_key);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBORError error = QCBORDecode_Finish(&decode_context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_set_suit_key_from_cwt_payload(UsefulBufC cwt_payload,
                                              suit_key_t *suit_key)
{
    suit_err_t result;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error;
    QCBORDecode_Init(&context, cwt_payload, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);

    const size_t cbor_map_len = item.val.uCount;
    for (size_t i = 0; i < cbor_map_len; i++) {
        error = QCBORDecode_PeekNext(&context, &item);
        if (error != QCBOR_SUCCESS) {
            return suit_error_from_qcbor_error(error);
        }
        if (item.uLabelType != QCBOR_TYPE_INT64) {
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        switch (item.label.int64) {
        case SUIT_COSE_CNF:
            if (item.uDataType != QCBOR_TYPE_MAP) {
                return SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            QCBORDecode_EnterMap(&context, &item);
            const size_t cnf_map_len = item.val.uCount;
            for (size_t j = 0; j < cnf_map_len; j++) {
                error = QCBORDecode_PeekNext(&context, &item);
                if (error != QCBOR_SUCCESS) {
                    return suit_error_from_qcbor_error(error);
                }
                if (item.uLabelType != QCBOR_TYPE_INT64) {
                    return SUIT_ERR_INVALID_TYPE_OF_KEY;
                }

                switch (item.label.int64) {
                case SUIT_COSE_COSE_KEY:
                    suit_key->cose_algorithm_id = T_COSE_ALGORITHM_ES256;
                    result = suit_set_suit_key_from_cose_key_from_item(&context, &item, suit_key);
                    if (result != SUIT_SUCCESS) {
                        return result;
                    }
                    break;

                default:
                    return SUIT_ERR_NOT_IMPLEMENTED;
                }
            }
            QCBORDecode_ExitMap(&context);
            error = QCBORDecode_GetError(&context);
            if (error != QCBOR_SUCCESS) {
                return suit_error_from_qcbor_error(error);
            }
            break; /* SUIT_COSE_CNF */

        default:
            return SUIT_ERR_NOT_IMPLEMENTED;
        }
    }
    QCBORDecode_ExitMap(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS && result == SUIT_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }
    return result;
}
