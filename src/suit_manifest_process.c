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

#include "csuit/suit_manifest_decode.h"
#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_callbacks.h"

suit_err_t suit_set_parameters(QCBORDecodeContext *context,
                               const suit_con_dir_key_t directive,
                               const suit_index_t *suit_index,
                               suit_parameter_args_t parameters[],
                               const bool may_soft_failure)
{
    suit_err_t result = SUIT_SUCCESS;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_parameter_key_t parameter = SUIT_PARAMETER_INVALID;
    bool override = (directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS || directive == SUIT_DIRECTIVE_OVERRIDE_MULTIPLE);

    QCBORDecode_EnterMap(context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }

    union {
        uint64_t u64;
        int64_t i64;
        UsefulBufC str;
        bool b;
        suit_digest_t digest;
        suit_version_match_t version_match;
        suit_wait_event_t wait_event;
        suit_component_metadata_t component_metadata;
    } val;

    size_t length = item.val.uCount;
    for (size_t i = 0; i < length; i++) {
        memset(&val, 0, sizeof(val));
        QCBORDecode_PeekNext(context, &item);
        if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto error;
        }
        parameter = item.label.int64;

        switch (parameter) {
        /* int64 */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
        case SUIT_PARAMETER_UPDATE_PRIORITY:
             QCBORDecode_GetInt64(context, &val.i64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY;
                    parameters[tmp_index].update_priority = val.i64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY */

        /* uint64 */
        case SUIT_PARAMETER_IMAGE_SIZE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_SIZE;
                    parameters[tmp_index].image_size = val.u64;
                }
            }
            break;
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
        case SUIT_PARAMETER_SOURCE_COMPONENT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT;
                    parameters[tmp_index].source_component = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
        case SUIT_PARAMETER_COMPONENT_SLOT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT;
                    parameters[tmp_index].component_slot = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
        case SUIT_PARAMETER_USE_BEFORE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_USE_BEFORE;
                    parameters[tmp_index].use_before = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
        case SUIT_PARAMETER_MINIMUM_BATTERY:
             QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY;
                    parameters[tmp_index].minimum_battery = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY */

        /* bool */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE)
        case SUIT_PARAMETER_SOFT_FAILURE:
            if (!may_soft_failure) {
                result = SUIT_ERR_INVALID_VALUE;
                goto error;
            }
            QCBORDecode_GetBool(context, &val.b);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_SOFT_FAILURE) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOFT_FAILURE;
                    parameters[tmp_index].soft_failure = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER)
        case SUIT_PARAMETER_STRICT_ORDER:
            QCBORDecode_GetBool(context, &val.b);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_STRICT_ORDER) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_STRICT_ORDER;
                    parameters[tmp_index].strict_order = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER */

        /* tstr */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_URI)
        case SUIT_PARAMETER_URI:
            QCBORDecode_GetTextString(context, &val.str);
            if (val.str.len > SUIT_MAX_URI_LENGTH) {
                result = SUIT_ERR_NO_MEMORY;
                goto error;
            }
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_URI) ||
                    directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_URI;
                    parameters[tmp_index].uri = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER;
                    parameters[tmp_index].vendor_id = val.str;
                }
            }
            break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER;
                    parameters[tmp_index].class_id = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER;
                    parameters[tmp_index].device_id = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
        case SUIT_PARAMETER_CONTENT:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CONTENT;
                    parameters[tmp_index].content = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CONTENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS)
        case SUIT_PARAMETER_INVOKE_ARGS:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_INVOKE_ARGS) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_INVOKE_ARGS;
                    parameters[tmp_index].invoke_args = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
        case SUIT_PARAMETER_FETCH_ARGS:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_FETCH_ARGS;
                    parameters[tmp_index].fetch_args = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO)
        /* bstr .cbor COSE_Encrypt0 // bstr .cbor COSE_Encrypt */
        case SUIT_PARAMETER_ENCRYPTION_INFO:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                uint8_t tmp_index = suit_index->index[j];
                if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) || override) {
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO;
                    parameters[tmp_index].encryption_info = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO */

        /* bstr .cbor SUIT_Diget */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            result = suit_decode_digest_from_item(SUIT_DECODE_MODE_STRICT, context, &item, true, &val.digest);
            QCBORDecode_ExitBstrWrapped(context);
            if (result == SUIT_SUCCESS) {
                for (size_t j = 0; j < suit_index->len; j++) {
                    uint8_t tmp_index = suit_index->index[j];
                    if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST) || override) {
                        parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST;
                        parameters[tmp_index].image_digest = val.digest;
                    }
                }
            }
            break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
        /* bstr .cbor SUIT_Parameter_Version_Match */
        case SUIT_PARAMETER_VERSION:
            QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            result = suit_decode_version_match_from_item(context, &item, true, &val.version_match);
            QCBORDecode_ExitBstrWrapped(context);
            if (result == SUIT_SUCCESS) {
                for (size_t j = 0; j < suit_index->len; j++) {
                    uint8_t tmp_index = suit_index->index[j];
                    if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VERSION) || override) {
                        parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_VERSION;
                        parameters[tmp_index].version_match = val.version_match;
                    }
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_VERSION */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
        /* bstr .cbor SUIT_Wait_Event */
        case SUIT_PARAMETER_WAIT_INFO:
            QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            result = suit_decode_wait_event_from_item(context, &item, true, &val.wait_event);
            QCBORDecode_ExitBstrWrapped(context);
            if (result == SUIT_SUCCESS) {
                for (size_t j = 0; j < suit_index->len; j++) {
                    uint8_t tmp_index = suit_index->index[j];
                    if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO) || override) {
                        parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_WAIT_INFO;
                        parameters[tmp_index].wait_info = val.wait_event;
                    }
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
        /* bstr .cbor SUIT_Component_Metadata */
        case SUIT_PARAMETER_COMPONENT_METADATA:
            QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            result = suit_decode_component_metadata_from_item(context, &item, true, &val.component_metadata);
            QCBORDecode_ExitBstrWrapped(context);
            if (result == SUIT_SUCCESS) {
                for (size_t j = 0; j < suit_index->len; j++) {
                    uint8_t tmp_index = suit_index->index[j];
                    if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) || override) {
                        parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA;
                        parameters[tmp_index].component_metadata = val.component_metadata;
                    }
                }
            }
            break;
