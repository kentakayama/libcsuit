/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_digest.h"

suit_err_t suit_encode_append_delegation(const suit_delegation_t *delegation,
                                         QCBOREncodeContext *context)
{
    if (delegation->delegation_chain_num > 0) {
        QCBOREncode_BstrWrapInMapN(context, SUIT_DELEGATION);
        QCBOREncode_OpenArray(context);
        for (size_t i = 0; i < delegation->delegation_chain_num; i++) {
            QCBOREncode_OpenArray(context);
            for (size_t j = 0; j < delegation->delegation_chains[i].len; j++) {
                QCBOREncode_AddBytes(context, delegation->delegation_chains[i].chain[j]);
            }
            QCBOREncode_CloseArray(context);
        }
        QCBOREncode_CloseArray(context);
        QCBOREncode_CloseBstrWrap(context, NULL);
    }
    return SUIT_SUCCESS;
}

/*!
    \file   suit_manifest_encode.c

    \brief  This implements libcsuit encoding

    Prepare suit_eocode_t struct and suit_keys_t,
    and then call suit_encode_envelope() to encode whole SUIT manifest.
 */

suit_err_t suit_encode_append_severed_members(const suit_encoder_context_t *encoder_context,
                                              QCBOREncodeContext *context)
{
#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->dependency_resolution) &&
        encoder_context->dependency_resolution_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_DEPENDENCY_RESOLUTION, encoder_context->dependency_resolution);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->payload_fetch) &&
        encoder_context->payload_fetch_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_PAYLOAD_FETCH, encoder_context->payload_fetch);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_CANDIDATE_VERIFICATION)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->candidate_verification) &&
        encoder_context->candidate_verification_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_CANDIDATE_VERIFICATION, encoder_context->candidate_verification);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_CANDIDATE_VERIFICATION */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_INSTALL)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->install) &&
        encoder_context->install_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_INSTALL, encoder_context->install);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_INSTALL */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_TEXT)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->text) &&
        encoder_context->text_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_TEXT, encoder_context->text);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_TEXT */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_COSWID)
    if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->coswid) &&
        encoder_context->coswid_digest.bytes.len > 0) {
        QCBOREncode_AddBytesToMapN(context, SUIT_SEVERED_COSWID, encoder_context->coswid);
    }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_COSWID */

    return SUIT_SUCCESS;
}

suit_err_t suit_encode_append_manifest(const suit_encoder_context_t *encoder_context,
                                       QCBOREncodeContext *context)
{
    QCBOREncode_AddBytesToMapN(context, SUIT_MANIFEST, encoder_context->manifest);
    return SUIT_SUCCESS;
}

suit_err_t suit_encode_append_version_match(const suit_version_match_t *version_match,
                                            const uint32_t label,
                                            QCBOREncodeContext *context)
{
    if (label > 0) {
        /* in map */
        QCBOREncode_OpenArrayInMapN(context, label);
    }
    else {
        QCBOREncode_OpenArray(context);
    }
    QCBOREncode_AddInt64(context, version_match->type);
    QCBOREncode_OpenArray(context);
    for (size_t i = 0; i < version_match->value.len; i++) {
        QCBOREncode_AddInt64(context, version_match->value.int64[i]);
    }
    QCBOREncode_CloseArray(context);
    return SUIT_SUCCESS;
}

suit_err_t suit_encode_append_digest(const suit_digest_t *digest,
                                     const uint32_t label,
                                     QCBOREncodeContext *context)
{
    if (label > 0) {
        /* in map */
        QCBOREncode_OpenArrayInMapN(context, label);
    }
    else {
        QCBOREncode_OpenArray(context);
    }
    QCBOREncode_AddInt64(context, digest->algorithm_id);
    QCBOREncode_AddBytes(context, (UsefulBufC){.ptr = digest->bytes.ptr, .len = digest->bytes.len});
    QCBOREncode_CloseArray(context);
    return SUIT_SUCCESS;
}

suit_err_t suit_encode_digest(suit_encoder_context_t *encoder_context,
                              const suit_digest_t *digest,
                              UsefulBuf *buf)
{
    QCBOREncodeContext context;
    UsefulBuf tmp_buf;
    suit_err_t result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncode_Init(&context, tmp_buf);
    suit_encode_append_digest(digest, 0, &context);
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    return suit_fix_suit_encode_buf(encoder_context, t_buf.len);
}

suit_err_t suit_generate_digest_include_header(suit_encoder_context_t *encoder_context,
                                               const uint8_t *ptr,
                                               const size_t len,
                                               suit_digest_t *digest)
{
    suit_err_t result = SUIT_SUCCESS;
    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, tmp_buf);
    QCBOREncode_AddBytes(&context, (UsefulBufC){.ptr = ptr, .len = len});
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    result = suit_fix_suit_encode_buf(encoder_context, t_buf.len);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    digest->algorithm_id = SUIT_ALGORITHM_ID_SHA256;
    return suit_generate_digest_using_encode_buf(encoder_context, t_buf, digest);
}

suit_err_t suit_generate_encoded_digest(suit_encoder_context_t *encoder_context,
                                        UsefulBuf *buf)
{
    suit_err_t result = SUIT_SUCCESS;

    suit_digest_t digest;
    digest.algorithm_id = SUIT_ALGORITHM_ID_SHA256;
    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, SHA256_DIGEST_LENGTH, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    digest.bytes.ptr = tmp_buf.ptr;
    digest.bytes.len = tmp_buf.len;
    result = suit_fix_suit_encode_buf(encoder_context, digest.bytes.len);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    result = suit_generate_digest_include_header(encoder_context, encoder_context->manifest.ptr, encoder_context->manifest.len, &digest);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    result = suit_encode_digest(encoder_context, &digest, buf);

    return result;
}

