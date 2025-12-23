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
#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_callbacks.h"
#include "csuit/suit_reporting_engine.h"

suit_err_t suit_set_parameters(suit_processor_context_t *processor_context,
                               QCBORDecodeContext *context,
                               const suit_con_dir_key_t directive,
                               const suit_index_t *suit_index,
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
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY;
                    processor_context->parameters[processor_context->component_index].update_priority = val.i64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY */

        /* uint64 */
        case SUIT_PARAMETER_IMAGE_SIZE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_SIZE;
                    processor_context->parameters[processor_context->component_index].image_size = val.u64;
                }
            }
            break;
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
        case SUIT_PARAMETER_SOURCE_COMPONENT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT;
                    processor_context->parameters[processor_context->component_index].source_component = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
        case SUIT_PARAMETER_COMPONENT_SLOT:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT;
                    processor_context->parameters[processor_context->component_index].component_slot = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
        case SUIT_PARAMETER_USE_BEFORE:
            QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_USE_BEFORE;
                    processor_context->parameters[processor_context->component_index].use_before = val.u64;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
        case SUIT_PARAMETER_MINIMUM_BATTERY:
             QCBORDecode_GetUInt64(context, &val.u64);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY;
                    processor_context->parameters[processor_context->component_index].minimum_battery = val.u64;
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
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_SOFT_FAILURE) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_SOFT_FAILURE;
                    processor_context->parameters[processor_context->component_index].soft_failure = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER)
        case SUIT_PARAMETER_STRICT_ORDER:
            QCBORDecode_GetBool(context, &val.b);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_STRICT_ORDER) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_STRICT_ORDER;
                    processor_context->parameters[processor_context->component_index].strict_order = (val.b) ? SUIT_PARAMETER_TRUE : SUIT_PARAMETER_FALSE;
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
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_URI) ||
                    directive == SUIT_DIRECTIVE_OVERRIDE_PARAMETERS) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_URI;
                    processor_context->parameters[processor_context->component_index].uri = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].vendor_id = val.str;
                }
            }
            break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].class_id = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].device_id = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
        case SUIT_PARAMETER_CONTENT:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_CONTENT;
                    processor_context->parameters[processor_context->component_index].content = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CONTENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS)
        case SUIT_PARAMETER_INVOKE_ARGS:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_INVOKE_ARGS) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_INVOKE_ARGS;
                    processor_context->parameters[processor_context->component_index].invoke_args = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
        case SUIT_PARAMETER_FETCH_ARGS:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_FETCH_ARGS;
                    processor_context->parameters[processor_context->component_index].fetch_args = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO)
        /* bstr .cbor COSE_Encrypt0 // bstr .cbor COSE_Encrypt */
        case SUIT_PARAMETER_ENCRYPTION_INFO:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO;
                    processor_context->parameters[processor_context->component_index].encryption_info = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO */

        /* bstr .cbor SUIT_Diget */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST;
                    processor_context->parameters[processor_context->component_index].image_digest_buf = val.str;
                }
            }
            break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
        /* bstr .cbor SUIT_Parameter_Version_Match */
        case SUIT_PARAMETER_VERSION:
            QCBORDecode_GetByteString(context, &val.str);
            // QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
            // result = suit_decode_version_match_from_item(context, &item, true, &val.str);
            // QCBORDecode_ExitBstrWrapped(context);
            if (result == SUIT_SUCCESS) {
                for (size_t j = 0; j < suit_index->len; j++) {
                    processor_context->component_index = suit_index->index[j];
                    if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_VERSION) || override) {
                        processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_VERSION;
                        processor_context->parameters[processor_context->component_index].version_match_buf = val.str;
                    }
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_VERSION */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
        /* bstr .cbor SUIT_Wait_Event */
        case SUIT_PARAMETER_WAIT_INFO:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_WAIT_INFO;
                    processor_context->parameters[processor_context->component_index].wait_info_buf = val.str;
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
        /* bstr .cbor SUIT_Component_Metadata */
        case SUIT_PARAMETER_COMPONENT_METADATA:
            QCBORDecode_GetByteString(context, &val.str);
            for (size_t j = 0; j < suit_index->len; j++) {
                processor_context->component_index = suit_index->index[j];
                if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) || override) {
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA;
                    processor_context->parameters[processor_context->component_index].component_metadata_buf = val.str;
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
            result = suit_error_from_qcbor_error(error);
            goto error;
        }
    }

    QCBORDecode_ExitMap(context);
    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

error:
    return result;
}

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE)
suit_err_t suit_override_multiple(suit_processor_context_t *processor_context,
                                  QCBORDecodeContext *context,
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
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        suit_index.index[0] = item.label.int64;
        suit_index.len = 1;
        result = suit_set_parameters(processor_context, context, SUIT_DIRECTIVE_OVERRIDE_MULTIPLE, &suit_index, may_soft_failure);
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


// returns one of SUIT_SUCCESS, SUIT_ERR_COMPONENT_NOT_FOUND, SUIT_ERR_NO_MEMORY
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
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        // fallthrough
    case QCBOR_TYPE_UINT64:
        QCBORDecode_GetUInt64(context, &val.u64);
        if (val.u64 > UINT8_MAX) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
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
                return SUIT_ERR_COMPONENT_NOT_FOUND;
            }
            suit_index->index[i] = (uint8_t)val.u64;
        }
        QCBORDecode_ExitArray(context);
        break;
#endif /* !LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY */

    default:
        return SUIT_ERR_COMPONENT_NOT_FOUND;
    }

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return SUIT_ERR_COMPONENT_NOT_FOUND;
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