#endif

        case SUIT_PARAMETER_INVALID:
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
            break;
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
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE)
suit_err_t suit_override_multiple(QCBORDecodeContext *context,
                                    suit_parameter_args_t parameters[],
                                    const bool may_soft_failure)
{
    QCBORItem item;
    suit_index_t suit_index;
    suit_err_t result = SUIT_SUCCESS;

    QCBORDecode_EnterMap(context, &item);
    const size_t map_count = item.val.uCount;
    for (size_t i = 0; i < map_count; i++) {
        QCBORDecode_PeekNext(context, &item);
        if (item.uLabelType != QCBOR_TYPE_INT64 || item.label.int64 < 0) {
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        if (SUIT_MAX_COMPONENT_NUM < item.label.int64) {
            return SUIT_ERR_NOT_FOUND;
        }
        suit_index.index[0] = item.label.int64;
        suit_index.len = 1;
        result = suit_set_parameters(context, SUIT_DIRECTIVE_OVERRIDE_MULTIPLE, &suit_index, parameters, may_soft_failure);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    QCBORDecode_ExitMap(context);

    return result;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE */

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
    case QCBOR_TYPE_INT64:
        if (item.val.int64 < 0) {
            return SUIT_ERR_INVALID_VALUE;
        }
        // fallthrough
    case QCBOR_TYPE_UINT64:
        QCBORDecode_GetUInt64(context, &val.u64);
        if (val.u64 > UINT8_MAX) {
            return SUIT_ERR_INVALID_TYPE_OF_VALUE;
        }
        suit_index->index[0] = (uint8_t)val.u64;
        suit_index->len = 1;
        break;

#if !defined(LIBCSUIT_DISABLE_COMPONENT_INDEX_TYPE_BOOLEAN)
    case QCBOR_TYPE_TRUE:
        QCBORDecode_GetBool(context, &val.b);
        for (size_t i = 0; i < extracted->components_len; i++) {
            suit_index->index[i] = extracted->components[i].index;
        }
        suit_index->len = extracted->components_len;
        break;
#endif /* !LIBCSUIT_DISABLE_COMPONENT_INDEX_TYPE_BOOLEAN */

#if !defined(LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY)
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
        break;
#endif /* !LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY */

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
    case SUIT_MANIFEST_COMPONENT_ID:
        result.manifest_component_id = 1;
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

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_FETCH)
suit_err_t suit_process_fetch(suit_extracted_t *extracted,
                              const suit_parameter_args_t parameters[],
                              const suit_index_t *suit_index,
                              const suit_rep_policy_t report,
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
            }
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
                fetch.component_metadata = parameters[tmp_index].component_metadata;
            }

            /* the size of uri is already checked at suit-directive-override-parameters */
            memcpy(fetch.uri, parameters[tmp_index].uri.ptr, parameters[tmp_index].uri.len);
            fetch.uri[parameters[tmp_index].uri.len] = '\0';
            fetch.uri_len = parameters[tmp_index].uri.len + 1;

            fetch.args = parameters[tmp_index].fetch_args;

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
            }
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
                store.component_metadata = parameters[tmp_index].component_metadata;
            }
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS) {
                store.fetch_args = parameters[tmp_index].fetch_args;
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
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_FETCH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_INVOKE)
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
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_INVOKE */

#if !defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
suit_err_t suit_index_is_dependency(const suit_extracted_t *extracted,
                                    uint8_t index)
{
    for (size_t i = 0; i < extracted->dependencies.len; i++) {
        if (extracted->dependencies.dependency[i].index == index) {
            return SUIT_SUCCESS;
        }
    }
    return SUIT_ERR_NOT_FOUND;
}
#endif /* !LIBCSUIT_DISABLE_COMMON_DEPENDENCIES */

suit_err_t suit_process_condition(suit_extracted_t *extracted,
                                  const suit_con_dir_key_t condition,
                                  suit_parameter_args_t parameters[],
                                  const suit_index_t *suit_index,
                                  suit_rep_policy_t report,
                                  const suit_inputs_t *suit_inputs)
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
#if !defined(LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT)
        case SUIT_CONDITION_COMPONENT_SLOT:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) {
                args.expected.u64 = parameters[tmp_index].component_slot;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_CONDITION_USE_BEFORE)
        case SUIT_CONDITION_USE_BEFORE:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) {
                args.expected.u64 = parameters[tmp_index].use_before;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY)
        case SUIT_CONDITION_MINIMUM_BATTERY:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) {
                args.expected.u64 = parameters[tmp_index].minimum_battery;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY */

#if !defined(LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED)
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) {
                args.expected.i64 = parameters[tmp_index].update_priority;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED */

        /* bstr */
        case SUIT_CONDITION_VENDOR_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].vendor_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

