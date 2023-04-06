/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process.c

    \brief  This implements libcsuit processing

    Call suit_process_envelopes() to process whole SUIT manifests at once.
    One or more manifests may depend other manifests.
 */

#include "qcbor/qcbor.h"
#include "qcbor/qcbor_spiffy_decode.h"
#include "csuit/suit_common.h"
#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_print.h"

suit_err_t suit_set_parameters(QCBORDecodeContext *context,
                               const suit_con_dir_key_t directive,
                               const suit_index_t *suit_index,
                               suit_parameter_args_t parameters[])
{
    suit_err_t result = SUIT_SUCCESS;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    QCBORDecode_EnterMap(context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }
    suit_parameter_key_t parameter;

    union {
        uint64_t u64;
        int64_t i64;
        UsefulBufC str;
        bool b;
        suit_digest_t digest;
    } val;

    size_t length = item.val.uCount;
    for (size_t i = 0; i < length; i++) {
        QCBORDecode_PeekNext(context, &item);
        if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto error;
        }
        parameter = item.label.int64;

        switch (parameter) {
        /* int64 */
        /* uint64 */
        case SUIT_PARAMETER_IMAGE_SIZE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_SIZE;
                    parameters[tmp_index].image_size = val.u64;
                }
            }
            break;
        case SUIT_PARAMETER_SOURCE_COMPONENT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT;
                    parameters[tmp_index].source_component = val.u64;
                }
            }
            break;
        case SUIT_PARAMETER_COMPONENT_SLOT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT;
                    parameters[tmp_index].component_slot = val.u64;
                }
            }
            break;
        case SUIT_PARAMETER_USE_BEFORE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_USE_BEFORE;
                    parameters[tmp_index].use_before = val.u64;
                }
            }
            break;
        case SUIT_PARAMETER_MINIMUM_BATTERY:
             QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY;
                    parameters[tmp_index].minimum_battery = val.u64;
                }
            }
            break;
        case SUIT_PARAMETER_UPDATE_PRIORITY:
             QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY;
                    parameters[tmp_index].update_priority = val.u64;
                }
            }
            break;

        /* bool */
        case SUIT_PARAMETER_SOFT_FAILURE:
            QCBORDecode_GetBool(context, &val.b);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_SOFT_FAILURE) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOFT_FAILURE;
                    parameters[tmp_index].soft_failure = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
                }
            }
            break;
        case SUIT_PARAMETER_STRICT_ORDER:
            QCBORDecode_GetBool(context, &val.b);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_STRICT_ORDER) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_STRICT_ORDER;
                    parameters[tmp_index].strict_order = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
                }
            }
            break;

        /* tstr */
        case SUIT_PARAMETER_URI:
            QCBORDecode_GetTextString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_URI) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_URI;
                    parameters[tmp_index].uri = val.str;
                }
            }
            break;

        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER;
                    parameters[tmp_index].vendor_id = val.str;
                }
            }
            break;
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER;
                    parameters[tmp_index].class_id = val.str;
                }
            }
            break;
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER;
                    parameters[tmp_index].device_id = val.str;
                }
            }
            break;
        case SUIT_PARAMETER_CONTENT:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CONTENT;
                    parameters[tmp_index].content = val.str;
                }
            }
            break;
        case SUIT_PARAMETER_INVOKE_ARGS:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_INVOKE_ARGS) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_INVOKE_ARGS;
                    parameters[tmp_index].invoke_args = val.str;
                }
            }
            break;

        /* .bstr COSE_Encrypt0 // .bstr COSE_Encrypt */
        case SUIT_PARAMETER_ENCRYPTION_INFO:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO;
                    parameters[tmp_index].encryption_info = val.str;
                }
            }
            break;

        /* .bstr suit-diget */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, context, &item, true, &val.digest);
            QCBORDecode_ExitBstrWrapped(context);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST) || directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST;
                    parameters[tmp_index].image_digest = val.digest;
                }
            }
            break;

        case SUIT_PARAMETER_VERSION:
        case SUIT_PARAMETER_WAIT_INFO:
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            goto error;
        }
        error = QCBORDecode_GetError(context);
        if (error != QCBOR_SUCCESS) {
            goto error;
        }
    }

    QCBORDecode_ExitMap(context);
    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = SUIT_COMMON,
                .level2.common_key = SUIT_SHARED_SEQUENCE,
                .level3.condition_directive = directive,
                .level4.parameter = parameter,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;
}

const suit_component_identifier_t *suit_index_to_component_identifier(const suit_extracted_t *extracted,
                                                                const uint8_t index)
{
    for (size_t i = 0; i < extracted->components_len; i++) {
        if (extracted->components[i].index == index) {
            if (extracted->components[i].component.len == 0) {
                return NULL;
            }
            return &extracted->components[i].component;
        }
    }
    return NULL;
}

suit_payload_t* suit_index_to_payload(suit_extracted_t *extracted,
                                      const uint8_t index)
{
    for (size_t i = 0; i < extracted->payloads.len; i++) {
        if (extracted->payloads.payload[i].index == index) {
            return &extracted->payloads.payload[i];
        }
    }
    return NULL;
}