suit_err_t suit_encode_append_payloads(const suit_envelope_t *envelope,
                                       QCBOREncodeContext *context)
{
    for (size_t i = 0; i < envelope->payloads.len; i++) {
        QCBOREncode_AddText(context, envelope->payloads.payload[i].key);
        QCBOREncode_AddBytes(context, envelope->payloads.payload[i].bytes);
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_encode_append_authentication_wrapper(UsefulBufC digest,
                                                     UsefulBuf signatures[],
                                                     size_t num_signature,
                                                     QCBOREncodeContext *context)
{
    QCBOREncode_BstrWrapInMapN(context, SUIT_AUTHENTICATION);
    QCBOREncode_OpenArray(context);
    QCBOREncode_AddBytes(context, digest);
    for (size_t i = 0; i < num_signature; i++) {
        QCBOREncode_AddBytes(context, UsefulBuf_Const(signatures[i]));
    }
    QCBOREncode_CloseArray(context);
    QCBOREncode_CloseBstrWrap(context, NULL);
    return SUIT_SUCCESS;
}

suit_err_t suit_append_directive_override_parameters(const suit_parameters_list_t *params_list,
                                                     QCBOREncodeContext *context)
{
    QCBOREncode_OpenMap(context);
    suit_err_t result = SUIT_SUCCESS;
    for (size_t i = 0; i < params_list->len; i++) {
        const suit_parameters_t *param = &params_list->params[i];
        switch (param->label) {
        /* int */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
        case SUIT_PARAMETER_UPDATE_PRIORITY:
            QCBOREncode_AddInt64ToMapN(context, param->label, param->value.int64);
            break;
#endif /* LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY */

        /* uint */
        case SUIT_PARAMETER_IMAGE_SIZE:
#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
        case SUIT_PARAMETER_COMPONENT_SLOT:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
        case SUIT_PARAMETER_SOURCE_COMPONENT:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
        case SUIT_PARAMETER_USE_BEFORE:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
        case SUIT_PARAMETER_MINIMUM_BATTERY:
#endif
            QCBOREncode_AddUInt64ToMapN(context, param->label, param->value.uint64);
            break;

        /* tstr */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_URI)
        case SUIT_PARAMETER_URI:
            if (param->value.string.len > 0) {
                QCBOREncode_AddTextToMapN(context, param->label, (UsefulBufC){.ptr = param->value.string.ptr, .len = param->value.string.len});
            }
            else {
                QCBOREncode_AddNULLToMapN(context, param->label);
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
#if !defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
        case SUIT_PARAMETER_CONTENT:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS)
        case SUIT_PARAMETER_INVOKE_ARGS:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
        case SUIT_PARAMETER_FETCH_ARGS:
#endif

        /* draft-ietf-suit-firmware-encryption */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO)
        case SUIT_PARAMETER_ENCRYPTION_INFO:
#endif

        /* draft-ietf-suit-update-management */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
        /* bstr .cbor SUIT_Parameter_Version_Match */
        case SUIT_PARAMETER_VERSION:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
        /* bstr .cbor SUIT_Wait_Event */
        case SUIT_PARAMETER_WAIT_INFO:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_INDEX)
        /* bstr .cbor SUIT_Component_Metadata */
        case SUIT_PARAMETER_COMPONENT_METADATA:
#endif
            QCBOREncode_AddBytesToMapN(context, param->label, (UsefulBufC){.ptr = param->value.string.ptr, .len = param->value.string.len});
            break;

        /* bool */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER)
        case SUIT_PARAMETER_STRICT_ORDER:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE)
        case SUIT_PARAMETER_SOFT_FAILURE:
#endif
#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER) || \
    !defined(LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE)
            QCBOREncode_AddBoolToMapN(context, param->label, param->value.boolean);
            break;
#endif

        /* SUIT_Digest */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            QCBOREncode_BstrWrapInMapN(context, param->label);
            result = suit_encode_append_digest(&param->value.digest, 0, context);
            QCBOREncode_CloseBstrWrap(context, NULL);
            break;

        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            break;
        }
    }
    QCBOREncode_CloseMap(context);
    return result;
}

suit_err_t suit_encode_int64_array(suit_encoder_context_t *encoder_context,
                                   const suit_int64_array_t *set_version,
                                   UsefulBuf *buf)
{
    if (set_version->len == 0) {
        buf->len = 0;
        return SUIT_SUCCESS;
    }

    suit_err_t result = SUIT_SUCCESS;
    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, tmp_buf);
    QCBOREncode_OpenArray(&context);
    for (size_t i = 0; i < set_version->len; i++) {
        QCBOREncode_AddInt64(&context, set_version->int64[i]);
    }
    QCBOREncode_CloseArray(&context);
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS && result == SUIT_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    return suit_fix_suit_encode_buf(encoder_context, t_buf.len);
}