#if !defined(LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER)
        case SUIT_CONDITION_CLASS_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].class_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER)
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) {
                args.expected.str = parameters[tmp_index].device_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT)
        case SUIT_CONDITION_CHECK_CONTENT:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) {
                args.expected.str = parameters[tmp_index].content;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT */

        /* suit-digest */
        case SUIT_CONDITION_IMAGE_MATCH:
#if !defined(LIBCSUIT_DISABLE_CONDITION_IMAGE_NOT_MATCH)
        case SUIT_CONDITION_IMAGE_NOT_MATCH:
#endif
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

        /* draft-ietf-suit-trust-domains */
#if !defined(LIBCSUIT_DISABLE_CONDITION_VERSION)
        case SUIT_CONDITION_VERSION:
            if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_VERSION) {
                args.expected.version_match = parameters[tmp_index].version_match;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_VERSION */

#if !defined(LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY)
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
            result = suit_index_is_dependency(extracted, tmp_index);
            if (result != SUIT_SUCCESS) {
                return result;
            }

            suit_inputs_t tmp_inputs = *suit_inputs;
            if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST)) {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            tmp_inputs.expected_manifest_digest = parameters[tmp_index].image_digest;

            suit_payload_t *payload = suit_index_to_payload(extracted, tmp_index);
            if (payload == NULL) {
                return SUIT_ERR_NOT_FOUND;
            }
            tmp_inputs.manifest = payload->bytes;

            // checks only suit-delegation and suit-authentication-wrapper
            tmp_inputs.process_flags.all = 0;
            tmp_inputs.dependency_depth++;

            result = suit_process_envelope(&tmp_inputs);
            if (result != SUIT_SUCCESS) {
                return result;
            }

            // callback is not needed
            continue;
#endif /* !LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY */

        case SUIT_CONDITION_ABORT:
            result = SUIT_ERR_ABORT;
            break;

#if !defined(LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY)
        case SUIT_CONDITION_IS_DEPENDENCY:
            result = suit_index_is_dependency(extracted, tmp_index);
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY */

        case SUIT_CONDITION_INVALID:
        default:
            return SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            /* already handled without callback */
            return result;
        }

        args.report = report;
        result = suit_condition_callback(args);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return result;
}

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WAIT)
suit_err_t suit_process_wait(suit_extracted_t *extracted,
                             suit_parameter_args_t parameters[],
                             const suit_index_t *suit_index,
                             suit_rep_policy_t report)
{
    suit_err_t result = SUIT_SUCCESS;

    for (uint8_t i = 0; i < suit_index->len; i++) {
        uint8_t tmp_index = suit_index->index[i];

        suit_wait_args_t wait_args = {0};
        if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO)) {
            return SUIT_ERR_PARAMETER_NOT_FOUND;
        }
        const suit_component_identifier_t *dst = suit_index_to_component_identifier(extracted, tmp_index);
        if (dst == NULL) {
            return SUIT_ERR_NOT_FOUND;
        }
        wait_args.dst = *dst;
        wait_args.wait_info = parameters[tmp_index].wait_info;
        wait_args.report = report;

        result = suit_wait_callback(wait_args);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return result;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WAIT */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WRITE)
suit_err_t suit_process_write(const suit_extracted_t *extracted,
                              const suit_parameter_args_t parameters[],
                              const suit_index_t *suit_index,
                              const suit_rep_policy_t report,
                              suit_inputs_t *suit_inputs)
{
    for (size_t i = 0; i < suit_index->len; i++) {
        const uint8_t tmp_index = suit_index->index[i];

        suit_store_args_t store = {0};
        if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT)) {
            return SUIT_ERR_PARAMETER_NOT_FOUND;
        }
        store.src_buf = parameters[tmp_index].content;
        if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
            store.encryption_info = parameters[tmp_index].encryption_info;
            memcpy(store.mechanisms, suit_inputs->mechanisms, sizeof(store.mechanisms));
        }
        if (parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
            store.component_metadata = parameters[tmp_index].component_metadata;
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
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WRITE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY) || \
    !defined(LIBCSUIT_DISABLE_DIRECTIVE_SWAP)
suit_err_t suit_process_copy_swap(const suit_extracted_t *extracted,
                                  const suit_parameter_args_t parameters[],
                                  bool is_swap,
                                  const suit_index_t *suit_index,
                                  const suit_rep_policy_t report,
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
#endif

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_UNLINK)
suit_err_t suit_process_unlink(const suit_extracted_t *extracted,
                               const suit_index_t *suit_index,
                               const suit_rep_policy_t report)
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
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_UNLINK */