void suit_set_consumed_parameters(suit_processor_context_t *processor_context,
                                  suit_callback_ret_t *ret)
{
    processor_context->reason = ret->reason;

    // NOTE: parameter key may updated by the caller
    if (ret->consumed_parameter_keys[0] != SUIT_PARAMETER_INVALID) {
        processor_context->parameter_keys[0] = ret->consumed_parameter_keys[0];
        switch (ret->consumed_parameter_keys[0]) {
        /* int */
        case SUIT_PARAMETER_UPDATE_PRIORITY:
            processor_context->parameter_value.i64 = processor_context->parameters[processor_context->component_index].update_priority;
            break;
        /* uint */
        case SUIT_PARAMETER_IMAGE_SIZE:
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].image_size;
            if (ret->consumed_parameter_keys[1] == SUIT_PARAMETER_IMAGE_DIGEST) {
                processor_context->parameter_keys[1] = SUIT_PARAMETER_IMAGE_DIGEST;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].image_digest_buf;
            }
            break;
        case SUIT_PARAMETER_COMPONENT_SLOT:
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].component_slot;
            break;
        case SUIT_PARAMETER_SOURCE_COMPONENT:
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].source_component;
            if (ret->consumed_parameter_keys[1] == SUIT_PARAMETER_ENCRYPTION_INFO) {
                processor_context->parameter_keys[1] = SUIT_PARAMETER_ENCRYPTION_INFO;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].encryption_info;
            }
            break;
        case SUIT_PARAMETER_USE_BEFORE:
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].use_before;
            break;
        case SUIT_PARAMETER_MINIMUM_BATTERY:
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].minimum_battery;
            break;
        /* tstr */
        case SUIT_PARAMETER_URI:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].uri;
            break;
        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].vendor_id;
            break;
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].class_id;
            break;
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].device_id;
            break;
        case SUIT_PARAMETER_CONTENT:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].content;
            break;
        case SUIT_PARAMETER_INVOKE_ARGS:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].invoke_args;
            break;
        case SUIT_PARAMETER_FETCH_ARGS:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].fetch_args;
            break;
        
        /* bstr .cbor SUIT_Digest */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].image_digest_buf;
            if (ret->consumed_parameter_keys[1] == SUIT_PARAMETER_IMAGE_SIZE) {
                processor_context->parameter_keys[1] = SUIT_PARAMETER_IMAGE_SIZE;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].image_size;
            }
            break;
        /* bstr .cbor SUIT_Encryption_Info */
        case SUIT_PARAMETER_ENCRYPTION_INFO:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].encryption_info;
            if (ret->consumed_parameter_keys[1] == SUIT_PARAMETER_SOURCE_COMPONENT) {
                processor_context->parameter_keys[1] = SUIT_PARAMETER_SOURCE_COMPONENT;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].source_component;
            }
            break;

        /* bstr .cbor SUIT_Parameter_Version_Match */
        case SUIT_PARAMETER_VERSION:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].version_match_buf;
            break;
        /* bstr .cbor SUIT_Wait_Event */
        case SUIT_PARAMETER_WAIT_INFO:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].wait_info_buf;
            break;
        /* bstr .cbor SUIT_Component_Metadata */
        case SUIT_PARAMETER_COMPONENT_METADATA:
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].component_metadata_buf;
            break;

        /* bool */
        case SUIT_PARAMETER_STRICT_ORDER:
            processor_context->parameter_value.b = processor_context->parameters[processor_context->component_index].strict_order;
            break;
        case SUIT_PARAMETER_SOFT_FAILURE:
            processor_context->parameter_value.b = processor_context->parameters[processor_context->component_index].soft_failure;
            break;

        case SUIT_PARAMETER_INVALID:
            break;
        }
    }
}

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_FETCH)
suit_err_t suit_process_fetch(suit_processor_context_t *processor_context,
                              suit_extracted_t *extracted,
                              const suit_index_t *suit_index,
                              const suit_rep_policy_t report_policy)
{
    suit_err_t result = SUIT_SUCCESS;

    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];
        if (processor_context->parameters[processor_context->component_index].uri.len >= SUIT_MAX_NAME_LENGTH) {
            return SUIT_ERR_NO_MEMORY;
        }
        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }

        processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].uri;
        suit_payload_t *payload = suit_key_to_payload(extracted, processor_context->parameters[processor_context->component_index].uri);
        if (payload == NULL) {
            if (extracted->payloads.len >= SUIT_MAX_ARRAY_LENGTH) {
                return SUIT_ERR_NO_MEMORY;
            }
            size_t buf_size = (processor_context->parameters[processor_context->component_index].image_size > 0) ? processor_context->parameters[processor_context->component_index].image_size : processor_context->left_len;
            if (buf_size > processor_context->left_len) {
                return SUIT_ERR_NO_MEMORY;
            }

            suit_fetch_args_t fetch = {0};
            suit_callback_ret_t fret = {0};
            fetch.dst = *processor_context->component;

            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                fetch.encryption_info = processor_context->parameters[processor_context->component_index].encryption_info;
                memcpy(fetch.mechanisms, processor_context->mechanisms, sizeof(fetch.mechanisms));
            }
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
                fetch.component_metadata_buf = processor_context->parameters[processor_context->component_index].component_metadata_buf;
            }

            /* the size of uri is already checked at suit-directive-override-parameters */
            memcpy(fetch.uri, processor_context->parameters[processor_context->component_index].uri.ptr, processor_context->parameters[processor_context->component_index].uri.len);
            fetch.uri[processor_context->parameters[processor_context->component_index].uri.len] = '\0';
            fetch.uri_len = processor_context->parameters[processor_context->component_index].uri.len + 1;

            fetch.args = processor_context->parameters[processor_context->component_index].fetch_args;

            fetch.buf_len = buf_size;
            fetch.ptr = processor_context->allocated.ptr + (processor_context->allocated.len - processor_context->left_len);

            fetch.report_policy = report_policy;

            /* store the fetched payload into fetch.ptr */
            result = suit_fetch_callback(fetch, &fret);
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
            suit_set_consumed_parameters(processor_context, &fret);
#endif
            if (result != SUIT_SUCCESS) {
                return result;
            }
            if (fetch.buf_len < fret.buf_len) {
                return SUIT_ERR_NO_MEMORY;
            }
            processor_context->left_len -= fret.buf_len;

            /* register fetched payload */
            payload = &extracted->payloads.payload[extracted->payloads.len];
            extracted->payloads.len++;
            payload->bytes.ptr = fetch.ptr;
            payload->bytes.len = fret.buf_len;

            payload->key = processor_context->parameters[processor_context->component_index].uri;
            payload->index = processor_context->component_index;
        }
        else {
            /* already handled with integrated-payload or integrated-dependency */
            suit_store_args_t store = {0};
            store.report_policy = report_policy;
            store.dst = *processor_context->component;
            store.src_buf = payload->bytes;
            store.operation = SUIT_STORE;
            suit_callback_ret_t ret = {0};

            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                store.encryption_info = processor_context->parameters[processor_context->component_index].encryption_info;
            }
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
                store.component_metadata_buf = processor_context->parameters[processor_context->component_index].component_metadata_buf;
            }
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS) {
                store.fetch_args = processor_context->parameters[processor_context->component_index].fetch_args;
            }
            result = suit_store_callback(store, &ret);
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
            suit_set_consumed_parameters(processor_context, &ret);
#endif
            if (result != SUIT_SUCCESS) {
                return result;
            }
            payload->key = processor_context->parameters[processor_context->component_index].uri;
            payload->index = processor_context->component_index;
        }
    }
    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_FETCH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_INVOKE)
suit_err_t suit_process_invoke(suit_processor_context_t *processor_context,
                               const suit_extracted_t *extracted,
                               const suit_rep_policy_t report_policy,
                               const suit_index_t *suit_index)
{
    suit_err_t result = SUIT_SUCCESS;

    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];

        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }

        memset(&processor_context->parameter_value, 0, sizeof(suit_union_parameter_t));
        memset(processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));

        suit_invoke_args_t invoke = {0};
        invoke.report_policy = report_policy;
        invoke.component_identifier = *processor_context->component;
        invoke.args_len = processor_context->parameters[processor_context->component_index].invoke_args.len;
        if (invoke.args_len > 0) {
            memcpy(invoke.args, processor_context->parameters[processor_context->component_index].invoke_args.ptr, invoke.args_len);
            processor_context->parameter_keys[0] = SUIT_PARAMETER_INVOKE_ARGS;
            processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].invoke_args;
        }

#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
        if (processor_context->reporting_engine != NULL && processor_context->u.report_invoke_pending && !processor_context->u.reported) {
            suit_report_finalize(
                processor_context->reporting_engine,
                processor_context->final_state,
                SUIT_REPORT_REASON_INVOKE_PENDING,
                processor_context->dependency_tree,
                processor_context->manifest_key,
                processor_context->section_offset,
                processor_context->parameter_keys,
                &processor_context->parameter_value
            );
            suit_report_args_t args = {0};
            args.suit_report = processor_context->reporting_engine->suit_report;
            suit_report_callback(args);
            processor_context->u.reported = 1;
        }
#endif /* LIBCSUIT_DISABLE_SUIT_REPORT */

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
    // treat that the dependency manifest is also a component
    return SUIT_ERR_COMPONENT_NOT_FOUND;
}
#endif /* !LIBCSUIT_DISABLE_COMMON_DEPENDENCIES */

suit_err_t suit_process_condition(suit_processor_context_t *processor_context,
                                  suit_extracted_t *extracted,
                                  const suit_index_t *suit_index,
                                  suit_rep_policy_t report_policy)
{
    suit_err_t result = SUIT_SUCCESS;

    for (uint8_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];
        // $$SUIT_Parameter will be set by the condition callbacks
        memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));

        bool callback_required = true;
        suit_condition_args_t args = {0};
        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        args.dst = *processor_context->component;
        args.condition = processor_context->condition_or_directive;

        switch (args.condition) {
        /* uint64 */
#if !defined(LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT)
        case SUIT_CONDITION_COMPONENT_SLOT:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT) {
                args.expected.u64 = processor_context->parameters[processor_context->component_index].component_slot;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].component_slot;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_CONDITION_USE_BEFORE)
        case SUIT_CONDITION_USE_BEFORE:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE) {
                args.expected.u64 = processor_context->parameters[processor_context->component_index].use_before;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].use_before;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY)
        case SUIT_CONDITION_MINIMUM_BATTERY:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY) {
                args.expected.u64 = processor_context->parameters[processor_context->component_index].minimum_battery;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].minimum_battery;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY */

#if !defined(LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED)
        case SUIT_CONDITION_UPDATE_AUTHORIZED:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY) {
                args.expected.i64 = processor_context->parameters[processor_context->component_index].update_priority;
                processor_context->parameter_value.i64 = processor_context->parameters[processor_context->component_index].update_priority;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED */

        /* bstr */
        case SUIT_CONDITION_VENDOR_IDENTIFIER:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER) {
                args.expected.str = processor_context->parameters[processor_context->component_index].vendor_id;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].vendor_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