suit_err_t suit_encode_shared_sequence(suit_encoder_context_t *encoder_context,
                                       suit_command_sequence_t *cmd_seq,
                                       UsefulBuf *buf)
{
    if (cmd_seq->len == 0) {
        buf->len = 0;
        return SUIT_SUCCESS;
    }

    suit_err_t result = SUIT_SUCCESS;
    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, tmp_buf);
    QCBOREncode_OpenArray(&context);
    size_t extra = 0;
    for (size_t i = 0; i < cmd_seq->len; i++) {
        const suit_command_sequence_item_t *item = &cmd_seq->commands[i];
        if (item->label == SUIT_CONDITION_INVALID) {
            continue;
        }
        switch (item->label) {
        /* SUIT_Rep_Policy */
        case SUIT_CONDITION_VENDOR_IDENTIFIER:
#if !defined(LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER)
        case SUIT_CONDITION_CLASS_IDENTIFIER:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER)
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
#endif
        case SUIT_CONDITION_IMAGE_MATCH:
#if !defined(LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT)
        case SUIT_CONDITION_COMPONENT_SLOT:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT)
        case SUIT_CONDITION_CHECK_CONTENT:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_ABORT)
        case SUIT_CONDITION_ABORT:
#endif

        /* in draft-ietf-suit-update-management */
#if !defined(LIBCSUIT_DISABLE_CONDITION_USE_BEFORE)
        case SUIT_CONDITION_USE_BEFORE:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_IMAGE_NOT_MATCH)
        case SUIT_CONDITION_IMAGE_NOT_MATCH:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY)
        case SUIT_CONDITION_MINIMUM_BATTERY:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED)
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_VERSION)
        case SUIT_CONDITION_VERSION:
#endif

        /* in draft-ietf-suit-trust-domains */
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY)
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY)
        case SUIT_CONDITION_IS_DEPENDENCY:
#endif

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WRITE)
        case SUIT_DIRECTIVE_WRITE:
#endif
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_FETCH)
        case SUIT_DIRECTIVE_FETCH:
#endif
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY)
        case SUIT_DIRECTIVE_COPY:
#endif
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_INVOKE)
        case SUIT_DIRECTIVE_INVOKE:
#endif
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SWAP)
        case SUIT_DIRECTIVE_SWAP:
#endif

        /* in draft-ietf-suit-update-management */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WAIT)
        case SUIT_DIRECTIVE_WAIT:
#endif

        /* in draft-ietf-suit-trust-domains */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY)
        case SUIT_DIRECTIVE_PROCESS_DEPENDENCY:
#endif
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_UNLINK)
        case SUIT_DIRECTIVE_UNLINK:
#endif
            QCBOREncode_AddUInt64(&context, item->label);
            QCBOREncode_AddUInt64(&context, item->value.uint64);
            break;

        /* bstr */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            QCBOREncode_AddUInt64(&context, item->label);
            QCBOREncode_AddBytes(&context, (UsefulBufC){.ptr = item->value.string.ptr, .len = item->value.string.len});
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

        /* uint, true, [ + uint ] */
        case SUIT_DIRECTIVE_SET_COMPONENT_INDEX:
            if (item->value.index_arg.len == 1) {
                QCBOREncode_AddUInt64(&context, item->label);
                QCBOREncode_AddUInt64(&context, item->value.index_arg.index[0]);
            }
#if !defined(LIBCSUIT_DISABLE_COMPONENT_INDEX_TYPE_BOOLEAN)
            else if (item->value.index_arg.len == 0) {
                QCBOREncode_AddUInt64(&context, item->label);
                QCBOREncode_AddBool(&context, true);
            }
#endif /* !LIBCSUIT_DISABLE_COMPONENT_INDEX_TYPE_BOOLEAN */
#if !defined(LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY)
            else if (item->value.index_arg.len < SUIT_MAX_INDEX_NUM) {
                QCBOREncode_AddUInt64(&context, item->label);
                QCBOREncode_OpenArray(&context);
                for (size_t j = 0; j < item->value.index_arg.len; j++) {
                    QCBOREncode_AddUInt64(&context, item->value.index_arg.index[j]);
                }
                QCBOREncode_CloseArray(&context);
            }
#endif /* !LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY */
            else {
                return SUIT_ERR_INVALID_VALUE;
            }
            break;

        /* $$SUIT_Parameters */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS)
        case SUIT_DIRECTIVE_SET_PARAMETERS:
#endif
        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            QCBOREncode_AddUInt64(&context, item->label);
            result = suit_append_directive_override_parameters(&item->value.params_list, &context);
            break;

        /* SUIT_Override_Mult_Arg */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE)
        case SUIT_DIRECTIVE_OVERRIDE_MULTIPLE:
            /*
                Encode expanded items into one map.

                e.g.
                [ override-multiple, 0, A ],
                [ override-multiple, 1, B ],
                [ override-multiple, 2, C ]
                =>
                suit-directive-override-multiple: {
                    / index / 0: { + $$SUIT_Parameters = A },
                    / index / 1: { + $$SUIT_Parameters = B },
                    / index / 2: { + $$SUIT_Parameters = C }
                }
             */
            QCBOREncode_OpenMapInMapN(&context, item->label);
            for (extra = 0; extra < cmd_seq->len; extra++) {
                item = &cmd_seq->commands[i + extra];
                if (item->label != SUIT_DIRECTIVE_OVERRIDE_MULTIPLE) {
                    break;
                }
                QCBOREncode_AddInt64(&context, item->value.params_list.index);
                result = suit_append_directive_override_parameters(&item->value.params_list, &context);
                if (result != SUIT_SUCCESS) {
                    break;
                }
            }
            QCBOREncode_CloseMap(&context);
            i += extra - 1;
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE */

        /* SUIT_Directive_Copy_Params */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS)
        case SUIT_DIRECTIVE_COPY_PARAMS:
            /*
                Encode expanded items into one map.

                e.g.
                [ copy-params, 0, A ],
                [ copy-params, 1, B ],
                [ copy-params, 2, C ],
                =>
                copy-params: {
                    / src-index / 0: [ + int = A ],
                    / src-index / 1: [ + int = B ],
                    / src-index / 2: [ + int = C ]
                }
             */
            QCBOREncode_OpenMapInMapN(&context, item->label);
            for (extra = 0; extra < cmd_seq->len; extra++) {
                if (cmd_seq->commands[i + extra].label != SUIT_DIRECTIVE_COPY_PARAMS) {
                    break;
                }
                QCBOREncode_OpenArrayInMapN(&context, cmd_seq->commands[i + extra].value.copy_params.src_index);
                for (size_t j = 0; j < cmd_seq->commands[i + extra].value.copy_params.int64s.len; j++) {
                    QCBOREncode_AddInt64(&context, cmd_seq->commands[i + extra].value.copy_params.int64s.int64[j]);
                }
                QCBOREncode_CloseArray(&context);
            }
            QCBOREncode_CloseMap(&context);
            i += extra - 1;
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS */

        /* SUIT_Directive_Try_Each_Argument */
