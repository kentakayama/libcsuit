/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_reporting_engine.c

    \brief  This implements SUIT Report encoder

    Call suit_report_init_engine() and pass it to the suit_processor_init().
 */

#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_reporting_engine.h"
#include "csuit/suit_manifest_process.h"

void suit_report_append_parameter(
    QCBOREncodeContext *cbor_encoder,
    const suit_parameter_key_t parameter_key,
    const struct suit_union_parameter *parameter_value)
{
    switch (parameter_key) {
    /* int */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
    case SUIT_PARAMETER_UPDATE_PRIORITY:
        QCBOREncode_AddInt64ToMapN(cbor_encoder, parameter_key, parameter_value->i64);
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
        QCBOREncode_AddUInt64ToMapN(cbor_encoder, parameter_key, parameter_value->u64);
        break;

    /* tstr */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_URI)
    case SUIT_PARAMETER_URI:
        QCBOREncode_AddTextToMapN(cbor_encoder, parameter_key, parameter_value->str);
        break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

    /* bstr */
    case SUIT_PARAMETER_VENDOR_IDENTIFIER:
    case SUIT_PARAMETER_IMAGE_DIGEST:
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
        QCBOREncode_AddBytesToMapN(cbor_encoder, parameter_key, parameter_value->str);
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
        QCBOREncode_AddBoolToMapN(cbor_encoder, parameter_key, parameter_value->b == SUIT_PARAMETER_TRUE);
        break;
#endif

    case SUIT_PARAMETER_INVALID:
        break;
    }
}

void suit_encode_append_suit_record(
    QCBOREncodeContext *cbor_encoder,
    const uint32_t label,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value)
{
    if (label != 0) {
        QCBOREncode_OpenArrayInMapN(cbor_encoder, label);
    }
    else {
        QCBOREncode_OpenArray(cbor_encoder);
    }
    // SUIT_Record [ start
    {
        // suit-record-manifest-id: [* uint ]
        QCBOREncode_OpenArray(cbor_encoder);
        for (size_t i = 0; i < dependency_tree.len; i++) {
            QCBOREncode_AddUInt64(cbor_encoder, dependency_tree.manifest_index[i]);
        }
        QCBOREncode_CloseArray(cbor_encoder);

        // suit-record-manifest-section: int
        QCBOREncode_AddInt64(cbor_encoder, manifest_key);

        // suit-record-section-offset : uint
        QCBOREncode_AddUInt64(cbor_encoder, section_offset);

        // suit-record-component-index : uint
        // NOTE: none of SUIT_Component_Identifier is processed here
        QCBOREncode_AddUInt64(cbor_encoder, 0);

        // suit-record-properties : {*$$SUIT_Parameters}
        // TODO: 
        QCBOREncode_OpenMap(cbor_encoder);
        for (size_t i = 0; i < LIBCSUIT_MAX_REPORT_PRAMETER_NUM; i++) {
            suit_report_append_parameter(cbor_encoder, parameter_keys[i], parameter_value);
        }
        QCBOREncode_CloseMap(cbor_encoder);
    }
    QCBOREncode_CloseArray(cbor_encoder);
}

suit_err_t suit_report_stop_records(suit_report_context_t *report_context)
{
    switch (report_context->state) {
    case SUIT_REPORTING_ENGINE_IN_SYSTEM_PROPERTY_CLAIMS:
        // close map for
        // system-property-claims = {
        //   system-component-id => SUIT_Component_Identifier,
        //   + $$SUIT_Parameters,
        // } <- this one
        QCBOREncode_CloseMap(&report_context->cbor_encoder);
        // fall through
    case SUIT_REPORTING_ENGINE_IN_REPORT_RECORD:
        // suit-report-records => [
        //   * SUIT_Record / system-property-claims
        // ] <- this one
        QCBOREncode_CloseArray(&report_context->cbor_encoder);
        break;
    case SUIT_REPORTING_ENGINE_INITIALIZED:
    case SUIT_REPORTING_ENGINE_SUIT_DIGEST_STORED:
    case SUIT_REPORTING_ENGINE_RECORDING_STOPPED:
        // in SUIT_Report, ignored
        break;
    case SUIT_REPORTING_ENGINE_NOT_INITIALIZED:
    case SUIT_REPORTING_ENGINE_KEY_LOADED:
    case SUIT_REPORTING_ENGINE_STARTED:
    case SUIT_REPORTING_ENGINE_SUIT_REPORT_IS_ENCODED:
    case SUIT_REPORTING_ENGINE_FINALIZED:
        return SUIT_ERR_WHILE_REPORTING;
    }

    report_context->state = SUIT_REPORTING_ENGINE_RECORDING_STOPPED;
    return SUIT_SUCCESS;
}