#if !defined(LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER)
        case SUIT_CONDITION_CLASS_IDENTIFIER:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER) {
                args.expected.str = processor_context->parameters[processor_context->component_index].class_id;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].class_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER)
        case SUIT_CONDITION_DEVICE_IDENTIFIER:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER) {
                args.expected.str = processor_context->parameters[processor_context->component_index].device_id;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].device_id;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT)
        case SUIT_CONDITION_CHECK_CONTENT:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT) {
                args.expected.str = processor_context->parameters[processor_context->component_index].content;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].content;
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
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE) {
                args.expected.u64 = processor_context->parameters[processor_context->component_index].image_size;
                processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].image_size;
            }
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST) {
                args.expected.str = processor_context->parameters[processor_context->component_index].image_digest_buf;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].image_digest_buf;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;

        /* draft-ietf-suit-trust-domains */
#if !defined(LIBCSUIT_DISABLE_CONDITION_VERSION)
        case SUIT_CONDITION_VERSION:
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_VERSION) {
                args.expected.str = processor_context->parameters[processor_context->component_index].version_match_buf;
                processor_context->parameter_value.str = processor_context->parameters[processor_context->component_index].version_match_buf;
            }
            else {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_VERSION */

#if !defined(LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY)
        case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
            callback_required = false;
            result = suit_index_is_dependency(extracted, processor_context->component_index);
            if (result != SUIT_SUCCESS) {
                return result;
            }

            if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST)) {
                return SUIT_ERR_PARAMETER_NOT_FOUND;
            }
            suit_payload_t *payload = suit_index_to_payload(extracted, processor_context->component_index);
            if (payload == NULL) {
                return SUIT_ERR_COMPONENT_NOT_FOUND;
            }

            // OK, let's dive into the dependency manifest
            processor_context->dependency_tree.manifest_index[processor_context->dependency_tree.len] = processor_context->component_index;
            processor_context->dependency_tree.len++;
            result = suit_decode_digest(processor_context->parameters[processor_context->component_index].image_digest_buf, &processor_context->expected_manifest_digest);
            if (result != SUIT_SUCCESS) {
                processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                return result;
            }

            UsefulBufC manifest_backup = processor_context->manifest;
            processor_context->manifest = payload->bytes;

            // checks only suit-delegation and suit-authentication-wrapper
            suit_process_flag_t process_flags_backup = processor_context->process_flags;
            processor_context->process_flags.all = 0;

            result = suit_process_envelope(processor_context);

            // recover the context
            processor_context->process_flags = process_flags_backup;
            processor_context->manifest = manifest_backup;
            processor_context->expected_manifest_digest.algorithm_id = 0;
            processor_context->dependency_tree.len--;
            break;

#endif /* !LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY */

        case SUIT_CONDITION_ABORT:
            callback_required = false;
            result = SUIT_ERR_ABORT;
            break;

#if !defined(LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY)
        case SUIT_CONDITION_IS_DEPENDENCY:
            callback_required = false;
            result = suit_index_is_dependency(extracted, processor_context->component_index);
            break;
#endif /* !LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY */

        case SUIT_CONDITION_INVALID:
        default:
            callback_required = false;
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }

        if (callback_required) {
            args.report_policy = report_policy;
            suit_callback_ret_t ret = {0};
            result = suit_condition_callback(args, &ret);
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
            suit_set_consumed_parameters(processor_context, &ret);
#endif
        }

#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
        if (processor_context->reporting_engine != NULL) {
            if (result == SUIT_SUCCESS) {
                if (report_policy.record_on_success) {
                    suit_report_extend_system_property_claims(
                        processor_context->reporting_engine,
                        processor_context->component_index,
                        processor_context->component,
                        processor_context->parameter_keys,
                        &processor_context->parameter_value
                    );
                }
                memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));
            }
            else if (result != SUIT_ERR_ABORT) {
                if (report_policy.record_on_failure) {
                    suit_report_extend_record(
                        processor_context->reporting_engine,
                        processor_context->dependency_tree,
                        processor_context->manifest_key,
                        processor_context->section_offset,
                        processor_context->parameter_keys,
                        &processor_context->parameter_value
                    );
                }
                memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));
            }
        }
#endif

        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return result;
}

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WAIT)
suit_err_t suit_process_wait(suit_processor_context_t *processor_context,
                             suit_extracted_t *extracted,
                             const suit_index_t *suit_index,
                             suit_rep_policy_t report_policy)
{
    suit_err_t result = SUIT_SUCCESS;

    for (uint8_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];

        suit_wait_args_t wait_args = {0};
        if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO)) {
            return SUIT_ERR_PARAMETER_NOT_FOUND;
        }
        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        wait_args.dst = *processor_context->component;
        wait_args.wait_info_buf = processor_context->parameters[processor_context->component_index].wait_info_buf;
        wait_args.report_policy = report_policy;

        result = suit_wait_callback(wait_args);
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return result;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WAIT */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WRITE)
suit_err_t suit_process_write(suit_processor_context_t *processor_context,
                              const suit_extracted_t *extracted,
                              const suit_index_t *suit_index,
                              const suit_rep_policy_t report_policy)
{
    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];

        suit_store_args_t store = {0};
        if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT)) {
            return SUIT_ERR_PARAMETER_NOT_FOUND;
        }
        store.src_buf = processor_context->parameters[processor_context->component_index].content;
        if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
            store.encryption_info = processor_context->parameters[processor_context->component_index].encryption_info;
            memcpy(store.mechanisms, processor_context->mechanisms, sizeof(store.mechanisms));
        }
        if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA) {
            store.component_metadata_buf = processor_context->parameters[processor_context->component_index].component_metadata_buf;
        }
        store.report_policy = report_policy;
        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        store.dst = *processor_context->component;
        store.operation = SUIT_STORE;

        suit_callback_ret_t ret = {0};
        suit_err_t result = suit_store_callback(store, &ret);
        processor_context-> reason = ret.reason;
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WRITE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY) || \
    !defined(LIBCSUIT_DISABLE_DIRECTIVE_SWAP)
suit_err_t suit_process_copy_swap(suit_processor_context_t *processor_context,
                                  const suit_extracted_t *extracted,
                                  bool is_swap,
                                  const suit_index_t *suit_index,
                                  const suit_rep_policy_t report_policy)
{
    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];

        suit_store_args_t store = {0};
        store.report_policy = report_policy;

        if (is_swap) {
            store.operation = SUIT_SWAP;
        }
        else {
            store.operation = SUIT_COPY;
            if (processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO) {
                store.encryption_info = processor_context->parameters[processor_context->component_index].encryption_info;
                for (size_t j = 0; j < SUIT_MAX_KEY_NUM; j++) {
                    if (processor_context->mechanisms[j].use &&
                        (processor_context->mechanisms[j].cose_tag == CBOR_TAG_COSE_ENCRYPT ||
                         processor_context->mechanisms[j].cose_tag == CBOR_TAG_COSE_ENCRYPT0)) {
                        store.mechanisms[j] = processor_context->mechanisms[j];
                    }
                }
            }
        }

        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->parameters[processor_context->component_index].source_component);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        store.src = *processor_context->component;
        const suit_component_identifier_t *tmp = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (tmp == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        store.dst = *tmp;

        suit_callback_ret_t ret = {0};
        suit_err_t result = suit_store_callback(store, &ret);
        processor_context->reason = ret.reason;
        if (ret.on_src) {
            processor_context->parameter_keys[0] = SUIT_PARAMETER_SOURCE_COMPONENT;
            processor_context->parameter_value.u64 = processor_context->parameters[processor_context->component_index].source_component;
        }
        if (result != SUIT_SUCCESS) {
            if (result == SUIT_ERR_FAILED_TO_DECRYPT) {
                processor_context->parameter_keys[0] = SUIT_PARAMETER_ENCRYPTION_INFO;
                processor_context->parameter_value.str = store.encryption_info;
            }
            return result;
        }
    }
    return SUIT_SUCCESS;
}
#endif

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_UNLINK)
suit_err_t suit_process_unlink(suit_processor_context_t *processor_context,
                               const suit_extracted_t *extracted,
                               const suit_index_t *suit_index,
                               const suit_rep_policy_t report_policy)
{

    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];
        processor_context->component = suit_index_to_component_identifier(extracted, processor_context->component_index);
        if (processor_context->component == NULL) {
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        suit_store_args_t store = {0};
        store.report_policy = report_policy;
        store.dst = *processor_context->component;
        store.operation = SUIT_UNLINK;

        suit_callback_ret_t ret = {0};
        suit_err_t result = suit_store_callback(store, &ret);
        processor_context->reason = ret.reason;
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }
    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_UNLINK */

