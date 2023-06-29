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
suit_err_t suit_generate_sha256(const uint8_t *tgt_ptr,
                                const size_t tgt_len,
                                uint8_t *digest_bytes_ptr,
                                const size_t digest_bytes_len)
{
    psa_status_t status;
    size_t real_hash_size;
    psa_hash_operation_t sha256_psa = PSA_HASH_OPERATION_INIT;

    status = psa_crypto_init( );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_setup( &sha256_psa, PSA_ALG_SHA_256 );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_update( &sha256_psa, tgt_ptr, tgt_len );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    status = psa_hash_finish( &sha256_psa, digest_bytes_ptr, digest_bytes_len, &real_hash_size );
    if( status != PSA_SUCCESS )
        return( SUIT_ERR_FAILED_TO_VERIFY );

    if(real_hash_size != SHA256_DIGEST_LENGTH)
        return( SUIT_ERR_FAILED_TO_VERIFY );

    return SUIT_SUCCESS;
}
#else
suit_err_t suit_generate_sha256(const uint8_t *tgt_ptr,
                                const size_t tgt_len,
                                uint8_t *digest_bytes_ptr,
                                const size_t digest_bytes_len)
{
    suit_err_t result = SUIT_ERR_FAILED_TO_VERIFY;
    if (digest_bytes_len > UINT32_MAX) {
        return SUIT_ERR_NO_MEMORY;
    }
    unsigned int generated_size = digest_bytes_len;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (ctx != NULL
        && EVP_DigestInit(ctx, EVP_sha256())
        && EVP_DigestUpdate(ctx, tgt_ptr, tgt_len)
        && EVP_DigestFinal(ctx, digest_bytes_ptr, &generated_size)
        && digest_bytes_len == generated_size) {
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
suit_err_t suit_verify_sha256(const uint8_t *tgt_ptr,
                              const size_t tgt_len,
                              const uint8_t *digest_bytes_ptr,
                              const size_t digest_bytes_len)
{
    if (digest_bytes_len != SHA256_DIGEST_LENGTH) {
        return SUIT_ERR_FATAL;
    }
    uint8_t hash[SHA256_DIGEST_WORK_SPACE_LENGTH];
    suit_err_t result = suit_generate_sha256(tgt_ptr, tgt_len, hash, SHA256_DIGEST_WORK_SPACE_LENGTH);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    return (memcmp(digest_bytes_ptr, hash, SHA256_DIGEST_LENGTH) == 0) ? SUIT_SUCCESS : SUIT_ERR_FAILED_TO_VERIFY;
}

suit_err_t suit_verify_digest(const suit_buf_t *buf,
                              const suit_digest_t *digest)
{
    suit_err_t result;

    switch (digest->algorithm_id) {
    case SUIT_ALGORITHM_ID_SHA256:
        result = suit_verify_sha256(buf->ptr, buf->len, digest->bytes.ptr, digest->bytes.len);
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

suit_err_t suit_generate_digest(const suit_buf_t buf,
                                const suit_buf_t digest_buf,
                                suit_digest_t *digest)
{
    suit_err_t result;

    switch (digest->algorithm_id) {
    case SUIT_ALGORITHM_ID_SHA256:
        result = suit_generate_sha256(buf.ptr, buf.len, digest_buf.ptr, digest_buf.len);
        if (result == SUIT_SUCCESS) {
            digest->bytes = digest_buf;
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

suit_err_t suit_generate_digest_using_encode_buf(const uint8_t *ptr,
                                                 const size_t len,
                                                 suit_encode_t *suit_encode,
                                                 suit_digest_t *digest)
{
    suit_err_t result = SUIT_SUCCESS;

    size_t work_space_length;
    size_t fixed_length;

    switch (digest->algorithm_id) {
    case SUIT_ALGORITHM_ID_SHA256:
        work_space_length = SHA256_DIGEST_WORK_SPACE_LENGTH;
        fixed_length = SHA256_DIGEST_LENGTH;
        break;
    default:
        return SUIT_ERR_NOT_IMPLEMENTED;
    }

    UsefulBuf digest_buf;
    result = suit_use_suit_encode_buf(suit_encode, work_space_length, &digest_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    suit_buf_t buf;
    buf.ptr = (uint8_t *)ptr;
    buf.len = len;
    suit_buf_t working_buf;
    working_buf.ptr = digest_buf.ptr;
    working_buf.len = digest_buf.len;
    result = suit_generate_digest(buf, working_buf, digest);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    /* given length are working memory size, so we must overwrite it into actual hash length */
    result = suit_fix_suit_encode_buf(suit_encode, fixed_length);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    digest->bytes.ptr = digest_buf.ptr;
    digest->bytes.len = fixed_length;
    return SUIT_SUCCESS;
}