suit_report_reason_t suit_report_reason_from_suit_err(suit_err_t result)
{
    switch (result) {
    case SUIT_ERR_NOT_A_SUIT_MANIFEST:
    case SUIT_ERR_NOT_CANONICAL_CBOR:
    case SUIT_ERR_REDUNDANT:
    case SUIT_ERR_MANIFEST_KEY_NOT_FOUND:
    case SUIT_ERR_INVALID_TYPE_OF_VALUE:
    case SUIT_ERR_INVALID_VALUE:
    case SUIT_ERR_INVALID_TYPE_OF_KEY:
    case SUIT_ERR_INVALID_KEY:
    case SUIT_ERR_NO_MORE_ITEMS:
    case SUIT_ERR_NO_MEMORY:
    case SUIT_ERR_FATAL:
        return SUIT_REPORT_REASON_CBOR_PARSE;

    case SUIT_ERR_NOT_IMPLEMENTED:
    case SUIT_ERR_INVALID_MANIFEST_VERSION:
        return SUIT_REPORT_REASON_COMMAND_UNSUPPORTED;

    case SUIT_ERR_AUTHENTICATION_NOT_FOUND:
    case SUIT_ERR_FAILED_TO_VERIFY:
    case SUIT_ERR_FAILED_TO_VERIFY_DELEGATION:
        return SUIT_REPORT_REASON_UNAUTHORIZED;

    case SUIT_ERR_COMPONENT_NOT_FOUND:
        return SUIT_REPORT_REASON_COMPONENT_UNSUPPORTED;

    case SUIT_ERR_PARAMETER_NOT_FOUND:
        return SUIT_REPORT_REASON_PARAMETER_UNSUPPORTED;

    case SUIT_ERR_FAILED_TO_ENCRYPT:
    case SUIT_ERR_FAILED_TO_DECRYPT:
    case SUIT_ERR_FAILED_TO_SIGN:
        return SUIT_REPORT_REASON_OPERATION_FAILED;

    case SUIT_ERR_CONDITION_MISMATCH:
    case SUIT_ERR_TRY_OUT:
        return SUIT_REPORT_REASON_CONDITION_FAILED;

    case SUIT_ERR_ABORT:
    case SUIT_SUCCESS:
    default:
        // should not reach here
        return SUIT_REPORT_REASON_OK;
    }
}

suit_err_t suit_report_result(
    suit_report_context_t *report_context,
    const suit_err_t final_state,
    suit_report_reason_t reason,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value)
{
    if (final_state == SUIT_ERR_ABORT) {
        return SUIT_ERR_WHILE_REPORTING;
    }

    if (suit_report_stop_records(report_context) != SUIT_SUCCESS) {
        return SUIT_ERR_WHILE_REPORTING;
    }

    if (final_state == SUIT_SUCCESS) {
        // suit-report-result => true
        QCBOREncode_AddBoolToMapN(&report_context->cbor_encoder, SUIT_REPORT_RESULT, true);
        return SUIT_SUCCESS;
    }

    // need to encode error reason
    // suit-report-result => { start
    QCBOREncode_OpenMapInMapN(&report_context->cbor_encoder, SUIT_REPORT_RESULT);
    {
        // suit-report-result-code => int,
        QCBOREncode_AddInt64ToMapN(&report_context->cbor_encoder, SUIT_REPORT_RESULT_CODE, final_state);

        // suit-report-result-record => SUIT_Record
        suit_encode_append_suit_record(
            &report_context->cbor_encoder,
            SUIT_REPORT_RESULT_RECORD,
            dependency_tree,
            manifest_key,
            section_offset,
            parameter_keys,
            parameter_value
        );

        // suit-report-result-reason => SUIT_Report_Reasons
        if (reason == SUIT_REPORT_REASON_OK) {
            // try to estimate it from suit_err_t result
            reason = suit_report_reason_from_suit_err(final_state);
        }
        QCBOREncode_AddInt64ToMapN(&report_context->cbor_encoder, SUIT_REPORT_RESULT_REASON, reason);
    }
    QCBOREncode_CloseMap(&report_context->cbor_encoder);
    // } suit-report-result end

    return SUIT_SUCCESS;
}