suit_err_t suit_process_command_sequence_buf(suit_extracted_t *extracted,
                                             const suit_manifest_key_t command_key,
                                             suit_parameter_args_t parameters[],
                                             UsefulBufC buf,
                                             suit_index_t *suit_index,
                                             suit_inputs_t *suit_inputs,
                                             const bool may_soft_failure);

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
suit_err_t suit_process_run_sequence(QCBORDecodeContext *context,
                                     suit_extracted_t *extracted,
                                     const suit_manifest_key_t command_key,
                                     suit_parameter_args_t parameters[],
                                     suit_index_t *suit_index,
                                     suit_inputs_t *suit_inputs)
{
    UsefulBufC buf;
    QCBORDecode_GetByteString(context, &buf);
    QCBORError error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }

    for (size_t i = 0; i < suit_index->len; i++) {
        suit_index_t tmp_suit_index;
        tmp_suit_index.len = 1;
        tmp_suit_index.index[0] = suit_index->index[i];

        // soft-failure is false by default in run-sequence
        parameters[suit_index->index[i]].soft_failure = false;

        suit_err_t result = suit_process_command_sequence_buf(extracted, command_key, parameters, buf, &tmp_suit_index, suit_inputs, true);
        if (result != SUIT_SUCCESS && !parameters[suit_index->index[i]].soft_failure) {
            return result;
        }
    }

    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
suit_err_t suit_process_try_each(QCBORDecodeContext *context,
                                 suit_extracted_t *extracted,
                                 const suit_manifest_key_t command_key,
                                 suit_parameter_args_t parameters[],
                                 suit_index_t *suit_index,
                                 suit_inputs_t *suit_inputs)
{
    suit_err_t result;
    QCBORItem item;
    QCBORDecode_EnterArray(context, &item);
    if (item.uDataType != QCBOR_TYPE_ARRAY) {
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    const size_t try_count = item.val.uCount;

    bool done_any = false;
    for (size_t i = 0; i < try_count; i++) {
        bool done = false;
        QCBORDecode_GetNext(context, &item);
        if (i + 1 == try_count) {
            /* the last element */
            if (item.uDataType == QCBOR_TYPE_NULL) {
                // see https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest#name-suit-directive-try-each
                done_any = true;
                done = true;
                break;
            }
        }

        for (size_t j = 0; j < suit_index->len; j++) {
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                if (!done) {
                    suit_index_t tmp_suit_index;
                    tmp_suit_index.len = 1;
                    tmp_suit_index.index[0] = suit_index->index[j];

                    // soft-failure is true by default in try-each
                    parameters[suit_index->index[j]].soft_failure = true;

                    result = suit_process_command_sequence_buf(extracted, command_key, parameters, item.val.string, &tmp_suit_index, suit_inputs, true);
                    if (result == SUIT_SUCCESS) {
                        done_any = true;
                        done = true;
                    }
                    else if (!parameters[suit_index->index[j]].soft_failure) {
                        return SUIT_ERR_TRY_OUT;
                    }
                }
            }
            else {
                return SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
        }
    }
    QCBORDecode_ExitArray(context);
    return (done_any) ? SUIT_SUCCESS : SUIT_ERR_TRY_OUT;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS)
suit_err_t suit_process_copy_params(QCBORDecodeContext *context,
                                    suit_extracted_t *extracted,
                                    suit_parameter_args_t parameters[],
                                    suit_index_t *suit_index)
{
    QCBORItem item;
    QCBORDecode_EnterMap(context, &item);
    size_t map_count = item.val.uCount;
    for (size_t i = 0; i < map_count; i++) {
        QCBORDecode_EnterArray(context, &item);
        if (item.uLabelType != QCBOR_TYPE_INT64) {
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        if (item.label.int64 < 0) {
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        } else if (extracted->components_len < item.label.int64) {
            return SUIT_ERR_NOT_FOUND;
        }
        const uint8_t src_index = item.label.int64;
        size_t len = item.val.uCount;
        for (size_t j = 0; j < len; j++) {
            int64_t parameter;
            QCBORDecode_GetInt64(context, &parameter);
            for (size_t k = 0; k < suit_index->len; k++) {
                const uint8_t tmp_index = suit_index->index[k];
                switch (parameter) {
                /* int64 */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
                case SUIT_PARAMETER_UPDATE_PRIORITY:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY;
                    parameters[tmp_index].update_priority = parameters[src_index].update_priority;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY */

                /* uint64 */
                case SUIT_PARAMETER_IMAGE_SIZE:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_SIZE;
                    parameters[tmp_index].image_size = parameters[src_index].image_size;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
                case SUIT_PARAMETER_SOURCE_COMPONENT:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT;
                    parameters[tmp_index].source_component = parameters[src_index].source_component;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
                case SUIT_PARAMETER_COMPONENT_SLOT:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT;
                    parameters[tmp_index].component_slot = parameters[src_index].component_slot;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
                case SUIT_PARAMETER_USE_BEFORE:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_USE_BEFORE;
                    parameters[tmp_index].use_before = parameters[src_index].use_before;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
                case SUIT_PARAMETER_MINIMUM_BATTERY:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY;
                    parameters[tmp_index].minimum_battery = parameters[src_index].minimum_battery;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY */

                /* bool */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE)
                case SUIT_PARAMETER_SOFT_FAILURE:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_SOFT_FAILURE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_SOFT_FAILURE;
                    parameters[tmp_index].soft_failure = parameters[src_index].soft_failure;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER)
                case SUIT_PARAMETER_STRICT_ORDER:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_STRICT_ORDER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_STRICT_ORDER;
                    parameters[tmp_index].strict_order = parameters[src_index].strict_order;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER */

                /* tstr */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_URI)
                case SUIT_PARAMETER_URI:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_URI)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_URI;
                    parameters[tmp_index].uri = parameters[src_index].uri;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

                /* bstr */
                case SUIT_PARAMETER_VENDOR_IDENTIFIER:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER;
                    parameters[tmp_index].vendor_id = parameters[src_index].vendor_id;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
                case SUIT_PARAMETER_CLASS_IDENTIFIER:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER;
                    parameters[tmp_index].class_id = parameters[src_index].class_id;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
                case SUIT_PARAMETER_DEVICE_IDENTIFIER:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER;
                    parameters[tmp_index].device_id = parameters[src_index].device_id;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
                case SUIT_PARAMETER_CONTENT:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_CONTENT;
                    parameters[tmp_index].content = parameters[src_index].content;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CONTENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS)
                case SUIT_PARAMETER_INVOKE_ARGS:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_INVOKE_ARGS)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_INVOKE_ARGS;
                    parameters[tmp_index].invoke_args = parameters[src_index].invoke_args;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
                case SUIT_PARAMETER_FETCH_ARGS:
                    if (!(parameters[tmp_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_FETCH_ARGS;
                    parameters[tmp_index].fetch_args = parameters[src_index].fetch_args;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS */

                /* bstr .cbor COSE_Encrypt0 // bstr .cbor COSE_Encrypt */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO)
                case SUIT_PARAMETER_ENCRYPTION_INFO:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO;
                    parameters[tmp_index].encryption_info = parameters[src_index].encryption_info;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO */

                /* bstr .cbor SUIT_Diget */
                case SUIT_PARAMETER_IMAGE_DIGEST:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST;
                    parameters[tmp_index].image_digest = parameters[src_index].image_digest;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
                /* SUIT_Parameter_Version_Match */
                case SUIT_PARAMETER_VERSION:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_VERSION)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_VERSION;
                    parameters[tmp_index].version_match = parameters[src_index].version_match;
                    break;
#endif

#if !defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
                /* SUIT_Wait_Event */
                case SUIT_PARAMETER_WAIT_INFO:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_WAIT_INFO;
                    parameters[tmp_index].wait_info = parameters[src_index].wait_info;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
                /* SUIT_Component_Metadata */
                case SUIT_PARAMETER_COMPONENT_METADATA:
                    if (!(parameters[src_index].exists & SUIT_PARAMETER_COMPONENT_METADATA)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    parameters[tmp_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA;
                    parameters[tmp_index].component_metadata = parameters[src_index].component_metadata;
                    break;
#endif

                case SUIT_PARAMETER_INVALID:
                default:
                    return SUIT_ERR_NOT_IMPLEMENTED;
                }
            }
        }
        QCBORDecode_ExitArray(context);
    }
    QCBORDecode_ExitMap(context);

    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS */

