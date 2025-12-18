/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
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

suit_err_t suit_use_suit_encode_buf(suit_encode_t *suit_encode,
                                    size_t len,
                                    UsefulBuf *buf);

suit_err_t suit_fix_suit_encode_buf(suit_encode_t *suit_encode,
                                    const size_t used_len);

suit_err_t suit_encode_append_component_identifier(const suit_component_identifier_t *component_id,
                                                   uint32_t label,
                                                   QCBOREncodeContext *context);
suit_err_t suit_encode_append_version_match(const suit_version_match_t *version_match,
                                            const uint32_t label,
                                            QCBOREncodeContext *context);
suit_err_t suit_encode_append_digest(const suit_digest_t *digest,
                                     const uint32_t label,
                                     QCBOREncodeContext *context);

/*!
    \brief  Encode SUIT binary

    \param[in]      mode        Controls parsing behavior, e.g. #SUIT_DECODE_MODE_STRICT.
    \param[in]      envelope    Input struct of libcsuit, correspond to the SUIT_Envelope.
    \param[in]      signing_key The private key (or key pair) to generate COSE_Sign1 signature.
    \param[out]     buf         The pointer of output buffer of the binary.
    \param[in,out]  len         Length of the allocated buf size in input,
                                and the size of the generated binary size in output.

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
suit_err_t suit_encode_envelope(suit_decode_mode_t mode,
                                const suit_envelope_t *envelope,
                                const suit_mechanism_t *mechanism,
                                uint8_t **buf,
                                size_t *len);

// void suit_encode_append_parameter(
//         QCBOREncodeContext *cbor_encoder,
//         suit_err_t result,
//         suit_rep_policy_t report_policy,
//         suit_con_dir_key_t condition_directive_key,
//         uint8_t index,
//         suit_parameter_args_t parameters[]);

// void suit_encode_append_suit_record(
//         QCBOREncodeContext *cbor_encoder,
//         uint32_t label,
//         suit_manifest_key_t manifest_key,
//         size_t section_offset,
//         uint64_t component_index);

// void suit_encode_append_suit_report_result(
//         QCBOREncodeContext *cbor_encoder,
//         suit_err_t result,
//         suit_manifest_key_t manifest_key,
//         size_t section_offset,
//         uint64_t component_index);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_MANIFEST_ENCODE_H */