suit_err_t suit_report_finalize(
    suit_report_context_t *report_context,
    const suit_err_t final_state,
    suit_report_reason_t reason,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value)
{
    suit_err_t result = SUIT_SUCCESS;
    UsefulBufC payload;

    // 1. stop encoding suit-report-records
    if (suit_report_stop_records(report_context) != SUIT_SUCCESS) {
        return SUIT_ERR_WHILE_REPORTING;
    }
    if (report_context->state != SUIT_REPORTING_ENGINE_RECORDING_STOPPED) {
        return SUIT_ERR_WHILE_REPORTING;
    }

    // 2. encode suit-report-result
    result = suit_report_result(
        report_context,
        final_state,
        reason,
        dependency_tree,
        manifest_key,
        section_offset,
        parameter_keys,
        parameter_value
    );
    if (result != SUIT_SUCCESS) {
        return result;
    }

    // 3. stop encoding SUIT_Report
    QCBOREncode_CloseMap(&report_context->cbor_encoder);

    // 4. sign, calculate MAC (not supported yet) or encrypt (not supported yet)
    switch (report_context->cose_protection_mechanism) {
    case 0:
        // raw SUIT_Report, do nothing here
        break;
    case CBOR_TAG_COSE_SIGN1:
        // use t_cose two-step sign to save memory
        QCBOREncode_CloseBstrWrap2(&report_context->cbor_encoder, false, &payload);
        enum t_cose_err_t t_cose_error = t_cose_sign_encode_finish(
            &report_context->sign_ctx,
            NULL_Q_USEFUL_BUF_C,
            payload,
            &report_context->cbor_encoder);
        if (t_cose_error != T_COSE_SUCCESS) {
            return SUIT_ERR_WHILE_REPORTING;
        }
        break;
    default:
        return SUIT_ERR_WHILE_REPORTING;
    }

    // 5. encode Protected_SUIT_Report or Unprotected_SUIT_Report to memory
    QCBORError cbor_error = QCBOREncode_Finish(&report_context->cbor_encoder, &report_context->suit_report);
    if (cbor_error != QCBOR_SUCCESS) {
        return SUIT_ERR_WHILE_REPORTING;
    }

    report_context->state = SUIT_REPORTING_ENGINE_FINALIZED;
    return SUIT_SUCCESS;
}

suit_err_t suit_report_extend_record(
    suit_report_context_t *report_context,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value)
{
    switch (report_context->state) {
    case SUIT_REPORTING_ENGINE_IN_SYSTEM_PROPERTY_CLAIMS:
        // close map for
        // system-property-claims = {
        //   system-component-id => SUIT_Component_Identifier,
        //   + $$SUIT_Parameters,
        // } <- this one
        QCBOREncode_CloseMap(&report_context->cbor_encoder);
        // fall through
    case SUIT_REPORTING_ENGINE_IN_REPORT_RECORD:
        // do nothing
        break;
    default:
        return SUIT_ERR_WHILE_REPORTING;
    }

    suit_encode_append_suit_record(
        &report_context->cbor_encoder,
        0,
        dependency_tree,
        manifest_key,
        section_offset,
        parameter_keys,
        parameter_value
    );

    report_context->state = SUIT_REPORTING_ENGINE_IN_REPORT_RECORD;
    return SUIT_SUCCESS;
}