suit_err_t suit_process_command_sequence_buf(suit_processor_context_t *processor_context,
                                             suit_extracted_t *extracted,
                                             UsefulBufC buf,
                                             suit_index_t *suit_index,
                                             const bool may_soft_failure);

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
suit_err_t suit_process_run_sequence(suit_processor_context_t *processor_context,
                                     QCBORDecodeContext *context,
                                     suit_extracted_t *extracted,
                                     suit_index_t *suit_index)
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
        processor_context->parameters[suit_index->index[i]].soft_failure = false;

        suit_err_t result = suit_process_command_sequence_buf(processor_context, extracted, buf, &tmp_suit_index, true);
        if (result != SUIT_SUCCESS && !processor_context->parameters[suit_index->index[i]].soft_failure) {
            return result;
        }
    }

    return SUIT_SUCCESS;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
suit_err_t suit_process_try_each(suit_processor_context_t *processor_context,
                                 QCBORDecodeContext *context,
                                 suit_extracted_t *extracted,
                                 suit_index_t *suit_index)
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
                    processor_context->parameters[suit_index->index[j]].soft_failure = true;

                    result = suit_process_command_sequence_buf(processor_context, extracted, item.val.string, &tmp_suit_index, true);
                    if (result == SUIT_SUCCESS) {
                        done_any = true;
                        done = true;
                    }
                    else if (!processor_context->parameters[suit_index->index[j]].soft_failure) {
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

#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
    if (processor_context->reporting_engine != NULL) {
        if (!done_any) {
            suit_report_extend_record(
                processor_context->reporting_engine,
                processor_context->dependency_tree,
                processor_context->manifest_key,
                processor_context->section_offset,
                processor_context->parameter_keys,
                &processor_context->parameter_value
            );
        }
        memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));
    }
#endif /* LIBCSUIT_SUIT_REPORT */
    return (done_any) ? SUIT_SUCCESS : SUIT_ERR_TRY_OUT;
}
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS)
suit_err_t suit_process_copy_params(suit_processor_context_t *processor_context,
                                    QCBORDecodeContext *context,
                                    suit_extracted_t *extracted,
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
            return SUIT_ERR_COMPONENT_NOT_FOUND;
        }
        const uint8_t src_index = item.label.int64;
        size_t len = item.val.uCount;
        for (size_t j = 0; j < len; j++) {
            int64_t parameter;
            QCBORDecode_GetInt64(context, &parameter);
            for (size_t k = 0; k < suit_index->len; k++) {
                processor_context->component_index = suit_index->index[k];
                switch (parameter) {
                /* int64 */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
                case SUIT_PARAMETER_UPDATE_PRIORITY:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY;
                    processor_context->parameters[processor_context->component_index].update_priority = processor_context->parameters[src_index].update_priority;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY */

                /* uint64 */
                case SUIT_PARAMETER_IMAGE_SIZE:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_IMAGE_SIZE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_SIZE;
                    processor_context->parameters[processor_context->component_index].image_size = processor_context->parameters[src_index].image_size;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
                case SUIT_PARAMETER_SOURCE_COMPONENT:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT;
                    processor_context->parameters[processor_context->component_index].source_component = processor_context->parameters[src_index].source_component;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
                case SUIT_PARAMETER_COMPONENT_SLOT:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT;
                    processor_context->parameters[processor_context->component_index].component_slot = processor_context->parameters[src_index].component_slot;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
                case SUIT_PARAMETER_USE_BEFORE:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_USE_BEFORE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_USE_BEFORE;
                    processor_context->parameters[processor_context->component_index].use_before = processor_context->parameters[src_index].use_before;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
                case SUIT_PARAMETER_MINIMUM_BATTERY:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY;
                    processor_context->parameters[processor_context->component_index].minimum_battery = processor_context->parameters[src_index].minimum_battery;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY */

                /* bool */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE)
                case SUIT_PARAMETER_SOFT_FAILURE:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_SOFT_FAILURE)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_SOFT_FAILURE;
                    processor_context->parameters[processor_context->component_index].soft_failure = processor_context->parameters[src_index].soft_failure;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER)
                case SUIT_PARAMETER_STRICT_ORDER:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_STRICT_ORDER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_STRICT_ORDER;
                    processor_context->parameters[processor_context->component_index].strict_order = processor_context->parameters[src_index].strict_order;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER */

                /* tstr */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_URI)
                case SUIT_PARAMETER_URI:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_URI)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_URI;
                    processor_context->parameters[processor_context->component_index].uri = processor_context->parameters[src_index].uri;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_URI */

                /* bstr */
                case SUIT_PARAMETER_VENDOR_IDENTIFIER:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].vendor_id = processor_context->parameters[src_index].vendor_id;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
                case SUIT_PARAMETER_CLASS_IDENTIFIER:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].class_id = processor_context->parameters[src_index].class_id;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
                case SUIT_PARAMETER_DEVICE_IDENTIFIER:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER;
                    processor_context->parameters[processor_context->component_index].device_id = processor_context->parameters[src_index].device_id;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
                case SUIT_PARAMETER_CONTENT:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_CONTENT)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_CONTENT;
                    processor_context->parameters[processor_context->component_index].content = processor_context->parameters[src_index].content;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_CONTENT */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS)
                case SUIT_PARAMETER_INVOKE_ARGS:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_INVOKE_ARGS)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_INVOKE_ARGS;
                    processor_context->parameters[processor_context->component_index].invoke_args = processor_context->parameters[src_index].invoke_args;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
                case SUIT_PARAMETER_FETCH_ARGS:
                    if (!(processor_context->parameters[processor_context->component_index].exists & SUIT_PARAMETER_CONTAINS_FETCH_ARGS)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_FETCH_ARGS;
                    processor_context->parameters[processor_context->component_index].fetch_args = processor_context->parameters[src_index].fetch_args;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS */

                /* bstr .cbor COSE_Encrypt0 // bstr .cbor COSE_Encrypt */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO)
                case SUIT_PARAMETER_ENCRYPTION_INFO:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO;
                    processor_context->parameters[processor_context->component_index].encryption_info = processor_context->parameters[src_index].encryption_info;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO */

                /* bstr .cbor SUIT_Diget */
                case SUIT_PARAMETER_IMAGE_DIGEST:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST;
                    processor_context->parameters[processor_context->component_index].image_digest_buf = processor_context->parameters[src_index].image_digest_buf;
                    break;

#if !defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
                /* SUIT_Parameter_Version_Match */
                case SUIT_PARAMETER_VERSION:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_VERSION)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_VERSION;
                    processor_context->parameters[processor_context->component_index].version_match_buf = processor_context->parameters[src_index].version_match_buf;
                    break;
#endif

#if !defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
                /* SUIT_Wait_Event */
                case SUIT_PARAMETER_WAIT_INFO:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_CONTAINS_WAIT_INFO)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_WAIT_INFO;
                    processor_context->parameters[processor_context->component_index].wait_info_buf = processor_context->parameters[src_index].wait_info_buf;
                    break;
#endif /* !LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO */

#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
                /* SUIT_Component_Metadata */
                case SUIT_PARAMETER_COMPONENT_METADATA:
                    if (!(processor_context->parameters[src_index].exists & SUIT_PARAMETER_COMPONENT_METADATA)) {
                        return SUIT_ERR_PARAMETER_NOT_FOUND;
                    }
                    processor_context->parameters[processor_context->component_index].exists |= SUIT_PARAMETER_CONTAINS_COMPONENT_METADATA;
                    processor_context->parameters[processor_context->component_index].component_metadata_buf = processor_context->parameters[src_index].component_metadata_buf;
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

