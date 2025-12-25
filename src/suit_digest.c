/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#include "csuit/suit_common.h"
#include "csuit/suit_digest.h"
#include "csuit/suit_manifest_encode.h"

/*
    Public function. See suit_digest.h
 */
#if defined(LIBCSUIT_PSA_CRYPTO_C)
suit_err_t suit_generate_sha256(UsefulBufC tgt, UsefulBuf digest_bytes)
{
    psa_status_t status;
    size_t real_hash_size;
    psa_hash_operation_t sha256_psa = psa_hash_operation_init();

    status = psa_crypto_init( );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_setup( &sha256_psa, PSA_ALG_SHA_256 );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_update( &sha256_psa, tgt.ptr, tgt.len );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_finish( &sha256_psa, digest_bytes.ptr, digest_bytes.len, &real_hash_size );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    if(real_hash_size != SHA256_DIGEST_LENGTH)
        return( SUIT_ERR_FAILED_TO_VERIFY );

    return SUIT_SUCCESS;
}
#else
suit_err_t suit_generate_sha256(UsefulBufC tgt, UsefulBuf digest_bytes)
{
    suit_err_t result = SUIT_ERR_FAILED_TO_VERIFY;
    if (digest_bytes.len < SHA256_DIGEST_LENGTH) {
        return SUIT_ERR_NO_MEMORY;
    }
    unsigned int generated_size = 0;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (ctx != NULL
        && EVP_DigestInit(ctx, EVP_sha256())
        && EVP_DigestUpdate(ctx, tgt.ptr, tgt.len)
        && EVP_DigestFinal(ctx, digest_bytes.ptr, &generated_size)) {
        result = SUIT_SUCCESS;
    }
    if (ctx != NULL) {
        EVP_MD_CTX_free(ctx);
    }
    return result;
}
#endif /* LIBCSUIT_PSA_CRYPTO_C */

/*
    Public function. See suit_digest.h
 */
suit_err_t suit_verify_sha256(UsefulBufC tgt, UsefulBufC digest_bytes)
{
    if (digest_bytes.len != SHA256_DIGEST_LENGTH) {
        return SUIT_ERR_FATAL;
    }
    MakeUsefulBufOnStack(hash, SHA256_DIGEST_LENGTH);
    suit_err_t result = suit_generate_sha256(tgt, hash);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    return (memcmp(digest_bytes.ptr, hash.ptr, SHA256_DIGEST_LENGTH) == 0) ? SUIT_SUCCESS : SUIT_ERR_FAILED_TO_VERIFY;
}

suit_err_t suit_verify_digest(UsefulBufC buf,
                              const suit_digest_t *digest)
{
    suit_err_t result;

    switch (digest->algorithm_id) {
    case SUIT_ALGORITHM_ID_SHA256:
        result = suit_verify_sha256(buf, digest->bytes);
        break;
    case SUIT_ALGORITHM_ID_SHAKE128:
    case SUIT_ALGORITHM_ID_SHA384:
    case SUIT_ALGORITHM_ID_SHA512:
    case SUIT_ALGORITHM_ID_SHAKE256:
    default:
        result = SUIT_ERR_NOT_IMPLEMENTED;
    }
    return result;
}

suit_err_t suit_generate_digest(UsefulBufC buf,
                                UsefulBuf digest_buf,
                                suit_digest_t *digest)
{
    suit_err_t result;

    switch (digest->algorithm_id) {
    case SUIT_ALGORITHM_ID_SHA256:
        result = suit_generate_sha256(buf, digest_buf);
        if (result == SUIT_SUCCESS) {
            digest->bytes = UsefulBuf_Const(digest_buf);
        }
        break;
    case SUIT_ALGORITHM_ID_SHAKE128:
    case SUIT_ALGORITHM_ID_SHA384:
    case SUIT_ALGORITHM_ID_SHA512:
    case SUIT_ALGORITHM_ID_SHAKE256:
    default:
        result = SUIT_ERR_NOT_IMPLEMENTED;
    }
    return result;
}

suit_err_t suit_generate_digest_using_encode_buf(suit_encoder_context_t *encoder_context,
                                                 UsefulBufC buf,
                                                 suit_digest_t *digest)
{
    suit_err_t result = SUIT_SUCCESS;

    if (digest->algorithm_id != SUIT_ALGORITHM_ID_SHA256) {
        return SUIT_ERR_NOT_IMPLEMENTED;
    }

    UsefulBuf digest_buf;
    result = suit_use_suit_encode_buf(encoder_context, SHA256_DIGEST_LENGTH, &digest_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    result = suit_generate_digest(buf, digest_buf, digest);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    result = suit_fix_suit_encode_buf(encoder_context, SHA256_DIGEST_LENGTH);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    digest->bytes.ptr = digest_buf.ptr;
    digest->bytes.len = SHA256_DIGEST_LENGTH;
    return SUIT_SUCCESS;
}