suit_payload_t* suit_key_to_payload(suit_extracted_t *extracted,
                                    UsefulBufC key)
{
    for (size_t i = 0; i < extracted->payloads.len; i++) {
        if (extracted->payloads.payload[i].key.len != key.len) {
            continue;
        }
        if (extracted->payloads.payload[i].key.ptr == key.ptr) {
            return &extracted->payloads.payload[i];
        }
        else if (memcmp(extracted->payloads.payload[i].key.ptr, key.ptr, key.len) == 0) {
            return &extracted->payloads.payload[i];
        }
    }
    return NULL;
}

suit_err_t suit_set_index(QCBORDecodeContext *context,
                          const suit_extracted_t *extracted,
                          suit_index_t *suit_index)
{
    union {
        uint64_t u64;
        bool b;
    } val;

    *suit_index = (suit_index_t) {0};

    QCBORItem item;
    QCBORError error;
    QCBORDecode_PeekNext(context, &item);
    switch (item.uDataType) {
    case QCBOR_TYPE_UINT64:
    case QCBOR_TYPE_INT64:
        QCBORDecode_GetUInt64(context, &val.u64);
        if (val.u64 > UINT8_MAX) {
            return SUIT_ERR_INVALID_TYPE_OF_VALUE;
        }
        suit_index->index[0] = (uint8_t)val.u64;
        suit_index->len = 1;
        break;
    case QCBOR_TYPE_TRUE:
        QCBORDecode_GetBool(context, &val.b);
        for (size_t i = 0; i < extracted->components_len; i++) {
            suit_index->index[i] = extracted->components[i].index;
        }
        suit_index->len = extracted->components_len;
        break;
    case QCBOR_TYPE_ARRAY:
        QCBORDecode_EnterArray(context, &item);
        if (item.val.uCount > SUIT_MAX_INDEX_NUM) {
            return SUIT_ERR_NO_MEMORY;
        }
        suit_index->len = item.val.uCount;
        for (size_t i = 0; i < suit_index->len; i++) {
            QCBORDecode_GetUInt64(context, &val.u64);
            if (val.u64 > UINT8_MAX) {
                return SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            suit_index->index[i] = (uint8_t)val.u64;
        }
        QCBORDecode_ExitArray(context);
    default:
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    return SUIT_SUCCESS;
}

suit_process_flag_t suit_manifest_key_to_process_flag(const suit_manifest_key_t key)
{
    suit_process_flag_t result = {0};
    switch (key) {
    case SUIT_REFERENCE_URI:
        result.reference_uri = 1;
        break;
    case SUIT_VALIDATE:
        result.validate = 1;
        break;
    case SUIT_LOAD:
        result.load = 1;
        break;
    case SUIT_INVOKE:
        result.invoke = 1;
        break;
    case SUIT_PAYLOAD_FETCH:
        result.payload_fetch = 1;
        break;
    case SUIT_INSTALL:
        result.install = 1;
        break;
    case SUIT_TEXT:
        result.text = 1;
        break;
    case SUIT_DEPENDENCY_RESOLUTION:
        result.dependency_resolution = 1;
        break;
    case SUIT_UNINSTALL:
        result.uninstall = 1;
        break;
    default:
        break;
    }
    return result;
}

suit_err_t suit_process_fetch(suit_extracted_t *extracted,
                              const suit_parameter_args_t parameters[],
                              const suit_rep_policy_t report,
                              const suit_index_t *suit_index,
                              suit_inputs_t *suit_inputs)
{
    suit_err_t result = SUIT_SUCCESS;
    for (size_t i = 0; i < suit_index->len; i++) {
        const uint8_t tmp_index = suit_index->index[i];
        if (parameters[tmp_index].uri.len >= SUIT_MAX_NAME_LENGTH) {
            return SUIT_ERR_NO_MEMORY;
        }
        const suit_component_identifier_t *dst = suit_index_to_component_identifier(extracted, tmp_index);
        if (dst == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }

        suit_payload_t *payload = suit_key_to_payload(extracted, parameters[tmp_index].uri);
        if (payload == NULL) {
            if (extracted->payloads.len >= SUIT_MAX_ARRAY_LENGTH) {
                return SUIT_ERR_NO_MEMORY;
            }
            size_t buf_size = (parameters[tmp_index].image_size > 0) ? parameters[tmp_index].image_size : suit_inputs->left_len;
            if (buf_size > suit_inputs->left_len) {
                return SUIT_ERR_NO_MEMORY;
            }

            suit_fetch_args_t fetch = {0};
            suit_fetch_ret_t fret = {0};
            fetch.dst = *dst;

            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                fetch.encryption_info = parameters[tmp_index].encryption_info;
                memcpy(fetch.mechanisms, suit_inputs->mechanisms, sizeof(fetch.mechanisms));
                if (sizeof(fetch.mechanisms) != sizeof(suit_mechanism_t) * 4) {
                    return SUIT_ERR_FATAL;
                }
            }

            memcpy(fetch.uri, parameters[tmp_index].uri.ptr, parameters[tmp_index].uri.len);
            fetch.uri[parameters[tmp_index].uri.len] = '\0';
            fetch.uri_len = parameters[tmp_index].uri.len;

            fetch.args = parameters[tmp_index].fetch_arguments;

            fetch.buf_len = buf_size;
            fetch.ptr = suit_inputs->ptr + (SUIT_MAX_DATA_SIZE - suit_inputs->left_len);

            fetch.report = report;

            /* store the fetched payload into fetch.ptr */
            result = suit_fetch_callback(fetch, &fret);
            if (result != SUIT_SUCCESS) {
                return result;
            }
            if (fetch.buf_len < fret.buf_len) {
                return SUIT_ERR_NO_MEMORY;
            }
            suit_inputs->left_len -= fret.buf_len;

            /* register fetched payload */
            payload = &extracted->payloads.payload[extracted->payloads.len];
            extracted->payloads.len++;
            payload->bytes.ptr = fetch.ptr;
            payload->bytes.len = fret.buf_len;

            payload->key = parameters[tmp_index].uri;
            payload->index = tmp_index;
        }
        else {
            /* already handled with integrated-payload or integrated-dependency */
            suit_store_args_t store = {0};
            store.report = report;
            store.dst = *dst;
            store.src_buf = payload->bytes;
            store.operation = SUIT_STORE;

            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                store.encryption_info = parameters[tmp_index].encryption_info;
                memcpy(store.mechanisms, suit_inputs->mechanisms, sizeof(store.mechanisms));
                if (sizeof(store.mechanisms) != sizeof(suit_mechanism_t) * 4) {
                    return SUIT_ERR_FATAL;
                }
            }
            result = suit_store_callback(store);
            if (result != SUIT_SUCCESS) {
                return result;
            }
            payload->key = parameters[tmp_index].uri;
            payload->index = tmp_index;
        }
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_process_invoke(const suit_extracted_t *extracted,
                               const suit_parameter_args_t parameters[],
                               const suit_rep_policy_t report,
                               const suit_index_t *suit_index)
{
    suit_err_t result;
    for (size_t j = 0; j < suit_index->len; j++) {
        const uint8_t tmp_index = suit_index->index[j];

        const suit_component_identifier_t *component = suit_index_to_component_identifier(extracted, tmp_index);
        if (component == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }

        suit_invoke_args_t invoke = {0};
        invoke.report = report;
        invoke.component_identifier = *component;
        invoke.args_len = parameters[tmp_index].invoke_args.len;
        if (invoke.args_len > 0) {
            memcpy(invoke.args, parameters[tmp_index].invoke_args.ptr, invoke.args_len);
        }
        result = suit_invoke_callback(invoke);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_process_condition(suit_extracted_t *extracted,
                                  suit_con_dir_key_t condition,
                                  suit_parameter_args_t parameters[],
                                  const suit_index_t *suit_index,
                                  suit_rep_policy_t report)
{
    suit_err_t result = SUIT_SUCCESS;

    for (uint8_t i = 0; i < suit_index->len; i++) {
        uint8_t tmp_index = suit_index->index[i];

        suit_condition_args_t args = {0};
        const suit_component_identifier_t *dst = suit_index_to_component_identifier(extracted, tmp_index);
        if (dst == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        args.dst = *dst;
        args.condition = condition;
        switch (condition) {
        /* uint64 */
        case SUIT_CONDITION_COMPONENT_SLOT:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) {
                args.expected.u64 = parameters[tmp_index].component_slot;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_USE_BEFORE:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) {
                args.expected.u64 = parameters[tmp_index].use_before;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_MINIMUM_BATTERY:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) {
                args.expected.u64 = parameters[tmp_index].minimum_battery;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) {
                args.expected.u64 = parameters[tmp_index].update_priority;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

        /* bstr */
        case SUIT_CONDITION_VENDOR_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].vendor_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_CLASS_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].class_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].device_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
        case SUIT_CONDITION_CHECK_CONTENT:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) {
                args.expected.str = parameters[tmp_index].content;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

        /* suit-digest */
        case SUIT_CONDITION_IMAGE_MATCH:
        case SUIT_CONDITION_IMAGE_NOT_MATCH:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE) {
                args.expected.image_size = parameters[tmp_index].image_size;
            }
            else {
                args.expected.image_size = 0; /* any size is ok */
            }
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST) {
                args.expected.image_digest = parameters[tmp_index].image_digest;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

        case SUIT_CONDITION_ABORT:
            break;

        case SUIT_CONDITION_IS_DEPENDENCY:
            result = (tmp_index > SUIT_MAX_INDEX_NUM) ? SUIT_SUCCESS : SUIT_ERR_ABORT;
            break;

        /* draft-ietf-suit-trust-domains */
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
        case SUIT_CONDITION_VERSION:
            break;
        default:
            return SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            /* already handled without callback */
            return result;
        }
        result = suit_condition_callback(args);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return result;
}

suit_err_t suit_process_write(const suit_extracted_t *extracted,
                              const suit_parameter_args_t parameters[],
                              const suit_rep_policy_t report,
                              const suit_index_t *suit_index,
                              suit_inputs_t *suit_inputs)
{
    for (size_t i = 0; i < suit_index->len; i++) {
        const uint8_t tmp_index = suit_index->index[i];

        suit_store_args_t store = {0};
        if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT)) {
            return SUIT_ERR_FATAL;
        }
        store.src_buf = parameters[tmp_index].content;
        if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
            store.encryption_info = parameters[tmp_index].encryption_info;

            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                store.encryption_info = parameters[tmp_index].encryption_info;
                memcpy(store.mechanisms, suit_inputs->mechanisms, sizeof(store.mechanisms));
                if (sizeof(store.mechanisms) != sizeof(suit_mechanism_t) * 4) {
                    return SUIT_ERR_FATAL;
                }
            }
        }
        store.report = report;
        const suit_component_identifier_t *dst = suit_index_to_component_identifier(extracted, tmp_index);
        if (dst == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        store.dst = *dst;
        store.operation = SUIT_STORE;
        suit_err_t result = suit_store_callback(store);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_process_copy_swap(const suit_extracted_t *extracted,
                                  const suit_parameter_args_t parameters[],
                                  const suit_rep_policy_t report,
                                  const suit_index_t *suit_index,
                                  bool is_swap,
                                  suit_inputs_t *suit_inputs)
{
    for (size_t i = 0; i < suit_index->len; i++) {
        const uint8_t tmp_index = suit_index->index[i];

        suit_store_args_t store = {0};
        store.report = report;

        if (is_swap) {
            store.operation = SUIT_SWAP;
        }
        else {
            store.operation = SUIT_COPY;
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                store.encryption_info = parameters[tmp_index].encryption_info;
                for (size_t j = 0; j < SUIT_MAX_KEY_NUM; j++) {
                    if (suit_inputs->mechanisms[j].use &&
                        (suit_inputs->mechanisms[j].cose_tag == CBOR_TAG_COSE_ENCRYPT ||
                         suit_inputs->mechanisms[j].cose_tag == CBOR_TAG_COSE_ENCRYPT0)) {
                        store.mechanisms[j] = suit_inputs->mechanisms[j];
                    }
                }
            }
        }

        const suit_component_identifier_t *tmp;
        tmp = suit_index_to_component_identifier(extracted, parameters[tmp_index].source_component);
        if (tmp == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        store.src = *tmp;
        tmp = suit_index_to_component_identifier(extracted, tmp_index);
        if (tmp == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        store.dst = *tmp;
        suit_err_t result = suit_store_callback(store);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}

suit_err_t suit_process_unlink(const suit_extracted_t *extracted,
                               const suit_parameter_args_t parameters[],
                               const suit_rep_policy_t report,
                               const suit_index_t *suit_index,
                               suit_inputs_t *suit_inputs)
{

    for (size_t i = 0; i < suit_index->len; i++) {
        const uint8_t tmp_index = suit_index->index[i];
        const suit_component_identifier_t *dst = suit_index_to_component_identifier(extracted, tmp_index);
        if (dst == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        suit_store_args_t store = {0};
        store.report = report;
        store.dst = *dst;
        store.operation = SUIT_UNLINK;
        suit_err_t result = suit_store_callback(store);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}


suit_err_t suit_process_command_sequence_buf(suit_extracted_t *extracted,
                                             const suit_manifest_key_t command_key,
                                             UsefulBufC buf,
                                             suit_parameter_args_t parameters[],
                                             suit_inputs_t *suit_inputs)
{
    suit_err_t result = SUIT_SUCCESS;
    suit_index_t suit_index = {.len = 1, .index[0] = 0};
    suit_con_dir_key_t condition_directive_key = SUIT_CONDITION_INVALID;
    suit_rep_policy_t report;
    union {
        uint64_t u64;
        int64_t i64;
        UsefulBufC buf;
        bool b;
    } val;

    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    QCBORDecode_Init(&context, buf, QCBOR_DECODE_MODE_NORMAL);

    QCBORDecode_EnterArray(&context, &item);
    const size_t length = item.val.uCount;
    if (length % 2 != 0) {
        result = SUIT_ERR_NO_MORE_ITEMS;
        goto error;
    }
    for (size_t i = 0; i < length; i += 2) {
        result = suit_qcbor_get_next_uint(&context, &item);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
        condition_directive_key = item.val.uint64;

        switch (condition_directive_key) {
        case SUIT_DIRECTIVE_SET_COMPONENT_INDEX:
            result = suit_set_index(&context, extracted, &suit_index);
            break;

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, &suit_index, parameters);
            break;
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_SET_PARAMETERS, &suit_index, parameters);
            break;

        case SUIT_DIRECTIVE_INVOKE:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_invoke(extracted, parameters, report, &suit_index);
            break;

        case SUIT_DIRECTIVE_FETCH:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_fetch(extracted, parameters, report, &suit_index, suit_inputs);
            break;

        case SUIT_DIRECTIVE_WRITE:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_write(extracted, parameters, report, &suit_index, suit_inputs);
            break;
        case SUIT_DIRECTIVE_COPY:
            QCBORDecode_GetUInt64(&context, &val.u64);
            result = suit_process_copy_swap(extracted, parameters, report, &suit_index, false, suit_inputs);
            break;
        case SUIT_DIRECTIVE_SWAP:
            QCBORDecode_GetUInt64(&context, &val.u64);
            result = suit_process_copy_swap(extracted, parameters, report, &suit_index, true, suit_inputs);
            break;
        case SUIT_DIRECTIVE_UNLINK:
            QCBORDecode_GetUInt64(&context, &val.u64);
            result = suit_process_unlink(extracted, parameters, report, &suit_index, suit_inputs);
            break;

        case SUIT_DIRECTIVE_TRY_EACH:
            QCBORDecode_EnterArray(&context, &item);
            if (item.uDataType != QCBOR_TYPE_ARRAY) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto error;
            }
            const size_t try_count = item.val.uCount;
            bool orig_soft_failures[SUIT_MAX_INDEX_NUM];
            for (size_t j = 0; j < SUIT_MAX_INDEX_NUM; j++) {
                orig_soft_failures[j] = parameters[j].soft_failure;
            }
            bool done = false;
            for (size_t j = 0; j < try_count; j++) {
                QCBORDecode_GetNext(&context, &item);
                if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                    if (!done) {
                        result = suit_process_command_sequence_buf(extracted, SUIT_COMMON, item.val.string, parameters, NULL);
                        if (result == SUIT_SUCCESS) {
                            done = true;
                        }
                    }
                }
                else if (item.uDataType == QCBOR_TYPE_NULL && j + 1 == try_count) {
                    /* continue without error, see #8.7.7.3 */
                    done = true;
                    break;
                }
                else {
                    result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                    goto error;
                }
            }
            for (size_t j = 0; j < SUIT_MAX_INDEX_NUM; j++) {
                parameters[j].soft_failure = orig_soft_failures[j];
            }
            QCBORDecode_ExitArray(&context);
            if (!done) {
                result = SUIT_ERR_TRY_OUT;
                goto error;
            }
            break;

        case SUIT_DIRECTIVE_PROCESS_DEPENDENCY:
            QCBORDecode_GetUInt64(&context, &val.u64);
            for (size_t j = 0; j < suit_index.len; j++) {
                const uint8_t tmp_index = suit_index.index[j];

                suit_inputs_t tmp_inputs = *suit_inputs;
                tmp_inputs.process_flags = suit_manifest_key_to_process_flag(command_key);
                tmp_inputs.dependency_depth++;
                suit_payload_t *payload = suit_index_to_payload(extracted, tmp_index);
                if (payload == NULL) {
                    return SUIT_ERR_NOT_FOUND;
                }
                tmp_inputs.manifest = payload->bytes;
                result = suit_process_envelope(&tmp_inputs);
                if (result != SUIT_SUCCESS) {
                    break;
                }
            }
            break;
        case SUIT_CONDITION_VENDOR_IDENTIFIER:
        case SUIT_CONDITION_CLASS_IDENTIFIER:
        case SUIT_CONDITION_IMAGE_MATCH:
        case SUIT_CONDITION_USE_BEFORE:
        case SUIT_CONDITION_COMPONENT_SLOT:
        case SUIT_CONDITION_ABORT:
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
        case SUIT_CONDITION_IMAGE_NOT_MATCH:
        case SUIT_CONDITION_MINIMUM_BATTERY:
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
        case SUIT_CONDITION_VERSION:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_condition(extracted, condition_directive_key, parameters, &suit_index, report);
            break;

        case SUIT_DIRECTIVE_WAIT:
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            goto error;
        }
        error = QCBORDecode_GetError(&context);
        if (error != QCBOR_SUCCESS) {
            goto error;
        }
    }
    QCBORDecode_ExitArray(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = command_key,
                .level2.condition_directive = condition_directive_key,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result,
                .report = report
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;

}

suit_index_t suit_index = {.len = 1, .index[0] = 0};
suit_err_t suit_process_shared_sequence(suit_extracted_t *extracted,
                                        suit_parameter_args_t parameters[])
{
    if (extracted->shared_sequence.len == 0) {
        return SUIT_SUCCESS;
    }
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_con_dir_key_t condition_directive_key;
    suit_index_t suit_index = (suit_index_t) {.len = 1, .index[0] = 0};
    suit_rep_policy_t report;
    union {
        uint64_t u64;
        int64_t i64;
        UsefulBufC buf;
        bool b;
    } val;

    QCBORDecode_Init(&context, extracted->shared_sequence, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterArray(&context, &item);
    if (item.uDataType != QCBOR_TYPE_ARRAY) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }
    size_t length = item.val.uCount;
    if (length % 2 != 0) {
        result = SUIT_ERR_NO_MORE_ITEMS;
        goto error;
    }
    for (size_t i = 0; i < length; i += 2) {
        result = suit_qcbor_get_next_uint(&context, &item);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
        condition_directive_key = item.val.uint64;
        switch (condition_directive_key) {
        case SUIT_DIRECTIVE_SET_COMPONENT_INDEX:
            // TODO: support also bool or [ + uint ] index
            QCBORDecode_GetUInt64(&context, &val.u64);
            if (val.u64 >= extracted->components_len) {
                result = SUIT_ERR_NO_MEMORY;
                goto error;
            }
            if (val.u64 > UINT8_MAX) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto error;
            }
            suit_index.len = 1;
            suit_index.index[0] = (uint8_t)val.u64;
            break;

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, &suit_index, parameters);
            break;
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_SET_PARAMETERS, &suit_index, parameters);
            break;

        case SUIT_CONDITION_VENDOR_IDENTIFIER:
        case SUIT_CONDITION_CLASS_IDENTIFIER:
        case SUIT_CONDITION_IMAGE_MATCH:
        case SUIT_CONDITION_USE_BEFORE:
        case SUIT_CONDITION_COMPONENT_SLOT:
        case SUIT_CONDITION_ABORT:
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
        case SUIT_CONDITION_IMAGE_NOT_MATCH:
        case SUIT_CONDITION_MINIMUM_BATTERY:
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
        case SUIT_CONDITION_VERSION:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_condition(extracted, condition_directive_key, parameters, &suit_index, report);
            break;
        case SUIT_DIRECTIVE_TRY_EACH:
            QCBORDecode_EnterArray(&context, &item);
            if (item.uDataType != QCBOR_TYPE_ARRAY) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto error;
            }
            const size_t try_count = item.val.uCount;
            bool orig_soft_failures[SUIT_MAX_INDEX_NUM];
            for (size_t j = 0; j < SUIT_MAX_INDEX_NUM; j++) {
                orig_soft_failures[j] = parameters[j].soft_failure;
            }
            bool done = false;
            for (size_t j = 0; j < try_count; j++) {
                QCBORDecode_GetNext(&context, &item);
                if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                    if (!done) {
                        result = suit_process_command_sequence_buf(extracted, SUIT_COMMON, item.val.string, parameters, NULL);
                        if (result == SUIT_SUCCESS) {
                            done = true;
                        }
                    }
                }
                else if (item.uDataType == QCBOR_TYPE_NULL && j + 1 == try_count) {
                    /* continue without error, see #8.7.7.3 */
                    done = true;
                    break;
                }
                else {
                    result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                    goto error;
                }
            }
            for (size_t j = 0; j < SUIT_MAX_INDEX_NUM; j++) {
                parameters[j].soft_failure = orig_soft_failures[j];
            }
            QCBORDecode_ExitArray(&context);
            if (!done) {
                result = SUIT_ERR_TRY_OUT;
                goto error;
            }
            break;
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            result = SUIT_ERR_NOT_IMPLEMENTED;
            break;
        default:
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }


    QCBORDecode_ExitArray(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = SUIT_COMMON,
                .level2.common_key = SUIT_SHARED_SEQUENCE,
                .level3.condition_directive = condition_directive_key,
                .level4.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;
}