suit_err_t suit_process_dependency(suit_processor_context_t *processor_context,
                                   suit_extracted_t *extracted,
                                   const suit_index_t *suit_index,
                                   suit_rep_policy_t report_policy)
{
    suit_err_t result = SUIT_SUCCESS;

    for (size_t i = 0; i < suit_index->len; i++) {
        processor_context->component_index = suit_index->index[i];

        result = suit_index_is_dependency(extracted, processor_context->component_index);
        if (result != SUIT_SUCCESS) {
            goto report;
        }

        suit_payload_t *payload = suit_index_to_payload(extracted, processor_context->component_index);
        if (payload == NULL) {
            result = SUIT_ERR_COMPONENT_NOT_FOUND;
            goto report;
        }

        // OK, let's dive into the dependency manifest
        processor_context->dependency_tree.manifest_index[processor_context->dependency_tree.len] = processor_context->component_index;
        processor_context->dependency_tree.len++;
        result = suit_decode_digest(processor_context->parameters[processor_context->component_index].image_digest_buf, &processor_context->expected_manifest_digest);
        if (result != SUIT_SUCCESS) {
            processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
            return result;
        }

        UsefulBufC manifest_backup = processor_context->manifest;
        processor_context->manifest = payload->bytes;

        // checks only suit-delegation and suit-authentication-wrapper
        suit_process_flag_t process_flags_backup = processor_context->process_flags;
        processor_context->process_flags = suit_manifest_key_to_process_flag(processor_context->manifest_key);

        /*
        * Calls "second manifest processor" defined in
        * https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-02#section-6.4.1-3
        * by limiting set of operations (processor_context->manifest_key).
        */
        result = suit_process_envelope(processor_context);

        // recover the context
        processor_context->process_flags = process_flags_backup;
        processor_context->manifest = manifest_backup;
        processor_context->dependency_tree.len--;

report:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
        if (processor_context->reporting_engine != NULL) {
            if (result == SUIT_SUCCESS) {
                if (report_policy.record_on_success) {
                    suit_report_extend_system_property_claims(
                        processor_context->reporting_engine,
                        processor_context->component_index,
                        processor_context->component,
                        processor_context->parameter_keys,
                        &processor_context->parameter_value
                    );
                }
                memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));
            }
            else if (result != SUIT_ERR_ABORT) {
                if (report_policy.record_on_failure) {
                    suit_report_extend_record(
                        processor_context->reporting_engine,
                        processor_context->dependency_tree,
                        processor_context->manifest_key,
                        processor_context->section_offset,
                        processor_context->parameter_keys,
                        &processor_context->parameter_value
                    );
                }
                memset(&processor_context->parameter_keys, 0, sizeof(processor_context->parameter_keys));
            }
        }
#endif
        if (result != SUIT_SUCCESS) {
            break;
        }
    }

    return result;
}

suit_err_t suit_process_command_sequence_buf(suit_processor_context_t *processor_context,
                                             suit_extracted_t *extracted,
                                             UsefulBufC buf,
                                             suit_index_t *suit_index,
                                             const bool may_soft_failure)
{
    suit_err_t result = SUIT_SUCCESS;
    suit_rep_policy_t report_policy;
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
        goto out;
    }
    for (size_t i = 0; i < length; i += 2) {
        processor_context->section_offset = UsefulInputBuf_Tell(&context.InBuf);
        result = suit_qcbor_get_next_uint(&context, &item);
        if (result != SUIT_SUCCESS) {
            goto out;
        }
        processor_context->condition_or_directive = item.val.uint64;
        switch (processor_context->condition_or_directive) {
        case SUIT_DIRECTIVE_SET_COMPONENT_INDEX:
            result = suit_set_index(&context, extracted, suit_index);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS)
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(processor_context, &context, SUIT_DIRECTIVE_SET_PARAMETERS, suit_index, may_soft_failure);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS */

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(processor_context, &context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, suit_index, may_soft_failure);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE)
        case SUIT_DIRECTIVE_OVERRIDE_MULTIPLE:
            result = suit_override_multiple(processor_context, &context, may_soft_failure);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS)
        case SUIT_DIRECTIVE_COPY_PARAMS:
            result = suit_process_copy_params(processor_context, &context, extracted, suit_index);
            break;
#endif

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_FETCH)
        case SUIT_DIRECTIVE_FETCH:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_fetch(processor_context, extracted, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_FETCH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WRITE)
        case SUIT_DIRECTIVE_WRITE:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_write(processor_context, extracted, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WRITE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_COPY)
        case SUIT_DIRECTIVE_COPY:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_copy_swap(processor_context, extracted, false, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_COPY */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SWAP)
       case SUIT_DIRECTIVE_SWAP:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_copy_swap(processor_context, extracted, true, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_SWAP */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_UNLINK)
        case SUIT_DIRECTIVE_UNLINK:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_unlink(processor_context, extracted, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_UNLINK */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_INVOKE)
        case SUIT_DIRECTIVE_INVOKE:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_invoke(processor_context, extracted, report_policy, suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_INVOKE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_WAIT)
        case SUIT_DIRECTIVE_WAIT:
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_wait(processor_context, extracted, suit_index, report_policy);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_WAIT */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
        case SUIT_DIRECTIVE_TRY_EACH:
            result = suit_process_try_each(processor_context, &context, extracted, suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            result = suit_process_run_sequence(processor_context, &context, extracted, suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY)
        case SUIT_DIRECTIVE_PROCESS_DEPENDENCY:
            QCBORDecode_GetUInt64(&context, &val.u64);
            result = suit_process_dependency(processor_context, extracted, suit_index, report_policy);
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
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_condition(processor_context, extracted, suit_index, report_policy);
            break;

        case SUIT_DIRECTIVE_INVALID:
        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }

        if (result != SUIT_SUCCESS) {
            // XXX: how do we handle the soft-failure is True?
            goto out;
        }
        error = QCBORDecode_GetError(&context);
        if (error != QCBOR_SUCCESS) {
            goto out;
        }
    }
    QCBORDecode_ExitArray(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        goto out;
    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

out:
    return result;
}