#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
        case SUIT_DIRECTIVE_TRY_EACH:
            /*
                Encode expanded items into one array.

                e.g.
                [ try-each, << A >> ],
                [ try-each, << B >> ],
                [ try-each, << C >> ]
                =>
                try-each: [
                    << SUIT_Command_Sequence = A >>,
                    << SUIT_Command_Sequence = B >>,
                    << SUIT_Command_Sequence = C >>
                ]
             */
            QCBOREncode_AddUInt64(&context, item->label);
            QCBOREncode_OpenArray(&context);
            for (extra = 0; extra < cmd_seq->len; extra++) {
                if (cmd_seq->commands[i + extra].label != SUIT_DIRECTIVE_TRY_EACH) {
                    break;
                }
                QCBOREncode_AddBytes(&context, (UsefulBufC){
                    .ptr = cmd_seq->commands[i + extra].value.string.ptr,
                    .len = cmd_seq->commands[i + extra].value.string.len}
                );
                cmd_seq->commands[i + extra].label = SUIT_CONDITION_INVALID;
            }
            QCBOREncode_CloseArray(&context);
            i += extra - 1;
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

        default:
            return SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    QCBOREncode_CloseArray(&context);
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS && result == SUIT_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    return suit_fix_suit_encode_buf(encoder_context, t_buf.len);
}

suit_err_t suit_encode_append_component_identifier(const suit_component_identifier_t *component_id,
                                                   uint32_t label,
                                                   QCBOREncodeContext *context)
{
    if (label > 0) {
        QCBOREncode_OpenArrayInMapN(context, label);
    }
    else {
        QCBOREncode_OpenArray(context);
    }
    for (size_t j = 0; j < component_id->len; j++) {
        QCBOREncode_AddBytes(context, component_id->identifier[j]);
    }
    QCBOREncode_CloseArray(context);
    return SUIT_SUCCESS;
}

suit_err_t suit_encode_common(suit_encoder_context_t *encoder_context,
                              const suit_common_t *suit_common,
                              UsefulBuf *buf)
{
    UsefulBuf suit_shared_sequence_buf;
    suit_err_t result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&suit_common->shared_seq, &suit_shared_sequence_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    UsefulBuf suit_common_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &suit_common_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    QCBOREncodeContext context;
    QCBOREncode_Init(&context, suit_common_buf);
    QCBOREncode_OpenMap(&context);

    // suit-dependencies
#if !defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
    if (suit_common->dependencies.len > 0) {
        QCBOREncode_OpenMapInMapN(&context, SUIT_DEPENDENCIES);
        for (size_t i = 0; i <suit_common->dependencies.len; i++) {
            const suit_dependency_t *dependency = &suit_common->dependencies.dependency[i];
            QCBOREncode_AddUInt64(&context, dependency->index);
            QCBOREncode_OpenMap(&context);
            suit_encode_append_component_identifier(&dependency->dependency_metadata.prefix, SUIT_DEPENDENCY_PREFIX, &context);
            //TODO: SUIT_Dependency-extensions
            QCBOREncode_CloseMap(&context);
        }
        QCBOREncode_CloseMap(&context);
    }
#endif /* LIBCSUIT_DISABLE_COMMON_DEPENDENCIES */

    // suit-components
    if (suit_common->components_len > 0) {
        QCBOREncode_OpenArrayInMapN(&context, SUIT_COMPONENTS);
        for (size_t i = 0; i < suit_common->components_len; i++) {
            suit_encode_append_component_identifier(&suit_common->components[i].component, 0, &context);
        }
        QCBOREncode_CloseArray(&context);
    }

    // suit-shared-sequence
    if (suit_shared_sequence_buf.len > 0) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_SHARED_SEQUENCE, UsefulBuf_Const(suit_shared_sequence_buf));
    }

    QCBOREncode_CloseMap(&context);
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS && result == SUIT_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    result = suit_fix_suit_encode_buf(encoder_context, t_buf.len);
    return result;
}