suit_err_t suit_process_common_and_command_sequence(suit_extracted_t *extracted,
                                                    const suit_manifest_key_t command_key,
                                                    suit_inputs_t *suit_inputs)
{
    suit_err_t result = SUIT_SUCCESS;
    suit_parameter_args_t parameters[SUIT_MAX_INDEX_NUM];

    UsefulBufC command_buf;
    switch (command_key) {
    case SUIT_DEPENDENCY_RESOLUTION:
        command_buf = extracted->dependency_resolution;
        break;
    case SUIT_PAYLOAD_FETCH:
        command_buf = extracted->payload_fetch;
        break;
    case SUIT_INSTALL:
        command_buf = extracted->install;
        break;
    case SUIT_UNINSTALL:
        command_buf = extracted->uninstall;
    case SUIT_VALIDATE:
        command_buf = extracted->validate;
        break;
    case SUIT_LOAD:
        command_buf = extracted->load;
        break;
    case SUIT_INVOKE:
        command_buf = extracted->invoke;
        break;
    default:
        return SUIT_ERR_INVALID_KEY;
    }
    if (UsefulBuf_IsNULLOrEmptyC(command_buf)) {
        /* no need to execute shared_sequence */
        return SUIT_SUCCESS;
    }

    memset(parameters, 0, sizeof(parameters));
    result = suit_process_shared_sequence(extracted, parameters);
    if (result != SUIT_SUCCESS) {
        goto error;
    }

    result = suit_process_command_sequence_buf(extracted, command_key, command_buf, parameters, suit_inputs);
    if (result != SUIT_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = command_key,
                .level2.condition_directive = SUIT_CONDITION_INVALID,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = QCBOR_SUCCESS,
                .suit_error = result,
                .report = {0}
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;
}

void suit_process_digest(QCBORDecodeContext *context,
                         suit_digest_t *digest)
{
    int64_t algorithm_id;
    UsefulBufC digest_bytes;
    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterArray(context, NULL);
    QCBORDecode_GetInt64(context, &algorithm_id);
    QCBORDecode_GetByteString(context, &digest_bytes);
    digest->algorithm_id = algorithm_id;
    digest->bytes.ptr = (uint8_t *)digest_bytes.ptr;
    digest->bytes.len = digest_bytes.len;
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);
}

