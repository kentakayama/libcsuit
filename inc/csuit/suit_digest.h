/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SUIT_DIGEST_H
#define SUIT_DIGEST_H

#include "suit_common.h"
struct suit_digest;

#define SHA256_DIGEST_LENGTH 32

/*!
    \file   suit_digest.h

    \brief  SHA-2 and SHA-3 functions to generate and verify the hash
 */

#ifdef __cplusplus
extern "C" {
#endif


/*!
    \brief      Generate SHA-256 digest

    \param[in]  tgt_ptr             Pointer of the payload to be taken hash.
    \param[in]  tgt_len             Length of the payload.
    \param[out] digest_bytes_ptr    Pointer of the buffer of generated digest to be placed.
    \param[in]  digest_bytes_len    Length of the digest buffer.

    This function generates SHA-256 hash to the digest_bytes_ptr.
 */
suit_err_t suit_generate_sha256(UsefulBufC tgt, UsefulBuf digest_bytes);



/*!
    \brief      Verify SHA-256 digest

    \param[in]  tgt_ptr             Pointer of the payload to be taken hash.
    \param[in]  tgt_len             Length of the payload.
    \param[out] digest_bytes_ptr    Pointer of the buffer of generated digest to be placed.
    \param[in]  digest_bytes_len    Length of the digest buffer.

    This verifies the hash of the payload, specified by tgt_ptr and tgt_len,
    matches the input digest, specified by digest_bytes_ptr and digest_bytes_len.
 */
suit_err_t suit_verify_sha256(UsefulBufC tgt, UsefulBufC digest_bytes);

/*!
    \brief      Verify the digest of UsefulBufC match

    \param[in]  buf                 Pointer and length of the payload.
    \param[in]  digest              \t suit_digest.

    \return     This returns \ref SUIT_SUCCESS or \ref SUIT_ERR_FAILED_TO_VERIFY.
 */
suit_err_t suit_verify_digest(UsefulBufC buf, const suit_digest_t *digest);

/*!
    \brief      Generate digest of suit_buf_t

    \param[in]  buf                 Pointer and length of the payload.
    \param[in]  working             Pointer and length of working buffer to calculate hash.
    \param[out] digest              \t suit_digest.

    \return     This returns SUIT_SUCCESS or SUIT_ERR_FAILED_TO_VERIFY.
 */
suit_err_t suit_generate_digest(UsefulBufC buf,
                                UsefulBuf digest_buf,
                                suit_digest_t *digest);

struct suit_encoder_context;
suit_err_t suit_generate_digest_using_encode_buf(struct suit_encoder_context *encoder_context,
                                                 UsefulBufC buf,
                                                 suit_digest_t *digest);

suit_err_t suit_decode_digest_from_item(QCBORDecodeContext *context,
                                        QCBORItem *item,
                                        bool next,
                                        suit_digest_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_DIGEST_H */