#if !defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
suit_err_t suit_encoder_context_text_lmap(const suit_text_lmap_t *text,
                                          UsefulBuf tag38_buf,
                                          QCBOREncodeContext *context)
{
    suit_err_t result = SUIT_SUCCESS;
    if (text->tag38_ltag.len + 1 > tag38_buf.len) {
        return SUIT_ERR_NO_MEMORY;
    }
    memcpy(tag38_buf.ptr, text->tag38_ltag.ptr, text->tag38_ltag.len);
    ((char *)tag38_buf.ptr)[text->tag38_ltag.len] = '\0';
    QCBOREncode_OpenMapInMap(context, tag38_buf.ptr);

    // SUIT_Text_Keys : tstr
    if (text->manifest_description.len > 0) {
        QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MANIFEST_DESCRIPTION, (UsefulBufC){.ptr = text->manifest_description.ptr, .len = text->manifest_description.len});
    }
    if (text->update_description.len > 0) {
        QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MANIFEST_DESCRIPTION, (UsefulBufC){.ptr = text->update_description.ptr, .len = text->manifest_description.len});
    }
    if (text->manifest_json_source.len > 0) {
        QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MANIFEST_DESCRIPTION, (UsefulBufC){.ptr = text->manifest_json_source.ptr, .len = text->manifest_description.len});
    }
    if (text->manifest_yaml_source.len > 0) {
        QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MANIFEST_DESCRIPTION, (UsefulBufC){.ptr = text->manifest_yaml_source.ptr, .len = text->manifest_description.len});
    }
    // TODO suit-text-key-extensions

    // SUIT_Component_Identifier : {}
    for (size_t i = 0; i < text->component_len; i++) {
        const suit_component_identifier_t *component = &text->component[i].key;
        QCBOREncode_OpenArray(context);
        for (size_t j = 0; j < component->len; j++) {
            QCBOREncode_AddBytes(context, (UsefulBufC){.ptr = component->identifier[j].ptr, .len = component->identifier[j].len});
        }
        QCBOREncode_CloseArray(context);
        QCBOREncode_OpenMap(context);
        const suit_text_component_t *text_component = &text->component[i].text_component;
        if (text_component->vendor_name.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_VENDOR_NAME, (UsefulBufC){.ptr = text_component->vendor_name.ptr, .len = text_component->vendor_name.len});
        }
        if (text_component->model_name.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MODEL_NAME, (UsefulBufC){.ptr = text_component->model_name.ptr, .len = text_component->model_name.len});
        }
        if (text_component->vendor_domain.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_VENDOR_DOMAIN, (UsefulBufC){.ptr = text_component->vendor_domain.ptr, .len = text_component->vendor_domain.len});
        }
        if (text_component->model_info.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_MODEL_INFO, (UsefulBufC){.ptr = text_component->model_info.ptr, .len = text_component->model_info.len});
        }
        if (text_component->component_description.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_COMPONENT_DESCRIPTION, (UsefulBufC){.ptr = text_component->component_description.ptr, .len = text_component->component_description.len});
        }
        if (text_component->component_version.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_COMPONENT_VERSION, (UsefulBufC){.ptr = text_component->component_version.ptr, .len = text_component->component_version.len});
        }
        /* in draft-ietf-suit-update-management */
        if (text_component->version_required.len > 0) {
            QCBOREncode_AddTextToMapN(context, SUIT_TEXT_VERSION_REQUIRED, (UsefulBufC){.ptr = text_component->version_required.ptr, .len = text_component->version_required.len});
        }
        // TODO suit-text-component-key-extensions
        QCBOREncode_CloseMap(context);
    }
    QCBOREncode_CloseMap(context);

    return result;
}