suit_err_t suit_process_authentication_wrapper(QCBORDecodeContext *context,
                                               const suit_inputs_t *suit_inputs,
                                               suit_digest_t *digest)
{
    QCBORItem item;

    /* authentication-wrapper */
    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterArray(context, &item);
    size_t length = item.val.uCount;
    if (length < 1) {
        return SUIT_ERR_FAILED_TO_VERIFY;
    }

    /* digest */
    suit_process_digest(context, digest);

    /* signatures */
    UsefulBufC signature;
    for (size_t i = 1; i < length; i++) {
        QCBORDecode_GetByteString(context, &signature);
        /* TODO: ignore signature for now */
    }
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);

    return SUIT_SUCCESS;
}

suit_err_t suit_extract_common(QCBORDecodeContext *context,
                               suit_extracted_t *extracted)
{
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_err_t result = SUIT_SUCCESS;
    suit_manifest_key_t manifest_key = SUIT_COMMON;
    suit_common_key_t common_key = SUIT_COMMON_KEY_INVALID;

    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterMap(context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }
    size_t length = item.val.uCount;
    uint8_t tmp_len = 0;
    for (size_t i = 0; i < length; i++) {
        error = QCBORDecode_PeekNext(context, &item);
        if (error != QCBOR_SUCCESS) {
            goto error;
        }
        else if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        common_key = item.label.int64;
        switch (common_key) {
        case SUIT_DEPENDENCIES:
            result = suit_decode_dependencies_from_item(SUIT_DECODE_MODE_STRICT, context, &item, true, &extracted->dependencies);
            for (size_t j = 0; j < extracted->dependencies.len; j++) {
                uint8_t index = extracted->dependencies.dependency[j].index;
                if (index > SUIT_MAX_INDEX_NUM) {
                    result = SUIT_ERR_NO_MEMORY;
                    goto error;
                }
                extracted->components[index].component = extracted->dependencies.dependency[j].dependency_metadata.prefix;
                extracted->components[index].index = index;
                if (extracted->components_len < index + 1) {
                    extracted->components_len = index + 1;
                }
            }
            break;
        case SUIT_COMPONENTS:
            result = suit_decode_components_from_item(SUIT_DECODE_MODE_STRICT, context, &item, true, extracted->components, &tmp_len);
            if (extracted->components_len < tmp_len) {
                extracted->components_len = tmp_len;
            }
            break;
        case SUIT_SHARED_SEQUENCE:
            QCBORDecode_GetByteString(context, &extracted->shared_sequence);
            break;
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }
    QCBORDecode_ExitMap(context);
    QCBORDecode_ExitBstrWrapped(context);

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = manifest_key,
                .level2.common_key = common_key,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;
}


suit_err_t suit_extract_manifest(suit_extracted_t *extracted)
{
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;

    suit_manifest_key_t manifest_key;

    QCBORDecode_Init(&context, extracted->manifest, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }
    size_t manifest_key_len = item.val.uCount;
    for (size_t j = 0; j < manifest_key_len; j++) {
        error = QCBORDecode_PeekNext(&context, &item);
        if (error != QCBOR_SUCCESS) {
            goto error;
        }
        if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto error;
        }
        manifest_key = item.label.int64;
        switch (manifest_key) {
        case SUIT_MANIFEST_VERSION:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto error;
            }
            if (!(item.uDataType == QCBOR_TYPE_INT64 || item.uDataType == QCBOR_TYPE_UINT64)) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto error;
            }
            if (item.val.int64 != 1) {
                result = SUIT_ERR_INVALID_MANIFEST_VERSION;
            }
            break;
        case SUIT_MANIFEST_SEQUENCE_NUMBER:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto error;
            }
            if (!(item.uDataType == QCBOR_TYPE_INT64 || item.uDataType == QCBOR_TYPE_UINT64)) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto error;
            }
            // TODO: check sequence-number
            break;
        case SUIT_COMMON:
            result = suit_extract_common(&context, extracted);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
            break;
        case SUIT_REFERENCE_URI:
            result = SUIT_ERR_NOT_IMPLEMENTED;
            break;
        case SUIT_MANIFEST_COMPONENT_ID:
            result = suit_decode_component_identifiers_from_item(
                SUIT_DECODE_MODE_STRICT,
                &context, &item, true,
                &extracted->manifest_component_id);
            break;
        case SUIT_VALIDATE:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->validate);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_DEPENDENCY_RESOLUTION:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->dependency_resolution);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, &context, &item, true, &extracted->dependency_resolution_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_PAYLOAD_FETCH:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->payload_fetch);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, &context, &item, true, &extracted->payload_fetch_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_INSTALL:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->install);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, &context, &item, true, &extracted->install_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_UNINSTALL:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->uninstall);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_LOAD:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->load);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_INVOKE:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->invoke);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_TEXT:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->text);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, &context, &item, true, &extracted->text_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
        case SUIT_COSWID:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->coswid);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, &context, &item, true, &extracted->coswid_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;

        default:
            result = SUIT_ERR_INVALID_KEY;
            goto error;
        }

    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_MANIFEST,
                .level1.manifest_key = manifest_key,
                .level2.condition_directive = SUIT_CONDITION_INVALID,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;

}