suit_err_t suit_report_extend_system_property_claims(
    suit_report_context_t *report_context,
    const uint8_t component_index,
    const suit_component_identifier_t *component,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value)
{
    // ignore if no valid parameter found
    size_t i = 0;
    for (; i < LIBCSUIT_MAX_REPORT_PRAMETER_NUM; i++) {
        if (parameter_keys[i] != SUIT_PARAMETER_INVALID) {
            break;
        }
    }
    if (i == LIBCSUIT_MAX_REPORT_PRAMETER_NUM) {
        return SUIT_SUCCESS;
    }

    switch (report_context->state) {
    case SUIT_REPORTING_ENGINE_IN_SYSTEM_PROPERTY_CLAIMS:
        if (report_context->current_index != component_index) {
            // need to close the system-property-claims
            QCBOREncode_CloseMap(&report_context->cbor_encoder);

            // open the map and start encoding system-property-claims for new component
            QCBOREncode_OpenMap(&report_context->cbor_encoder);
            //   system-component-id
            QCBOREncode_AddUInt64(&report_context->cbor_encoder, 0);
            //   => SUIT_Component_Identifier,
            suit_encode_append_component_identifier(component, 0, &report_context->cbor_encoder);
            report_context->current_index = component_index;
        }
        break;
    case SUIT_REPORTING_ENGINE_IN_REPORT_RECORD:
        // open the map and start encoding system-property-claims for new component
        QCBOREncode_OpenMap(&report_context->cbor_encoder);
        //   system-component-id
        QCBOREncode_AddUInt64(&report_context->cbor_encoder, 0);
        //   => SUIT_Component_Identifier,
        suit_encode_append_component_identifier(component, 0, &report_context->cbor_encoder);
        report_context->current_index = component_index;
        break;
    default:
        return SUIT_ERR_WHILE_REPORTING;
    }

    // append the parameter
    for (size_t i = 0; i < LIBCSUIT_MAX_REPORT_PRAMETER_NUM; i++) {
        suit_report_append_parameter(&report_context->cbor_encoder, parameter_keys[i], parameter_value);
    }
    report_context->state = SUIT_REPORTING_ENGINE_IN_SYSTEM_PROPERTY_CLAIMS;
    return SUIT_SUCCESS;
}

suit_err_t suit_report_manifest_reference_uri(
    suit_report_context_t *report_context,
    UsefulBufC reference_uri)
{
    switch (report_context->state) {
    case SUIT_REPORTING_ENGINE_SUIT_DIGEST_STORED:
        // ok
        break;
    case SUIT_REPORTING_ENGINE_IN_REPORT_RECORD:
        // called twice, ignore
        return SUIT_SUCCESS;
    default:
        return SUIT_ERR_WHILE_REPORTING;
    }

    
    // SUIT_Reference = [
    //   suit-report-manifest-uri: tstr,
    //   suit-report-manifest-digest: SUIT_Digest
    // ]
    QCBOREncode_OpenArrayInMapN(&report_context->cbor_encoder, SUIT_REPORT_REFERENCE);
    QCBOREncode_AddText(&report_context->cbor_encoder, reference_uri);
    suit_encode_append_digest(&report_context->digest, 0, &report_context->cbor_encoder);
    QCBOREncode_CloseArray(&report_context->cbor_encoder);

    // suit-report-records => [
    QCBOREncode_OpenArrayInMapN(&report_context->cbor_encoder, SUIT_REPORT_RECORDS);
    report_context->state = SUIT_REPORTING_ENGINE_IN_REPORT_RECORD;

    return SUIT_SUCCESS;
}

suit_err_t suit_report_manifest_digest(
    suit_report_context_t *report_context,
    suit_digest_t digest)
{
    if (report_context->state != SUIT_REPORTING_ENGINE_STARTED) {
        return SUIT_ERR_WHILE_REPORTING;
    }
    report_context->digest = digest;
    report_context->state = SUIT_REPORTING_ENGINE_SUIT_DIGEST_STORED;

    return SUIT_SUCCESS;
}