suit_err_t suit_process_shared_sequence(suit_processor_context_t *processor_context,
                                        suit_extracted_t *extracted)
{
    if (extracted->shared_sequence.len == 0) {
        return SUIT_SUCCESS;
    }
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_index_t suit_index = (suit_index_t) {.len = 1, .index[0] = 0};
    suit_rep_policy_t report_policy;

    QCBORDecode_Init(&context, extracted->shared_sequence, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterArray(&context, &item);
    if (item.uDataType != QCBOR_TYPE_ARRAY) {
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    size_t length = item.val.uCount;
    if (length % 2 != 0) {
        return SUIT_ERR_NO_MORE_ITEMS;
    }
    for (size_t i = 0; i < length; i += 2) {
        processor_context->section_offset = UsefulInputBuf_Tell(&context.InBuf);

        result = suit_qcbor_get_next_uint(&context, &item);
        if (result != SUIT_SUCCESS) {
            return result;
        }
        processor_context->condition_or_directive = item.val.uint64;
        switch (processor_context->condition_or_directive) {
        case SUIT_DIRECTIVE_SET_COMPONENT_INDEX:
            result = suit_set_index(&context, extracted, &suit_index);
            break;

        case SUIT_DIRECTIVE_OVERRIDE_PARAMETERS:
            result = suit_set_parameters(processor_context, &context, SUIT_DIRECTIVE_OVERRIDE_PARAMETERS, &suit_index, false);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS)
        case SUIT_DIRECTIVE_SET_PARAMETERS:
            result = suit_set_parameters(processor_context, &context, SUIT_DIRECTIVE_SET_PARAMETERS, &suit_index, false);
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
            QCBORDecode_GetUInt64(&context, &report_policy.val);
            result = suit_process_condition(processor_context, extracted, &suit_index, report_policy);
            break;

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH)
        case SUIT_DIRECTIVE_TRY_EACH:
            result = suit_process_try_each(processor_context, &context, extracted, &suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH */

#if !defined(LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE)
        case SUIT_DIRECTIVE_RUN_SEQUENCE:
            result = suit_process_run_sequence(processor_context, &context, extracted, &suit_index);
            break;
#endif /* !LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE */

        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
        }
        if (result != SUIT_SUCCESS) {
            return result;
        }
    }


    QCBORDecode_ExitArray(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

    return result;
}

suit_err_t suit_process_common_and_command_sequence(suit_processor_context_t *processor_context,
                                                    suit_extracted_t *extracted)
{
    suit_err_t result = SUIT_SUCCESS;
    for (size_t i = 0; i < SUIT_MAX_INDEX_NUM; i++) {
        processor_context->parameters[i].soft_failure = true;
    }

    UsefulBufC command_buf;
    switch (processor_context->manifest_key) {
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

    result = suit_process_shared_sequence(processor_context, extracted);
    if (result != SUIT_SUCCESS) {
        return result;
    }

    suit_index_t suit_index = {.len = 1, .index[0] = 0};
    result = suit_process_command_sequence_buf(processor_context, extracted, command_buf, &suit_index, false);
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
                                               suit_processor_context_t *processor_context,
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
            switch (processor_context->mechanisms[j].cose_tag) {
            case COSE_SIGN1_TAG:
                result = suit_verify_cose_sign1(signature, &processor_context->mechanisms[j].key, &digest_buf);
                break;
            case COSE_MAC0_TAG:
                result = suit_validate_cose_mac0(signature, &processor_context->mechanisms[j].key, &digest_buf);
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

#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
    if (verified && processor_context->reporting_engine != NULL) {
        suit_report_manifest_digest(
            processor_context->reporting_engine,
            *digest
        );
    }
#endif /* LIBCSUIT_DISABLE_SUIT_REPORT */

    return (verified) ? SUIT_SUCCESS : SUIT_ERR_FAILED_TO_VERIFY;
}

suit_err_t suit_extract_common(suit_processor_context_t *processor_context,
                               QCBORDecodeContext *context,
                               suit_extracted_t *extracted)
{
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;
    suit_err_t result = SUIT_SUCCESS;
    suit_common_key_t common_key = SUIT_COMMON_KEY_INVALID;

    processor_context->manifest_key = SUIT_COMMON;

    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterMap(context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
        return SUIT_ERR_INVALID_TYPE_OF_VALUE;
    }
    size_t length = item.val.uCount;
    uint8_t tmp_len = 0;
    for (size_t i = 0; i < length; i++) {
        error = QCBORDecode_PeekNext(context, &item);
        if (error != QCBOR_SUCCESS) {
            processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
            return suit_error_from_qcbor_error(error);
        }
        else if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
            return SUIT_ERR_INVALID_TYPE_OF_KEY;
        }
        common_key = item.label.int64;
        switch (common_key) {
#if !defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
        case SUIT_DEPENDENCIES:
            result = suit_decode_dependencies_from_item(context, &item, true, &extracted->dependencies);
            for (size_t j = 0; j < extracted->dependencies.len; j++) {
                uint8_t index = extracted->dependencies.dependency[j].index;
                if (index > SUIT_MAX_INDEX_NUM) {
                    processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                    return SUIT_ERR_NO_MEMORY;
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
            result = suit_decode_components_from_item(context, &item, true, extracted->components, &tmp_len);
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
            processor_context->reason = suit_report_reason_from_suit_err(result);
            return result;
        }
    }
    QCBORDecode_ExitMap(context);
    QCBORDecode_ExitBstrWrapped(context);

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        return suit_error_from_qcbor_error(error);
    }

    return result;
}


suit_err_t suit_extract_manifest(suit_processor_context_t *processor_context, suit_extracted_t *extracted)
{
    suit_err_t result = SUIT_SUCCESS;
    QCBORDecodeContext context;
    QCBORItem item;
    QCBORError error = QCBOR_SUCCESS;

    // though this is not any Command Sequence, for better SUIT Reporting
    processor_context->manifest_key = (suit_manifest_key_t)SUIT_AUTHENTICATION;
    suit_manifest_key_t manifest_key = SUIT_MANIFEST_KEY_INVALID;

    QCBORDecode_Init(&context, extracted->manifest, QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
        goto out;
    }
    size_t manifest_key_len = item.val.uCount;
    for (size_t i = 0; i < manifest_key_len; i++) {
        error = QCBORDecode_PeekNext(&context, &item);
        if (error != QCBOR_SUCCESS) {
            goto out;
        }
        if (!(item.uLabelType == QCBOR_TYPE_INT64 || item.uLabelType == QCBOR_TYPE_UINT64)) {
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto out;
        }
        if (item.label.int64 <= manifest_key) {
            result = SUIT_ERR_NOT_CANONICAL_CBOR;
            goto out;
        }
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
            if (manifest_key < SUIT_REFERENCE_URI && SUIT_REFERENCE_URI < item.label.int64) {
                // no suit-reference-uri
                if (processor_context->reporting_engine != NULL) {
                    suit_report_manifest_reference_uri(
                        processor_context->reporting_engine,
                        NULLUsefulBufC
                    );
                }
            }
#endif /* LIBCSUIT_DISABLE_SUIT_REPORT */

        manifest_key = item.label.int64;
        switch (manifest_key) {
        case SUIT_MANIFEST_VERSION:
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto out;
            }
            if (!(item.uDataType == QCBOR_TYPE_INT64 || item.uDataType == QCBOR_TYPE_UINT64)) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto out;
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
                goto out;
            }
            if (!(item.uDataType == QCBOR_TYPE_INT64 || item.uDataType == QCBOR_TYPE_UINT64)) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto out;
            }
            // TODO: check sequence-number
            break;

        case SUIT_COMMON:
            result = suit_extract_common(processor_context, &context, extracted);
            if (result != SUIT_SUCCESS) {
                goto out;
            }
            break;

#if !defined(LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI)
        case SUIT_REFERENCE_URI:
            QCBORDecode_GetNext(&context, &item);
            if (!(item.uDataType == QCBOR_TYPE_TEXT_STRING)) {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                goto out;
            }
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
            if (processor_context->reporting_engine != NULL) {
                suit_report_manifest_reference_uri(
                    processor_context->reporting_engine,
                    item.val.string
                );
            }
#endif /* LIBCSUIT_DISABLE_SUIT_REPORT */
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
        case SUIT_MANIFEST_COMPONENT_ID:
            result = suit_decode_component_identifiers_from_item(
                &context, &item, true,
                &extracted->manifest_component_id);
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_SET_VERSION)
        case SUIT_SET_VERSION: // unseverable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_EnterBstrWrapped(&context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
                QCBORDecode_EnterArray(&context, &item);
                if (item.val.uCount > SUIT_MAX_ARRAY_LENGTH) {
                    result = SUIT_ERR_NO_MEMORY;
                }
                else {
                    extracted->set_version.len = item.val.uCount;
                    for (size_t i = 0; i < extracted->set_version.len; i++) {
                        QCBORDecode_GetInt64(&context, &extracted->set_version.int64[i]);
                    }
                    QCBORDecode_ExitArray(&context);
                    QCBORDecode_ExitBstrWrapped(&context);
                }
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_SET_VERSION */

        case SUIT_VALIDATE: // unseverable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->validate);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;

#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
        case SUIT_DEPENDENCY_RESOLUTION: // severable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->dependency_resolution);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(&context, &item, true, &extracted->dependency_resolution_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
        case SUIT_PAYLOAD_FETCH: // severable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->payload_fetch);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(&context, &item, true, &extracted->payload_fetch_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
        case SUIT_INSTALL: // severable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->install);
            }
            else if (item.uDataType == QCBOR_TYPE_ARRAY) {
                result = suit_decode_digest_from_item(&context, &item, true, &extracted->install_digest);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INSTALL */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
        case SUIT_UNINSTALL: // unseverable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->uninstall);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
        case SUIT_LOAD: // unseverable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->load);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
        case SUIT_INVOKE: // unseverable
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING) {
                QCBORDecode_GetByteString(&context, &extracted->invoke);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
        case SUIT_TEXT: // severable
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto out;
            }
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING || item.uDataType == QCBOR_TYPE_ARRAY) {
                // ignore
                suit_qcbor_skip_any(&context, &item);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_TEXT */

#if !defined(LIBCSUIT_DISABLE_MANIFEST_COSWID)
        case SUIT_COSWID: // severable
            error = QCBORDecode_GetNext(&context, &item);
            if (error != QCBOR_SUCCESS) {
                goto out;
            }
            if (item.uDataType == QCBOR_TYPE_BYTE_STRING || item.uDataType == QCBOR_TYPE_ARRAY) {
                // ignore
                suit_qcbor_skip_any(&context, &item);
            }
            else {
                result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
            }
            break;
#endif /* !LIBCSUIT_DISABLE_MANIFEST_COSWID */

        default:
            result = SUIT_ERR_NOT_IMPLEMENTED;
            goto out;
        }

    }

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