suit_err_t suit_process_command_sequence_buf(suit_extracted_t *extracted,
                                             const suit_manifest_key_t command_key,
                                             suit_parameter_args_t parameters[],
                                             UsefulBufC buf,
                                             suit_index_t *suit_index,
                                             suit_inputs_t *suit_inputs,
                                             const bool may_soft_failure)
{
    suit_err_t result = SUIT_SUCCESS;
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
            result = suit_set_index(&context, extracted, suit_index);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS)
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_SET_PARAMETERS, suit_index, parameters, may_soft_failure);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS */

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, suit_index, parameters, may_soft_failure);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE)
        case SUIT_DIRECTIVE_OVERRIDE_MULTIPLE:
            result = suit_override_multiple(&context, parameters, may_soft_failure);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS)
        case SUIT_DIRECTIVE_COPY_PARAMS:
            result = suit_process_copy_params(&context, extracted, parameters, suit_index);
            break;
#endif

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_FETCH)
        case SUIT_DIRECTIVE_FETCH:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_fetch(extracted, parameters, suit_index, report, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_FETCH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WRITE)
        case SUIT_DIRECTIVE_WRITE:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_write(extracted, parameters, suit_index, report, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WRITE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY)
        case SUIT_DIRECTIVE_COPY:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_copy_swap(extracted, parameters, false, suit_index, report, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_COPY */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SWAP)
       case SUIT_DIRECTIVE_SWAP:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_copy_swap(extracted, parameters, true, suit_index, report, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_SWAP */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_UNLINK)
        case SUIT_DIRECTIVE_UNLINK:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_unlink(extracted, suit_index, report);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_UNLINK */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_INVOKE)
        case SUIT_DIRECTIVE_INVOKE:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_invoke(extracted, parameters, report, suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_INVOKE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WAIT)
        case SUIT_DIRECTIVE_WAIT:
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_wait(extracted, parameters, suit_index, report);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WAIT */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
        case SUIT_DIRECTIVE_TRY_EACH:
            result = suit_process_try_each(&context, extracted, command_key, parameters, suit_index, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            result = suit_process_run_sequence(&context, extracted, command_key, parameters, suit_index, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY)
        case SUIT_DIRECTIVE_PROCESS_DEPENDENCY:
            QCBORDecode_GetUInt64(&context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                const uint8_t tmp_index = suit_index->index[j];
                result = suit_index_is_dependency(extracted, tmp_index);
                if (result != SUIT_SUCCESS) {
                    return result;
                }

                suit_inputs_t tmp_inputs = *suit_inputs;
                suit_payload_t *payload = suit_index_to_payload(extracted, tmp_index);
                if (payload == NULL) {
                    return SUIT_ERR_NOT_FOUND;
                }
                tmp_inputs.manifest = payload->bytes;

                /*
                 * Calls "second manifest processor" defined in
                 * https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-02#section-6.4.1-3
                 * by limiting set of operations (command_key).
                 */
                tmp_inputs.process_flags = suit_manifest_key_to_process_flag(command_key);
                tmp_inputs.dependency_depth++;

                result = suit_process_envelope(&tmp_inputs);
                if (result != SUIT_SUCCESS) {
                    break;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY */

        case SUIT_CONDITION_VENDOR_IDENTIFIER:
#if !defined(LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER)
        case SUIT_CONDITION_CLASS_IDENTIFIER:
#endif
        case SUIT_CONDITION_IMAGE_MATCH:
#if !defined(LIBCSUIT_DISABLE_CONDITION_USE_BEFORE)
        case SUIT_CONDITION_USE_BEFORE:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT)
        case SUIT_CONDITION_COMPONENT_SLOT:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT)
        case SUIT_CONDITION_CHECK_CONTENT:
#endif
        case SUIT_CONDITION_ABORT:
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER)
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY)
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY)
        case SUIT_CONDITION_IS_DEPENDENCY:
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
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_condition(extracted, condition_directive_key, parameters, suit_index, report, suit_inputs);
            break;

        case SUIT_DIRECTIVE_INVALID:
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
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

