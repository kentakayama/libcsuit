/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_encode.h

    \brief  Declarations of structures and functions
 */

#ifndef SUIT_MANIFEST_ENCODE_H
#define SUIT_MANIFEST_ENCODE_H

#include "suit_common.h"
#include "suit_cose.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct suit_encoder_context {
    UsefulBufC manifest;
    // SUIT_SeverableMembers
    UsefulBufC dependency_resolution;
    suit_digest_t dependency_resolution_digest;
    UsefulBufC payload_fetch;
    suit_digest_t payload_fetch_digest;
    UsefulBufC candidate_verification;
    suit_digest_t candidate_verification_digest;
    UsefulBufC install;
    suit_digest_t install_digest;
    UsefulBufC text;
    suit_digest_t text_digest;
    UsefulBufC coswid;
    suit_digest_t coswid_digest;

    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];

    size_t pos;
    size_t cur_pos;
    size_t max_pos;
    uint8_t buf[];
} suit_encoder_context_t;

suit_err_t suit_use_suit_encode_buf(suit_encoder_context_t *encoder_context,
                                    size_t len,
                                    UsefulBuf *buf);

suit_err_t suit_fix_suit_encode_buf(suit_encoder_context_t *encoder_context,
                                    const size_t used_len);

suit_err_t suit_encode_append_version_match(const suit_version_match_t *version_match,
                                            const uint32_t label,
                                            QCBOREncodeContext *context);
suit_err_t suit_encode_append_digest(const suit_digest_t *digest,
                                     const uint32_t label,
                                     QCBOREncodeContext *context);

/*!
    \brief  Encode SUIT binary

    \param[in]      encoder_context     Encoding context initialized with \ref suit_encode_init
    \param[in]      envelope            Input struct of libcsuit, correspond to the SUIT_Envelope.
    \param[out]     encoded_manifest    Output manifest.

    \return     This returns one of the error codes defined by \ref suit_err_t.

    Encoding SUIT_Envelope takes several steps.
    1st. Generate SUIT_Digest of severed members
    2nd. Generate SUIT_Digest of suit-manifest
    3rd. Respectively append suit-authentication-wrapper, suit-manifest, ...

    This is the "map" of the encoding proccess.
    \code{.unparsed}
    SUIT_Envelope { // <= You are here!
        suit-authentication-wrapper,
        suit-manifest {
            suit-common,
            suit-install,
            suit-validate,
            ...
        }

        // severed member
        suit-install,
        suit-validate,
        ...
    }
    \endcode
 */
suit_err_t suit_encode_envelope(suit_encoder_context_t *encoder_context,
                                const suit_envelope_t *envelope,
                                UsefulBufC *encoded_manifest);

/*!
    \brief      Assigns a recipient key to the SUIT Manifest encoder

    \param[in]  encoder_context  Encoding context initialized with \ref suit_encode_init
    \param[in]  cose_tag            The tag, for example, 18 (COSE_Sign1), 17 (COSE_Mac0) or 96 (COSE_Encrypt).
    \param[in]  cose_algorithm_id   The COSE Algorithm identifier, such as -7 (ES256).
    \param[in]  sender_key          COSE_Key used as a sender key.

    \return     This returns one of the error codes defined by \ref suit_err_t.
 */
suit_err_t suit_encode_add_sender_key(
    suit_encoder_context_t *encoder_context,
    const int cose_tag,
    int cose_algorithm_id,
    suit_key_t *sender_key);

/*!
    \brief      Initializes the SUIT Manifest encoder

    \param[in]  encoder_context     Pointer to the SUIT Manifest encoder itself.
    \param[in]  buf_size            Allocated buffer size for the SUIT Manifest encoder while encoding.

    Call this function first to encode a SUIT Manifest.
    The size of encoder_context must be sizeof(suit_encoder_context_t) + buf_size.
    Refer \ref suit_encoder_context_t .
 */
suit_err_t suit_encode_init(suit_encoder_context_t *encoder_context, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_MANIFEST_ENCODE_H */