out:
    return result;
}

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DELEGATION)
suit_err_t suit_process_delegation(QCBORDecodeContext *context,
                                   suit_processor_context_t *processor_context)
{
    suit_err_t result;
    QCBORError error = QCBOR_SUCCESS;
    QCBORItem item;
    // though this is not any Command Sequence, for better SUIT Reporting
    processor_context->manifest_key = (suit_manifest_key_t)SUIT_DELEGATION;

    QCBORDecode_EnterBstrWrapped(context, QCBOR_TAG_REQUIREMENT_NOT_A_TAG, NULL);
    QCBORDecode_EnterArray(context, &item);
    if (item.val.uCount > SUIT_MAX_KEY_NUM) {
        return SUIT_ERR_NO_MEMORY;
    }
    const size_t delegation_chain_num = item.val.uCount;
    for (size_t i = 0; i < delegation_chain_num; i++) {
        QCBORDecode_EnterArray(context, &item);
        if (item.val.uCount > SUIT_MAX_KEY_NUM) {
            return SUIT_ERR_NO_MEMORY;
        }
        const size_t num = item.val.uCount;
        for (size_t j = 0; j < num; j++) {
            UsefulBufC cwt;
            QCBORDecode_GetByteString(context, &cwt);
            UsefulBufC cwt_payload;
            size_t k = 0;
            for (; k < SUIT_MAX_KEY_NUM; k++) {
                if (!processor_context->mechanisms[k].use) {
                    continue;
                }
                cwt_payload = NULLUsefulBufC;
                switch (processor_context->mechanisms[k].cose_tag) {
                case COSE_SIGN1_TAG:
                    result = suit_verify_cose_sign1(cwt, &processor_context->mechanisms[k].key, &cwt_payload);
                    break;
                case COSE_MAC0_TAG:
                    result = suit_validate_cose_mac0(cwt, &processor_context->mechanisms[k].key, &cwt_payload);
                    break;
                default:
                    continue;
                }
                if (result == SUIT_SUCCESS) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                return SUIT_ERR_FAILED_TO_VERIFY_DELEGATION;
            }

            // search empty slot
            for (k = 0; k < SUIT_MAX_KEY_NUM; k++) {
                if (!processor_context->mechanisms[k].use) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                return SUIT_ERR_NO_MEMORY;
            }
            processor_context->mechanisms[k].key.cose_algorithm_id = T_COSE_ALGORITHM_ES256;
            result = suit_set_suit_key_from_cwt_payload(cwt_payload, &processor_context->mechanisms[k].key);
            if (result != SUIT_SUCCESS) {
                return result;
            }
            processor_context->mechanisms[k].cose_tag = CBOR_TAG_COSE_SIGN1;
            processor_context->mechanisms[k].use = true;

            // search another empty slot
            for (; k < SUIT_MAX_KEY_NUM; k++) {
                if (!processor_context->mechanisms[k].use) {
                    break;
                }
            }
            if (k == SUIT_MAX_KEY_NUM) {
                return SUIT_ERR_NO_MEMORY;
            }
            processor_context->mechanisms[k].key.cose_algorithm_id = T_COSE_ALGORITHM_ECDH_ES_A128KW;
            result = suit_set_suit_key_from_cwt_payload(cwt_payload, &processor_context->mechanisms[k].key);
            if (result != SUIT_SUCCESS) {
                return result;
            }
            processor_context->mechanisms[k].cose_tag = CBOR_TAG_COSE_ENCRYPT;
            processor_context->mechanisms[k].use = true;
        }
        QCBORDecode_ExitArray(context);
    }
    QCBORDecode_ExitArray(context);
    QCBORDecode_ExitBstrWrapped(context);

    error = QCBORDecode_GetError(context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

    return result;
}
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_DELEGATION */

/*
    Public function. See suit_manifest_process.h
 */
suit_err_t suit_process_envelope(suit_processor_context_t *processor_context)
{

    QCBORDecodeContext context;
    QCBORError error = QCBOR_SUCCESS;
    QCBORItem item;
    suit_err_t result = SUIT_SUCCESS;

    suit_envelope_key_t envelope_key = SUIT_ENVELOPE_KEY_INVALID;
    suit_digest_t manifest_digest;
    suit_extracted_t extracted = {0};

    if (!processor_context->u.initialized ||
        !processor_context->u.manifest_loaded ||
        !processor_context->u.recipient_key_loaded) {
        result = SUIT_ERR_NOT_INITIALIZED;
        goto report;
    }

    /* extract items */
    QCBORDecode_Init(&context,
                     (UsefulBufC){processor_context->manifest.ptr, processor_context->manifest.len},
                     QCBOR_DECODE_MODE_NORMAL);
    QCBORDecode_EnterMap(&context, &item);
    size_t length = item.val.uCount;
    for (size_t i = 0; i < length; i++) {
        error = QCBORDecode_PeekNext(&context, &item);
        if (error != QCBOR_SUCCESS) {
            goto report;
        }
        if (item.uLabelType == QCBOR_TYPE_TEXT_STRING) {
            /* integrated-payload */
            envelope_key = SUIT_INTEGRATED_PAYLOAD;
            if (extracted.payloads.len >= SUIT_MAX_ARRAY_LENGTH) {
                result = SUIT_ERR_NO_MEMORY;
                goto report;
            }
            extracted.payloads.payload[extracted.payloads.len].key = item.label.string;
            QCBORDecode_GetByteString(&context, &extracted.payloads.payload[extracted.payloads.len].bytes);
            extracted.payloads.len++;
        }
        else if (item.uLabelType == QCBOR_TYPE_INT64) {
            if (item.label.int64 <= envelope_key) {
                result = SUIT_ERR_NOT_CANONICAL_CBOR;
                goto report;
            }
            envelope_key = item.label.int64;
            switch (envelope_key) {
#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DELEGATION)
            case SUIT_DELEGATION:
                result = suit_process_delegation(&context, processor_context);
                if (result != SUIT_SUCCESS) {
                    processor_context->final_state = result;
                    goto report;
                }
                break;
#endif

            case SUIT_AUTHENTICATION:
                result = suit_process_authentication_wrapper(&context, processor_context, &manifest_digest);
                if (result != SUIT_SUCCESS) {
                    processor_context->final_state = result;
                    goto report;
                }

                /*
                 * Check with expected digest
                 * mainly for suit-condition-dependency-integrity
                 */
                if (processor_context->expected_manifest_digest.bytes.len > 0) {
                    if (processor_context->expected_manifest_digest.bytes.len != manifest_digest.bytes.len ||
                        memcmp(processor_context->expected_manifest_digest.bytes.ptr, manifest_digest.bytes.ptr, manifest_digest.bytes.len) != 0) {
                        result = SUIT_ERR_FAILED_TO_VERIFY;
                        processor_context->reason = SUIT_REPORT_REASON_UNAUTHORIZED;
                        goto report;
                    }
                }
                break;

            case SUIT_MANIFEST:
                if (manifest_digest.algorithm_id == SUIT_ALGORITHM_ID_INVALID) {
                    result = SUIT_ERR_AUTHENTICATION_NOT_FOUND;
                    processor_context->reason = SUIT_REPORT_REASON_UNAUTHORIZED;
                    goto report;
                }
                QCBORDecode_GetNext(&context, &item);
                result = suit_verify_item(&context, &item, &manifest_digest);
                if (result != SUIT_SUCCESS) {
                    processor_context->reason = SUIT_REPORT_REASON_UNAUTHORIZED;
                    goto report;
                }
                extracted.manifest = item.val.string;
                break;

            /* Severed Members */
#if !defined(LIBCSUIT_DISABLE_ENVELOPE_INSTALL)
            case SUIT_SEVERED_INSTALL:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.install)) {
                    result = SUIT_ERR_REDUNDANT;
                    processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                    goto report;
                }
                QCBORDecode_GetByteString(&context, &extracted.install);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_INSTALL */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION)
            case SUIT_SEVERED_DEPENDENCY_RESOLUTION:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.dependency_resolution)) {
                    result = SUIT_ERR_REDUNDANT;
                    processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                    goto report;
                }
                QCBORDecode_GetByteString(&context, &extracted.dependency_resolution);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH)
            case SUIT_SEVERED_PAYLOAD_FETCH:
                if (!UsefulBuf_IsNULLOrEmptyC(extracted.payload_fetch)) {
                    result = SUIT_ERR_REDUNDANT;
                    processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                    goto report;
                }
                QCBORDecode_GetByteString(&context, &extracted.payload_fetch);
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_TEXT)
            case SUIT_SEVERED_TEXT:
                error = QCBORDecode_GetNext(&context, &item);
                if (error != QCBOR_SUCCESS) {
                    result = suit_error_from_qcbor_error(error);
                    processor_context->reason = SUIT_REPORT_REASON_CBOR_PARSE;
                    goto report;
                }
                if (item.uDataType != QCBOR_TYPE_BYTE_STRING) {
                    result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                    goto report;
                }
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_TEXT */

#if !defined(LIBCSUIT_DISABLE_ENVELOPE_COSWID)
            case SUIT_SEVERED_COSWID:
                error = QCBORDecode_GetNext(&context, &item);
                if (error != QCBOR_SUCCESS) {
                    goto report;
                }
                if (item.uDataType != QCBOR_TYPE_BYTE_STRING) {
                    result = SUIT_ERR_INVALID_TYPE_OF_VALUE;
                    goto report;
                }
                break;