suit_err_t suit_encoder_context_text(suit_encoder_context_t *encoder_context,
                                     const suit_text_map_t *text,
                                     UsefulBuf *buf)
{
    suit_err_t result = SUIT_SUCCESS;

    UsefulBuf tag38_buf;
    result = suit_use_suit_encode_buf(encoder_context, SUIT_MAX_NAME_LENGTH, &tag38_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    result = suit_fix_suit_encode_buf(encoder_context, SUIT_MAX_NAME_LENGTH);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, tmp_buf);
    QCBOREncode_OpenMap(&context);
    for (size_t i = 0; i < text->text_lmaps_len; i++) {
        result = suit_encoder_context_text_lmap(&text->text_lmaps[i], tag38_buf, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    QCBOREncode_CloseMap(&context);
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    return suit_fix_suit_encode_buf(encoder_context, t_buf.len);
}

suit_err_t suit_encoder_context_text_bstr(suit_encoder_context_t *encoder_context,
                                          const suit_text_map_t *text,
                                          UsefulBuf *buf)
{
    suit_err_t result = SUIT_SUCCESS;
    UsefulBuf text_buf;
    result = suit_encoder_context_text(encoder_context, text, &text_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    UsefulBuf tmp_buf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &tmp_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    QCBOREncodeContext context;
    QCBOREncode_Init(&context, *buf);
    QCBOREncode_AddBytes(&context, (UsefulBufC){.ptr = tmp_buf.ptr, .len = tmp_buf.len});
    UsefulBufC t_buf;
    QCBORError error = QCBOREncode_Finish(&context, &t_buf);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    *buf = (UsefulBuf){.ptr = (void *)t_buf.ptr, .len = t_buf.len};
    return suit_fix_suit_encode_buf(encoder_context, t_buf.len);
}
#endif /* !LIBCSUIT_DISABLE_MANIFEST_TEXT */

/*!
    \brief  Encode suit-manifest

    \param[in]  envelope    Input struct of libcsuit, correspond to the SUIT_Envelope.
    \param[out] suit_encode Internal struct holding the status of encoding binary.

    \return     This returns one of the error codes defined by \ref suit_err_t.

    This is the "map" of the encoding process.
    \code{.unparsed}
    SUIT_Envelope {
        suit-authentication-wrapper,
        suit-manifest { // <= You are here!
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
suit_err_t suit_encode_manifest(suit_encoder_context_t *encoder_context,
                                const suit_envelope_t *envelope)
{
    /*
     * Encode each bstr wrapped element first
     * and then create whole manifest file,
     * because some elements would be taken their digests
     */
    const suit_manifest_t *manifest = &envelope->manifest;
    UsefulBuf suit_common = NULLUsefulBuf;
    suit_err_t result = suit_encode_common(encoder_context, &manifest->common, &suit_common);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    /* encode unseverable members */
    UsefulBuf validate_buf = NULLUsefulBuf;
    if (manifest->unsev_mem.validate.len > 0) {
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->unsev_mem.validate, &validate_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }

#if !defined(LIBCSUIT_DISABLE_MANIFEST_SET_VERSION)
    UsefulBuf set_version_buf = NULLUsefulBuf;
    if (manifest->unsev_mem.set_version.len > 0) {
        result = suit_encode_int64_array(encoder_context, &manifest->unsev_mem.set_version, &set_version_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
#endif

#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
    UsefulBuf load_buf = NULLUsefulBuf;
    if (manifest->unsev_mem.load.len > 0) {
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->unsev_mem.load, &load_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
    UsefulBuf invoke_buf = NULLUsefulBuf;
    if (manifest->unsev_mem.invoke.len > 0) {
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->unsev_mem.invoke, &invoke_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
    UsefulBuf uninstall_buf = NULLUsefulBuf;
    if (manifest->unsev_mem.uninstall.len > 0) {
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->unsev_mem.uninstall, &uninstall_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

    /* encode severable members */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
    if (manifest->sev_man_mem.dependency_resolution_status & SUIT_SEVERABLE_EXISTS) {
        UsefulBuf dependency_resolution_buf = NULLUsefulBuf;
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->sev_man_mem.dependency_resolution, &dependency_resolution_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        encoder_context->dependency_resolution = UsefulBuf_Const(dependency_resolution_buf);

        if (manifest->sev_man_mem.dependency_resolution_status & SUIT_SEVERABLE_IN_ENVELOPE) {
            result = suit_generate_digest_include_header(encoder_context, encoder_context->dependency_resolution.ptr, encoder_context->dependency_resolution.len, &encoder_context->dependency_resolution_digest);
            if (result != SUIT_SUCCESS) {
                return result;
            }
        }
    }
    else if (manifest->sev_mem_dig.dependency_resolution.bytes.len > 0) {
#if defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        encoder_context->dependency_resolution_digest = manifest->sev_mem_dig.dependency_resolution;
#endif /* LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
    if (manifest->sev_man_mem.payload_fetch_status & SUIT_SEVERABLE_EXISTS) {
        UsefulBuf payload_fetch_buf = NULLUsefulBuf;
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->sev_man_mem.payload_fetch, &payload_fetch_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        encoder_context->payload_fetch = UsefulBuf_Const(payload_fetch_buf);

        if (manifest->sev_man_mem.payload_fetch_status & SUIT_SEVERABLE_IN_ENVELOPE) {
            result = suit_generate_digest_include_header(encoder_context, encoder_context->payload_fetch.ptr, encoder_context->payload_fetch.len, &encoder_context->payload_fetch_digest);
            if (result != SUIT_SUCCESS) {
                return result;
            }
        }
    }
    else if (manifest->sev_mem_dig.payload_fetch.bytes.len > 0) {
#if defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
        return SUIT_ERR_NOT_IMPLEMENETD;
#else
        encoder_context->payload_fetch_digest = manifest->sev_mem_dig.payload_fetch;
#endif /* LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_COSWID)
    if (manifest->sev_man_mem.coswid_status & SUIT_SEVERABLE_EXISTS) {
        encoder_context->coswid = (UsefulBufC){.ptr = manifest->sev_man_mem.coswid.ptr, .len = manifest->sev_man_mem.coswid.len};
        if (manifest->sev_man_mem.coswid_status & SUIT_SEVERABLE_IN_ENVELOPE) {
            result = suit_generate_digest_include_header(encoder_context, encoder_context->coswid.ptr, encoder_context->coswid.len, &encoder_context->coswid_digest);
            if (result != SUIT_SUCCESS) {
                return result;
            }
        }
    }
    else if (manifest->sev_mem_dig.coswid.bytes.len > 0) {
#if defined(LIBCSUIT_DISABLE_ENVELOPE_COSWID)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        encoder_context->coswid_digest = manifest->sev_mem_dig.coswid;
#endif /* LIBCSUIT_DISABLE_ENVELOPE_COSWID */
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COSWID */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
    if (manifest->sev_man_mem.install_status & SUIT_SEVERABLE_EXISTS) {
        UsefulBuf install_buf = NULLUsefulBuf;
        result = suit_encode_shared_sequence(encoder_context, (suit_command_sequence_t *)&manifest->sev_man_mem.install, &install_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        encoder_context->install = UsefulBuf_Const(install_buf);

        if (manifest->sev_man_mem.install_status & SUIT_SEVERABLE_IN_ENVELOPE) {
            result = suit_generate_digest_include_header(encoder_context, encoder_context->install.ptr, encoder_context->install.len, &encoder_context->install_digest);
            if (result != SUIT_SUCCESS) {
                return result;
            }
        }
    }
    else if (manifest->sev_mem_dig.install.bytes.len > 0) {
#if defined(LIBCSUIT_DISABLE_ENVELOPE_INSTALL)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        encoder_context->install_digest = manifest->sev_mem_dig.install;
#endif /* LIBCSUIT_DISABLE_ENVELOPE_INSTALL */
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INSTALL */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
    if (manifest->sev_man_mem.text_status & SUIT_SEVERABLE_EXISTS) {
        UsefulBuf text_buf = NULLUsefulBuf;
        result = suit_encoder_context_text(encoder_context, &manifest->sev_man_mem.text, &text_buf);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        encoder_context->text = UsefulBuf_Const(text_buf);

        if (manifest->sev_man_mem.text_status & SUIT_SEVERABLE_IN_ENVELOPE) {
            result = suit_generate_digest_include_header(encoder_context, encoder_context->text.ptr, encoder_context->text.len, &encoder_context->text_digest);
            if (result != SUIT_SUCCESS) {
                return result;
            }
        }
    }
    else if (manifest->sev_mem_dig.text.bytes.len > 0) {
#if defined(LIBCSUIT_DISABLE_ENVELOPE_TEXT)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        encoder_context->text_digest = manifest->sev_mem_dig.text;
#endif /* LIBCSUIT_DISABLE_ENVELOPE_TEXT */
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_TEXT */


    /* Encode whole manifest */
    UsefulBuf suit_manifest = NULLUsefulBuf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &suit_manifest);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    QCBOREncodeContext context;
    QCBOREncode_Init(&context, suit_manifest);
    QCBOREncode_OpenMap(&context);
    // NOTE: create canonical (deterministic) cbor
    // 1
    QCBOREncode_AddUInt64ToMapN(&context, SUIT_MANIFEST_VERSION, manifest->version);
    // 2
    QCBOREncode_AddUInt64ToMapN(&context, SUIT_MANIFEST_SEQUENCE_NUMBER, manifest->sequence_number);
    // 3
    QCBOREncode_AddBytesToMapN(&context, SUIT_COMMON, UsefulBuf_Const(suit_common));

    // 4
#if !defined(LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI)
    if (manifest->reference_uri.len > 0) {
        QCBOREncode_AddTextToMapN(&context, SUIT_REFERENCE_URI, (UsefulBufC){.ptr = manifest->reference_uri.ptr, .len = manifest->reference_uri.len});
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI */

    // 5
#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
    if (manifest->manifest_component_id.len > 0) {
        suit_encode_append_component_identifier(&manifest->manifest_component_id, SUIT_MANIFEST_COMPONENT_ID, &context);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID */

    // 6
#if !defined(LIBCSUIT_DISABLE_MANIFEST_SET_VERSION)
    if (!UsefulBuf_IsNULLOrEmpty(set_version_buf)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_SET_VERSION, UsefulBuf_Const(set_version_buf));
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_SET_VERSION */

    // 7
    if (!UsefulBuf_IsNULLOrEmpty(validate_buf)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_VALIDATE, UsefulBuf_Const(validate_buf));
    }

    // 8
#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
    if (!UsefulBuf_IsNULLOrEmpty(load_buf)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_LOAD, UsefulBuf_Const(load_buf));
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

    // 9
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
    if (!UsefulBuf_IsNULLOrEmpty(invoke_buf)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_INVOKE, UsefulBuf_Const(invoke_buf));
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

    // 14
#if !defined(LIBCSUIT_DISABLE_MANIFEST_COSWID)
    if (encoder_context->coswid_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_COSWID)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_COSWID);
        result = suit_encode_append_digest(&encoder_context->coswid_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* LIBCSUIT_DISABLE_ENVELOPE_COSWID */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->coswid)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_COSWID, encoder_context->coswid);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COSWID */

    // 15
#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
    if (encoder_context->dependency_resolution_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_DEPENDENCY_RESOLUTION);
        result = suit_encode_append_digest(&encoder_context->dependency_resolution_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->dependency_resolution)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_DEPENDENCY_RESOLUTION, encoder_context->dependency_resolution);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

    // 16
#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
    if (encoder_context->payload_fetch_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_PAYLOAD_FETCH);
        result = suit_encode_append_digest(&encoder_context->payload_fetch_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->payload_fetch)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_PAYLOAD_FETCH, encoder_context->payload_fetch);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

    // 18
#if !defined(LIBCSUIT_DISABLE_MANIFEST_CANDIDATE_VERIFICATION)
    if (encoder_context->candidate_verification_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_CANDIDATE_VERIFICATION)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_CANDIDATE_VERIFICATION);
        result = suit_encode_append_digest(&encoder_context->candidate_verification_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* LIBCSUIT_DISABLE_ENVELOPE_CANDIDATE_VERIFICATION */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->candidate_verification)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_CANDIDATE_VERIFICATION, encoder_context->candidate_verification);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_CANDIDATE_VERIFICATION */

    // 20
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
    if (encoder_context->install_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_INSTALL)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_INSTALL);
        result = suit_encode_append_digest(&encoder_context->install_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* LIBCSUIT_DISABLE_ENVELOPE_INSTALL */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->install)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_INSTALL, encoder_context->install);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INSTALL */

    // 23
#if !defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
    if (encoder_context->text_digest.bytes.len > 0) {
        /* severed */
#if defined(LIBCSUIT_DISABLE_ENVELOPE_TEXT)
        return SUIT_ERR_NOT_IMPLEMENTED;
#else
        QCBOREncode_AddUInt64(&context, SUIT_TEXT);
        result = suit_encode_append_digest(&encoder_context->text_digest, 0, &context);
        if (result != SUIT_SUCCESS) {
            return result;
        }
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_TEXT */
    }
    else if (!UsefulBuf_IsNULLOrEmptyC(encoder_context->text)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_TEXT, encoder_context->text);
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_TEXT */

    // 24
#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
    if (!UsefulBuf_IsNULLOrEmpty(uninstall_buf)) {
        QCBOREncode_AddBytesToMapN(&context, SUIT_UNINSTALL, UsefulBuf_Const(uninstall_buf));
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

    QCBOREncode_CloseMap(&context);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    QCBORError error = QCBOREncode_Finish(&context, &encoder_context->manifest);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    return suit_fix_suit_encode_buf(encoder_context, encoder_context->manifest.len);
}

/*
    Public function. See suit_manifest_encode.h
 */
suit_err_t suit_encode_envelope(suit_encoder_context_t *encoder_context,
                                const suit_envelope_t *envelope,
                                UsefulBufC *encoded_manifest)
{
    suit_err_t result = SUIT_SUCCESS;
    result = suit_encode_manifest(encoder_context, envelope);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    /* calculate digest and signatures of suit-manifest */
    UsefulBuf digest;
    result = suit_generate_encoded_digest(encoder_context, &digest);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    UsefulBuf signatures[SUIT_MAX_ARRAY_LENGTH] = {0};
    size_t num_signatures = 0;
    for (size_t i = 0; i < SUIT_MAX_KEY_NUM; i++) {
        if (encoder_context->mechanisms[i].use == false) {
            continue;
        }
        switch (encoder_context->mechanisms[i].key.cose_algorithm_id) {
        case T_COSE_ALGORITHM_ES256:
        case T_COSE_ALGORITHM_ESP256:
        case T_COSE_ALGORITHM_HMAC256:
            result = SUIT_SUCCESS;
            break;
        default:
            continue;
        }

        result = suit_use_suit_encode_buf(encoder_context, 0, &signatures[num_signatures]);
        if (result != SUIT_SUCCESS) {
            return result;
        }

        switch (encoder_context->mechanisms[i].cose_tag) {
        case COSE_SIGN1_TAG:
            result = suit_sign_cose_sign1(UsefulBuf_Const(digest), &encoder_context->mechanisms[i].key, &signatures[num_signatures]);
            break;
        case COSE_MAC0_TAG:
            result = suit_compute_cose_mac0(UsefulBuf_Const(digest), &encoder_context->mechanisms[i].key, &signatures[num_signatures]);
            break;
        case COSE_SIGN_TAG:
        case COSE_MAC_TAG:
        case COSE_ENCRYPT_TAG:
        case COSE_ENCRYPT0_TAG:
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            return result;
        }

        result = suit_fix_suit_encode_buf(encoder_context, signatures[num_signatures].len);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        num_signatures++;
    }

    UsefulBuf suit_envelope = NULLUsefulBuf;
    result = suit_use_suit_encode_buf(encoder_context, 0, &suit_envelope);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, suit_envelope);
    if (envelope->tagged) {
        QCBOREncode_AddTag(&context, SUIT_ENVELOPE_TAG);
    }
    QCBOREncode_OpenMap(&context);

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DELEGATION)
    result = suit_encode_append_delegation(&envelope->delegation, &context);
    if (result != SUIT_SUCCESS) {
        goto out;
    }
#endif

    result = suit_encode_append_authentication_wrapper(UsefulBuf_Const(digest), signatures, num_signatures, &context);
    if (result != SUIT_SUCCESS) {
        goto out;
    }

    result = suit_encode_append_manifest(encoder_context, &context);
    if (result != SUIT_SUCCESS) {
        goto out;
    }

    result = suit_encode_append_severed_members(encoder_context, &context);
    if (result != SUIT_SUCCESS) {
        goto out;
    }

    result = suit_encode_append_payloads(envelope, &context);
    if (result != SUIT_SUCCESS) {
        goto out;
    }

out:
    QCBOREncode_CloseMap(&context);
    QCBORError error = QCBOREncode_Finish(&context, encoded_manifest);
    if (error != QCBOR_SUCCESS && result == SUIT_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }
    return suit_fix_suit_encode_buf(encoder_context, encoded_manifest->len);
}

suit_err_t suit_encode_add_sender_key(
    suit_encoder_context_t *encoder_context,
    const int cose_tag,
    int cose_algorithm_id,
    suit_key_t *sender_key)
{

    size_t i = 0;
    for (; i < SUIT_MAX_KEY_NUM; i++) {
        if (encoder_context->mechanisms[i].cose_tag == COSE_TAG_INVALID) {
            break;
        }
    }
    if (i == SUIT_MAX_KEY_NUM) {
        return SUIT_ERR_NO_MEMORY;
    }

    encoder_context->mechanisms[i].key = *sender_key;
    if (encoder_context->mechanisms[i].key.cose_algorithm_id == T_COSE_ALGORITHM_RESERVED) {
        encoder_context->mechanisms[i].key.cose_algorithm_id = cose_algorithm_id;
    }
    encoder_context->mechanisms[i].use = true;
    encoder_context->mechanisms[i].cose_tag = cose_tag;

    return SUIT_SUCCESS;
}

suit_err_t suit_encode_init(suit_encoder_context_t *encoder_context, size_t buf_size)
{
    memset(encoder_context, 0, sizeof(suit_encoder_context_t));
    encoder_context->max_pos = buf_size;

    return SUIT_SUCCESS;
}