suit_err_t suit_process_shared_sequence(suit_extracted_t *extracted,
                                        suit_parameter_args_t parameters[],
                                        suit_inputs_t *suit_inputs)
{
    if (extracted->shared_sequence.len == 0) {
        return SUIT_SUCCESS;
    }
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_con_dir_key_t condition_directive_key = SUIT_CONDITION_INVALID;
    suit_index_t suit_index = (suit_index_t) {.len = 1, .index[0] = 0};
    suit_rep_policy_t report;

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
            result = suit_set_index(&context, extracted, &suit_index);
            break;

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, &suit_index, parameters, false);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS)
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(&context, SUIT_DIRECTIVE_SET_PARAMETERS, &suit_index, parameters, false);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS */

        case SUIT_CONDITION_VENDOR_IDENTIFIER:
#if !defined(LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER)
        case SUIT_CONDITION_CLASS_IDENTIFIER:
#endif
        case SUIT_CONDITION_IMAGE_MATCH:
#if !defined(LIBCSUIT_DISABLE_CONDITION_USE_BEFORE)
        case SUIT_CONDITION_USE_BEFORE:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT)
        case SUIT_CONDITION_COMPONENT_SLOT:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT)
        case SUIT_CONDITION_CHECK_CONTENT:
#endif
        case SUIT_CONDITION_ABORT:
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER)
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY)
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
#endif
#if !defined(LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY)
        case SUIT_CONDITION_IS_DEPENDENCY:
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
            QCBORDecode_GetUInt64(&context, &report.val);
            result = suit_process_condition(extracted, condition_directive_key, parameters, &suit_index, report, suit_inputs);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
        case SUIT_DIRECTIVE_TRY_EACH:
            result = suit_process_try_each(&context, extracted, SUIT_COMMON, parameters, &suit_index, NULL);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            result = suit_process_run_sequence(&context, extracted, SUIT_COMMON, parameters, &suit_index, suit_inputs);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
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
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

suit_err_t suit_process_common_and_command_sequence(suit_extracted_t *extracted,
                                                    const suit_manifest_key_t command_key,
                                                    suit_inputs_t *suit_inputs)
{
    suit_err_t result = SUIT_SUCCESS;
    for (size_t i = 0; i < SUIT_MAX_INDEX_NUM; i++) {
        suit_inputs->parameters[i].soft_failure = true;
    }

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
        break;
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

    result = suit_process_shared_sequence(extracted, suit_inputs->parameters, suit_inputs);
    if (result != SUIT_SUCCESS) {
        goto error;
    }

    suit_index_t suit_index = {.len = 1, .index[0] = 0};
    result = suit_process_command_sequence_buf(extracted, command_key, suit_inputs->parameters, command_buf, &suit_index, suit_inputs, false);
    if (result != SUIT_SUCCESS) {
        goto error;
    }
    return result;

error:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

suit_err_t suit_process_digest(QCBORDecodeContext *context,
                               suit_digest_t *digest,
                               UsefulBufC *digest_buf)
{
    int64_t algorithm_id;
    QCBORItem item;
    QCBORDecode_PeekNext(context, &item);
    if (item.uDataType != QCBOR_TYPE_BYTE_STRING) {
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    if (digest_buf != NULL) {
        *digest_buf = item.val.string;
    }
    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterArray(context, NULL);
    QCBORDecode_GetInt64(context, &algorithm_id);
    UsefulBufC digest_bytes;
    QCBORDecode_GetByteString(context, &digest_bytes);
    digest->algorithm_id = algorithm_id;
    digest->bytes.ptr = (uint8_t *)digest_bytes.ptr;
    digest->bytes.len = digest_bytes.len;
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);

    QCBORError error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }
    return SUIT_SUCCESS;
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
    UsefulBufC digest_buf;
    suit_err_t result = suit_process_digest(context, digest, &digest_buf);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    /* signatures */
    bool verified = false;
    UsefulBufC signature;
    for (size_t i = 1; i < length; i++) {
        QCBORDecode_GetByteString(context, &signature);
        if (verified) {
            continue;
        }
        size_t j = 0;
        for (; j < SUIT_MAX_KEY_NUM; j++) {
            switch (suit_inputs->mechanisms[j].cose_tag) {
            case COSE_SIGN1_TAG:
                result = suit_verify_cose_sign1(signature, &suit_inputs->mechanisms[j].key, &digest_buf);
                break;
            case COSE_MAC0_TAG:
                result = suit_validate_cose_mac0(signature, &suit_inputs->mechanisms[j].key, &digest_buf);
                break;
            default:
                continue;
            }
            if (result == SUIT_SUCCESS) {
                verified = true;
                break;
            }
        }
    }
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);
    QCBORError error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }

    return (verified) ? SUIT_SUCCESS : SUIT_ERR_FAILED_TO_VERIFY;
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
#if !defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
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
#endif /* !LIBCSUIT_DISABLE_COMMON_DEPENDENCIES */

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
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}