/*
    Public function. See suit_reporting_engine.h
 */
suit_err_t suit_report_start_encoding(
    suit_report_context_t *report_context,
    UsefulBufC nonce)
{
    // SUIT_Report start {
    QCBOREncode_OpenMap(&report_context->cbor_encoder);
    // ? suit-report-nonce         => bstr,
    if (!UsefulBuf_IsNULLOrEmptyC(nonce)) {
        QCBOREncode_AddBytesToMapN(&report_context->cbor_encoder, SUIT_REPORT_NONCE, nonce);
    }

    // TODO: encode * SUIT_Capability_Report

    report_context->state = SUIT_REPORTING_ENGINE_STARTED;

    return SUIT_SUCCESS;
}

/*
    Public function. See suit_reporting_engine.h
 */
suit_err_t suit_report_add_sender_key(
    suit_report_context_t *report_context,
    const int cose_tag,
    int cose_algorithm_id,
    suit_key_t *sender_key)
{
    switch (report_context->state) {
    case SUIT_REPORTING_ENGINE_NOT_INITIALIZED:
        return SUIT_ERR_NOT_INITIALIZED;
    case SUIT_REPORTING_ENGINE_KEY_LOADED:
        return SUIT_ERR_INITIALIZED_AGAIN;
    case SUIT_REPORTING_ENGINE_INITIALIZED:
        break;
    default:
        return SUIT_ERR_WHILE_REPORTING;
    }

    if (sender_key->cose_algorithm_id != T_COSE_ALGORITHM_RESERVED) {
        // the algorithm id is overwritten by the key
        cose_algorithm_id = sender_key->cose_algorithm_id;
    }
    if (cose_algorithm_id == T_COSE_ALGORITHM_RESERVED) {
        return SUIT_ERR_WHILE_REPORTING;
    }

    switch (cose_tag) {
    case CBOR_TAG_COSE_SIGN1:
        // use t_cose two-step sign to save memory
        t_cose_sign_sign_init(&report_context->sign_ctx,
                              T_COSE_OPT_MESSAGE_TYPE_SIGN1);
        t_cose_signature_sign_main_init(&report_context->signer, cose_algorithm_id);
        t_cose_signature_sign_main_set_signing_key(&report_context->signer,
                                                    sender_key->cose_key,
                                                    sender_key->kid);
        t_cose_sign_add_signer(&report_context->sign_ctx,
                                t_cose_signature_sign_from_main(&report_context->signer));
        enum t_cose_err_t t_cose_error = 
            t_cose_sign_encode_start(&report_context->sign_ctx,
                                     &report_context->cbor_encoder);
        if (t_cose_error != T_COSE_SUCCESS) {
            return SUIT_ERR_WHILE_REPORTING;
        }
        QCBOREncode_BstrWrap(&report_context->cbor_encoder);
        break;
    case CBOR_TAG_COSE_MAC0:
    case CBOR_TAG_COSE_ENCRYPT:
        return SUIT_ERR_NOT_IMPLEMENTED;
    case CBOR_TAG_COSE_SIGN:
    case CBOR_TAG_COSE_MAC:
    default:
        return SUIT_ERR_INVALID_VALUE;
    }

    report_context->cose_protection_mechanism = cose_tag;

    report_context->state = SUIT_REPORTING_ENGINE_KEY_LOADED;

    return SUIT_SUCCESS;
}

/*
    Public function. See suit_reporting_engine.h
 */
suit_err_t suit_report_init_engine(
    suit_report_context_t *report_context,
    const size_t buf_size)
{
    report_context->state = SUIT_REPORTING_ENGINE_NOT_INITIALIZED;

    QCBOREncode_Init(&report_context->cbor_encoder, (UsefulBuf){.ptr = report_context->buf, .len = buf_size});

    report_context->state = SUIT_REPORTING_ENGINE_INITIALIZED;

    return SUIT_SUCCESS;
}