#endif /* !LIBCSUIT_DISABLE_ENVELOPE_COSWID */

            default:
                result = SUIT_ERR_NOT_IMPLEMENTED;
                goto report;
                break;
            }
        }
        else {
            // XXX: may be QCBOR_TYPE_UINT64 ?
            result = SUIT_ERR_INVALID_TYPE_OF_KEY;
            goto report;
        }
    }
    QCBORDecode_ExitMap(&context);
    error = QCBORDecode_Finish(&context);
    if (error != QCBOR_SUCCESS) {
        result = SUIT_ERR_NOT_A_SUIT_MANIFEST;
        goto report;
    }

    result = suit_extract_manifest(processor_context, &extracted);
    if (result != SUIT_SUCCESS) {
        goto report;
    }
    /* TODO: check digests */

    /* manifest-component-id */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
    if (processor_context->process_flags.manifest_component_id) {
        processor_context->manifest_key = SUIT_MANIFEST_COMPONENT_ID;
        if (extracted.manifest_component_id.len > 0) {
            suit_store_args_t store = {0};
            store.report_policy.val = 0;
            store.dst = extracted.manifest_component_id;
            store.src_buf = processor_context->manifest;
            store.operation = SUIT_STORE;

            suit_callback_ret_t ret = {0};
            result = suit_store_callback(store, &ret);
            processor_context->reason = ret.reason;
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif

    /* set-version */
#if !defined(LIBCSUIT_DISABLE_SET_VERSION)
    if (processor_context->process_flags.set_version) {
        processor_context->manifest_key = SUIT_SET_VERSION;
        if (extracted.set_version.len > 0) {
            result = suit_set_version_callback(extracted.set_version);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif

    /* dependency-resolution */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
    if (processor_context->process_flags.dependency_resolution) {
        processor_context->manifest_key = SUIT_DEPENDENCY_RESOLUTION;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.dependency_resolution)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION */

    /* payload-fetch */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
    if (processor_context->process_flags.payload_fetch) {
        processor_context->manifest_key = SUIT_PAYLOAD_FETCH;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.payload_fetch)) {
            processor_context->manifest_key = SUIT_PAYLOAD_FETCH;
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH */

    /* install */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
    if (processor_context->process_flags.install) {
        processor_context->manifest_key = SUIT_INSTALL;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.install)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* LIBCSUIT_DISABLE_MANIFEST_INSTALL */

    /* validate */
    if (processor_context->process_flags.validate) {
        processor_context->manifest_key = SUIT_VALIDATE;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.validate)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }

    /* load */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
    if (processor_context->process_flags.load) {
        processor_context->manifest_key = SUIT_LOAD;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.load)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_LOAD */

    /* invoke */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
    if (processor_context->process_flags.invoke) {
        processor_context->manifest_key = SUIT_INVOKE;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.invoke)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_INVOKE */

    /* uninstall */
#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
    if (processor_context->process_flags.uninstall) {
        processor_context->manifest_key = SUIT_UNINSTALL;
        if (!UsefulBuf_IsNULLOrEmptyC(extracted.uninstall)) {
            result = suit_process_common_and_command_sequence(processor_context, &extracted);
            if (result != SUIT_SUCCESS) {
                goto report;
            }
        }
        else if (!processor_context->process_flags.allow_missing) {
            result = SUIT_ERR_MANIFEST_KEY_NOT_FOUND;
            goto report;
        }
    }
#endif /* !LIBCSUIT_DISABLE_MANIFEST_UNINSTALL */

    error = QCBORDecode_GetError(&context);
    if (error != QCBOR_SUCCESS) {
        result = suit_error_from_qcbor_error(error);
    }

report:
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
    if (processor_context->reporting_engine != NULL) {
        // call only if this Processor context is top of the dependency
        if (processor_context->dependency_tree.len == 0 && !processor_context->u.reported) {
            suit_report_finalize(
                processor_context->reporting_engine,
                processor_context->final_state,
                processor_context->reason,
                processor_context->dependency_tree,
                processor_context->manifest_key,
                processor_context->section_offset,
                processor_context->parameter_keys,
                &processor_context->parameter_value
            );
            suit_report_args_t args = {0};
            args.suit_report = processor_context->reporting_engine->suit_report;
            suit_report_callback(args);
            processor_context->u.reported = 1;
        }
    }
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
    return result;
}

suit_err_t suit_processor_add_manifest(
    suit_processor_context_t *processor_context,
    UsefulBufC manifest,
    suit_process_flag_t process_flags)
{
    if (!processor_context->u.initialized) {
        return SUIT_ERR_NOT_INITIALIZED;
    }
    if (processor_context->u.manifest_loaded) {
        return SUIT_ERR_REDUNDANT;
    }
    if (manifest.ptr != processor_context->manifest.ptr) {
        return SUIT_ERR_INVALID_VALUE;
    }
    if (manifest.len > processor_context->manifest.len) {
        return SUIT_ERR_INVALID_VALUE;
    }

    processor_context->manifest = manifest;
    processor_context->left_len -= manifest.len;
    processor_context->process_flags = process_flags;
    processor_context->u.manifest_loaded = 1;

    return SUIT_SUCCESS;
}

suit_err_t suit_processor_add_recipient_key(
    suit_processor_context_t *processor_context,
    int cose_tag,
    int cose_algorithm_id,
    UsefulBufC cose_key)
{
    if (!processor_context->u.initialized) {
        return SUIT_ERR_NOT_INITIALIZED;
    }

    switch (cose_tag) {
    case CBOR_TAG_COSE_SIGN1:
    case CBOR_TAG_COSE_SIGN:
    case CBOR_TAG_COSE_MAC0:
    case CBOR_TAG_COSE_MAC:
    case CBOR_TAG_COSE_ENCRYPT:
        break;
    // draft-ietf-suit-firmware-encryption does not mention this
    // case CBOR_TAG_COSE_ENCRYPT0:
    default:
        return SUIT_ERR_NOT_IMPLEMENTED;
    }

    size_t i = 0;
    for (; i < SUIT_MAX_KEY_NUM; i++) {
        if (processor_context->mechanisms[i].cose_tag == COSE_TAG_INVALID) {
            break;
        }
    }
    if (i == SUIT_MAX_KEY_NUM) {
        return SUIT_ERR_NO_MEMORY;
    }

    suit_err_t result = suit_set_suit_key_from_cose_key(cose_key, &processor_context->mechanisms[i].key);
    if (result != SUIT_SUCCESS) {
        return result;
    }
    if (processor_context->mechanisms[i].key.cose_algorithm_id == T_COSE_ALGORITHM_RESERVED) {
        processor_context->mechanisms[i].key.cose_algorithm_id = cose_algorithm_id;
    }
    processor_context->mechanisms[i].use = true;
    processor_context->mechanisms[i].cose_tag = cose_tag;

    processor_context->u.recipient_key_loaded = 1;

    return SUIT_SUCCESS;
}

suit_err_t suit_processor_init(
    suit_processor_context_t *processor_context,
    size_t buf_size,
    suit_report_context_t *report_context,
    bool report_invoke_pending,
    UsefulBuf *manifest)
{
    memset(processor_context, 0, sizeof(processor_context) + buf_size);
    // just to be sure
    processor_context->final_state = SUIT_SUCCESS;
    processor_context->component = NULL;

    // NULL is allowed, but it should be started if exists
    if (report_context != NULL && report_context->state != SUIT_REPORTING_ENGINE_STARTED) {
        return SUIT_ERR_NOT_INITIALIZED;
    }
    processor_context->reporting_engine = report_context;
    if (report_invoke_pending) {
        processor_context->u.report_invoke_pending = 1;
    }

    processor_context->allocated.ptr = processor_context->buf;
    processor_context->allocated.len = buf_size;
    processor_context->left_len = buf_size;

    processor_context->manifest.ptr = processor_context->buf;
    processor_context->manifest.len = buf_size;
    *manifest = UsefulBuf_Unconst(processor_context->manifest);

    processor_context->u.initialized = 1;
    return SUIT_SUCCESS;
}

void suit_processor_free(suit_processor_context_t *processor_context)
{
    if (processor_context == NULL) {
        return;
    }
    for (size_t i = 0; i < SUIT_MAX_KEY_NUM; i++) {
        suit_free_key(&processor_context->mechanisms[i].key);
        processor_context->mechanisms[i] = (suit_mechanism_t){0};
    }
    processor_context->u.status = 0;
}
