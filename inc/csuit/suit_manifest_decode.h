/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_decode.h

    \brief  Declarations of structures and functions
 */

#ifndef SUIT_MANIFEST_DECODE_H
#define SUIT_MANIFEST_DECODE_H

#include "suit_common.h"
#include "suit_cose.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
    \brief  Decode SUIT binary.

    \param[in]  mode        This ontrols parsing behavior, e.g. #SUIT_DECODE_MODE_STRICT.
    \param[in]  buf         Pointer and length of input binary.
    \param[out] envelope    Pointer of output structure to hold the parsing result of SUIT binary.
    \param[in]  public_key  Pointer of public key to verify the COSE_Sign1 of authentication-wrapper.

    \return     This returns one of the error codes defined by \ref suit_err_t.
 */
suit_err_t suit_decode_envelope(suit_decode_mode_t mode,
                                suit_buf_t *buf,
                                suit_envelope_t *envelope,
                                suit_mechanism_t *mechanism);

/*!
    \brief  Decode array of SUIT_Component_Identifier.

    \param[in]  mode        Controls parsing behavior, e.g. #SUIT_DECODE_MODE_STRICT.
    \param[in]  buf         Pointer and length of input binary.
    \param[out] identifier  Pointers and length of resulting SUIT_Component_Identifiers.

    \return     This returns one of the error codes defined by \ref suit_err_t.
 */
suit_err_t suit_decode_component_identifiers(suit_decode_mode_t mode,
                                             suit_buf_t *buf,
                                             suit_component_identifier_t *identifier);

/*!
    \brief  Decode bstr-wrapped command sequence.

    \param[in]  mode        Controls parsing behavior, e.g. #SUIT_DECODE_MODE_STRICT.
    \param[in]  buf         Pointer and length of input binary.
    \param[out] identifier  Pointers and length of resulting SUIT_Command_Sequence.

    \return     This returns one of the error codes defined by \ref suit_err_t.
 */
suit_err_t suit_decode_command_sequence(suit_decode_mode_t mode,
                                        const suit_buf_t *buf,
                                        suit_command_sequence_t *cmd_seq);

suit_err_t suit_decode_dependencies_from_item(suit_decode_mode_t mode,
                                              QCBORDecodeContext *context,
                                              QCBORItem *item,
                                              bool next,
                                              suit_dependencies_t *dependencies);

suit_err_t suit_decode_version_match_from_item(QCBORDecodeContext *context,
                                               QCBORItem *item,
                                               bool next,
                                               suit_version_match_t *version_match);
suit_err_t suit_decode_version_match(const suit_buf_t *buf,
                                     suit_version_match_t *version_match);

suit_err_t suit_decode_wait_event_from_item(QCBORDecodeContext *context,
                                            QCBORItem *item,
                                            bool next,
                                            suit_wait_event_t *wait_event);
suit_err_t suit_decode_wait_event(const suit_buf_t *buf,
                                  suit_wait_event_t *wait_event);

suit_err_t suit_decode_component_metadata(const suit_buf_t *buf,
                                          suit_component_metadata_t *component_metadata);
suit_err_t suit_decode_component_metadata_from_item(QCBORDecodeContext *context,
                                                    QCBORItem *item,
                                                    bool next,
                                                    suit_component_metadata_t *component_metadata);

#ifdef __cplusplus
}
#endif

#endif  // SUIT_MANIFEST_DECODE_H