suit_err_t suit_extract_manifest(suit_extracted_t *extracted)
{
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;

    suit_manifest_key_t manifest_key = SUIT_MANIFEST_KEY_INVALID;

    QCBORDecode_Init(&context, extracted->manifest, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto error;
    }
    size_t manifest_key_len = item.val.uCount;
    for (size_t i = 0; i < manifest_key_len; i++) {
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
            size_t j = 0;
            for (; j < LIBCSUIT_SUPPORTED_VERSIONS_LEN; j++) {
                if (LIBCSUIT_SUPPORTED_VERSIONS[j] == item.val.uint64) {
                    break;
                }
            }
            if (j == LIBCSUIT_SUPPORTED_VERSIONS_LEN) {
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

#if !defined(LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI)
        case SUIT_REFERENCE_URI:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto error;
            }
            // XXX: ignore this
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
        case SUIT_MANIFEST_COMPONENT_ID:
            result = suit_decode_component_identifiers_from_item(
                SUIT_DECODE_MODE_STRICT,
                &context, &item, true,
                &extracted->manifest_component_id);
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID */

        case SUIT_VALIDATE:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->validate);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;

#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
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
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
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
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
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
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INSTALL */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
        case SUIT_UNINSTALL:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->uninstall);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
        case SUIT_LOAD:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->load);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
        case SUIT_INVOKE:
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->invoke);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
        case SUIT_TEXT:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto error;
            }
            // XXX: ignore this
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_TEXT */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_COSWID)
        case SUIT_COSWID:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto error;
            }
            // XXX: ignore this
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COSWID */

        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
            goto error;
        }

    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return result;

error:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DELEGATION)
suit_err_t suit_process_delegation(QCBORDecodeContext *context,
                                   suit_inputs_t *suit_inputs)
{
    suit_err_t result;
    QCBORError error = QCBOR_SUCCESS;
    QCBORItem item;
    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterArray(context, &item);
    if (item.val.uCount > SUIT_MAX_KEY_NUM) {
        result = SUIT_ERR_NO_MEMORY;
        goto error;
    }
    const size_t delegation_chain_num = item.val.uCount;
    for (size_t i = 0; i < delegation_chain_num; i++) {
        QCBORDecode_EnterArray(context, &item);
        if (item.val.uCount > SUIT_MAX_KEY_NUM) {
            result = SUIT_ERR_NO_MEMORY;
            goto error;
        }
        const size_t num = item.val.uCount;
        for (size_t j = 0; j < num; j++) {
            UsefulBufC cwt;
            QCBORDecode_GetByteString(context, &cwt);
            UsefulBufC cwt_payload;
            size_t k = 0;
            for (; k < SUIT_MAX_KEY_NUM; k++) {
                if (!suit_inputs->mechanisms[k].use) {
                    continue;
                }
                cwt_payload = NULLUsefulBufC;
                switch (suit_inputs->mechanisms[k].cose_tag) {
                case COSE_SIGN1_TAG:
                    result = suit_verify_cose_sign1(cwt, &suit_inputs->mechanisms[k].key, &cwt_payload);
                    break;
                case COSE_MAC0_TAG:
                    result = suit_validate_cose_mac0(cwt, &suit_inputs->mechanisms[k].key, &cwt_payload);
                    break;
                default:
                    continue;
                }
                if (result == SUIT_SUCCESS) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                result = SUIT_ERR_FAILED_TO_VERIFY_DELEGATION;
                goto error;
            }

            // search empty slot
            for (k = 0; k < SUIT_MAX_KEY_NUM; k++) {
                if (!suit_inputs->mechanisms[k].use) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                result = SUIT_ERR_NO_MEMORY;
                goto error;
            }
            suit_inputs->mechanisms[k].key.cose_algorithm_id = T_COSE_ALGORITHM_ES256;
            result = suit_set_suit_key_from_cwt_payload(cwt_payload, &suit_inputs->mechanisms[k].key);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
            suit_inputs->mechanisms[k].cose_tag = CBOR_TAG_COSE_SIGN1;
            suit_inputs->mechanisms[k].use = true;

            // search another empty slot
            for (; k < SUIT_MAX_KEY_NUM; k++) {
                if (!suit_inputs->mechanisms[k].use) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                result = SUIT_ERR_NO_MEMORY;
                goto error;
            }
            suit_inputs->mechanisms[k].key.cose_algorithm_id = T_COSE_ALGORITHM_ECDH_ES_A128KW;
            result = suit_set_suit_key_from_cwt_payload(cwt_payload, &suit_inputs->mechanisms[k].key);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
            suit_inputs->mechanisms[k].cose_tag = CBOR_TAG_COSE_ENCRYPT;
            suit_inputs->mechanisms[k].use = true;
        }
        QCBORDecode_ExitArray(context);
    }
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }
    return SUIT_SUCCESS;

error:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
    if (result != SUIT_ERR_ABORT) {
        suit_report_callback(
            (suit_report_args_t) {
                .level0 = SUIT_DELEGATION,
                .level1.manifest_key = SUIT_MANIFEST_KEY_INVALID,
                .level2.condition_directive = SUIT_CONDITION_INVALID,
                .level3.parameter = SUIT_PARAMETER_INVALID,
                .qcbor_error = error,
                .suit_error = result
            }
        );
        return SUIT_ERR_ABORT;
    }
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_DELEGATION */

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
        else if (item.uLabelType == QCBOR_TYPE_INT64) {
            envelope_key = item.label.int64;
            switch (envelope_key) {
#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DELEGATION)
            case SUIT_DELEGATION:
                result = suit_process_delegation(&context, suit_inputs);
                if (result != SUIT_SUCCESS) {
                    goto error;
                }
                break;
#endif

            case SUIT_AUTHENTICATION:
                result = suit_process_authentication_wrapper(&context, suit_inputs, &manifest_digest);
                if (result != SUIT_SUCCESS) {
                    goto error;
                }

                /*
                 * Check with expected digest
                 * mainly for suit-condition-dependency-integrity
                 */
                if (suit_inputs->expected_manifest_digest.bytes.len > 0) {
                    if (suit_inputs->expected_manifest_digest.bytes.len != manifest_digest.bytes.len ||
                        memcmp(suit_inputs->expected_manifest_digest.bytes.ptr, manifest_digest.bytes.ptr, manifest_digest.bytes.len) != 0) {
                        return SUIT_ERR_FAILED_TO_VERIFY;
                    }
                }
                break;

            case SUIT_MANIFEST:
                if (manifest_digest.algorithm_id == SUIT_ALGORITHM_ID_INVALID) {
                    result = SUIT_ERR_AUTHENTICATION_NOT_FOUND;
                    goto error;
                }
                QCBORDecode_GetNext(&context, &item);
                result = suit_verify_item(&context, &item, &manifest_digest);
                if (result != SUIT_SUCCESS) {
                    goto error;
                }
                extracted.manifest = item.val.string;
                break;

            /* Severed Members */
#if !defined(LIBCSUIT_DISABLE_ENVELOPE_INSTALL)
            case SUIT_SEVERED_INSTALL:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.install)) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.install);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_INSTALL */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
            case SUIT_SEVERED_DEPENDENCY_RESOLUTION:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.dependency_resolution)) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.dependency_resolution);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH)
            case SUIT_SEVERED_PAYLOAD_FETCH:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.payload_fetch)) {
                    result = SUIT_ERR_REDUNDANT;
                    goto error;
                }
                QCBORDecode_GetByteString(&context, &extracted.payload_fetch);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_TEXT)
            case SUIT_SEVERED_TEXT:
                error = QCBORDecode_GetNext(&context, &item);
                if (error != QCBOR_SUCCESS) {
                    goto error;
                }
                // XXX: ignore this
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_TEXT */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_COSWID)
            case SUIT_SEVERED_COSWID:
                error = QCBORDecode_GetNext(&context, &item);
                if (error != QCBOR_SUCCESS) {
                    goto error;
                }
                // XXX: ignore this
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_COSWID */

            default:
                result = SUIT_ERR_NOT_IMPLEMENTED;
                goto error;
                break;
            }
        }
        else {
            // XXX: may be QCBOR_TYPE_UINT64 ?
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
    if (result != SUIT_SUCCESS) {
        goto error;
    }
    /* TODO: check digests */

    /* manifest-component-id */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
    if (suit_inputs->process_flags.manifest_component_id) {
        manifest_key = SUIT_MANIFEST_COMPONENT_ID;
        if (extracted.manifest_component_id.len > 0) {
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
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif

    /* dependency-resolution */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
    if (suit_inputs->process_flags.dependency_resolution) {
        manifest_key = SUIT_DEPENDENCY_RESOLUTION;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.dependency_resolution)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_DEPENDENCY_RESOLUTION, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

    /* payload-fetch */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
    if (suit_inputs->process_flags.payload_fetch) {
        manifest_key = SUIT_PAYLOAD_FETCH;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.payload_fetch)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_PAYLOAD_FETCH, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

    /* install */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
    if (suit_inputs->process_flags.install) {
        manifest_key = SUIT_INSTALL;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.install)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_INSTALL, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* LIBCSUIT_DISABLE_MANIFEST_INSTALL */

    /* validate */
    if (suit_inputs->process_flags.validate) {
        manifest_key = SUIT_VALIDATE;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.validate)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_VALIDATE, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }

    /* load */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
    if (suit_inputs->process_flags.load) {
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.load)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_LOAD, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

    /* invoke */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
    if (suit_inputs->process_flags.invoke) {
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.invoke)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_INVOKE, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

    /* uninstall */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
    if (suit_inputs->process_flags.uninstall) {
        manifest_key = SUIT_UNINSTALL;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.uninstall)) {
            result = suit_process_common_and_command_sequence(&extracted, SUIT_UNINSTALL, suit_inputs);
            if (result != SUIT_SUCCESS) {
                goto error;
            }
        }
        else if (!suit_inputs->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto error;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        goto error;
    }

out:
    return result;

error:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
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
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}