/*
    Public function. See suit_manifest_process.h
 */
suit_err_t suit_process_envelope(suit_inputs_t *suit_inputs)
{
    QCBORDecodeContext context;
    QCBORError error = QCBOR_SUCCESS;
    QCBORItem item;
    suit_err_t result = SUIT_SUCCESS;

    suit_envelope_key_t envelope_key = SUIT_ENVELOPE_KEY_INVALID;
    suit_manifest_key_t manifest_key = SUIT_MANIFEST_KEY_INVALID;
    suit_digest_t manifest_digest;
    suit_extracted_t extracted = {0};

    /* extract items */
    QCBORDecode_Init(&context,
                     (UsefulBufC){suit_inputs->manifest.ptr, suit_inputs->manifest.len},
                     QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);
    size_t length = item.val.uCount;
    for (size_t i = 0; i < length; i++) {
        error = QCBORDecode_PeekNext(&context, &item);
        if (error != QCBOR_SUCCESS) {
            goto error;
        }
        if (item.uLabelType == QCBOR_TYPE_TEXT_STRING) {
            /* integrated-payload */
            envelope_key = SUIT_INTEGRATED_PAYLOAD;
            if (extracted.payloads.len >= SUIT_MAX_ARRAY_LENGTH) {
                result = SUIT_ERR_NO_MEMORY;
                goto error;
            }
            extracted.payloads.payload[extracted.payloads.len].key = item.label.string;
            QCBORDecode_GetByteString(&context, &extracted.payloads.payload[extracted.payloads.len].bytes);
            extracted.payloads.len++;
        }
        else if (item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64) {
            envelope_key = item.label.int64;
            switch (envelope_key) {
            case SUIT_AUTHENTICATION:
                result = suit_process_authentication_wrapper(&context, suit_inputs, &manifest_digest);
                break;
            case SUIT_MANIFEST:
                if (manifest_digest.algorithm_id == SUIT_ALGORITHM_ID_INVALID) {
                    result = SUIT_ERR_AUTHENTICATION_NOT_FOUND;
                    goto error;
                }
                else {
                    QCBORDecode_GetNext(&context, &item);
                    result = suit_verify_item(&context, &item, &manifest_digest);
                    if (result != SUIT_SUCCESS) {
                        goto error;
                    }
                    extracted.manifest = item.val.string;
                }
                break;
            case SUIT_DELEGATION:
                result = SUIT_ERR_NOT_IMPLEMENTED;
                goto error;

            /* Severed Members */
            case SUIT_SEVERED_INSTALL:
                if (extracted.install.ptr != NULL) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.install);
                break;

            case SUIT_SEVERED_DEPENDENCY_RESOLUTION:
                if (extracted.dependency_resolution.ptr != NULL) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.dependency_resolution);
                break;

            case SUIT_SEVERED_PAYLOAD_FETCH:
                if (extracted.payload_fetch.ptr != NULL) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.payload_fetch);
                break;

            case SUIT_SEVERED_TEXT:
                if (extracted.text.ptr != NULL) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.text);
                break;

            case SUIT_SEVERED_COSWID:
                if (extracted.coswid.ptr != NULL) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.coswid);
                break;

            default:
                result = SUIT_ERR_NOT_IMPLEMENTED;
                goto error;
                break;
            }
        }
        else {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto error;
        }
    }
    QCBORDecode_ExitMap(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        result = SUIT_ERR_NOT_A_SUIT_MANIFEST;
        goto out;
    }

    result = suit_extract_manifest(&extracted);

    /* TODO: check digests */

    if (suit_inputs->dependency_depth == 0 &&
        extracted.manifest_component_id.len > 0) {
        suit_store_args_t store = {0};
        store.report.val = 0;
        store.dst = extracted.manifest_component_id;
        store.src_buf = suit_inputs->manifest;
        store.operation = SUIT_STORE;
        result = suit_store_callback(store);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* dependency-resolution */
    if (suit_inputs->process_flags.dependency_resolution) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_DEPENDENCY_RESOLUTION, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* payload-fetch */
    if (suit_inputs->process_flags.payload_fetch) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_PAYLOAD_FETCH, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* install */
    if (suit_inputs->process_flags.install) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_INSTALL, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* uninstall */
    if (suit_inputs->process_flags.uninstall) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_UNINSTALL, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* validate */
    if (suit_inputs->process_flags.validate) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_VALIDATE, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* load */
    if (suit_inputs->process_flags.load) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_LOAD, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    /* invoke */
    if (suit_inputs->process_flags.invoke) {
        result = suit_process_common_and_command_sequence(&extracted, SUIT_INVOKE, suit_inputs);
        if (result != SUIT_SUCCESS) {
            goto error;
        }
    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }

out:
    return result;

error:
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = envelope_key,
                .level1.manifest_key = manifest_key,
                .level2.condition_directive = SUIT_CONDITION_INVALID,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
    return result;
}
