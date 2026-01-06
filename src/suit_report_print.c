/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_report_print.c

    \brief  This implements SUIT Report printing

    Call these functions if you want to print the encoded SUIT Report.
 */

#include "csuit/suit_manifest_print.h"

void suit_print_suit_parameters_from_item(QCBORDecodeContext *context,
                                          QCBORItem *item,
                                          bool in_system_property,
                                          uint32_t indent_space,
                                          uint32_t indent_delta)
{
    bool comma = false;
    union {
        suit_component_identifier_t component_identifier;
        suit_digest_t digest;
        suit_version_match_t version_match;
        suit_wait_event_t wait_info;
        suit_component_metadata_t component_metadata;
    }v;

    if (item->uDataType != QCBOR_TYPE_MAP) {
        return;
    }
    printf("{\n");
    size_t map_count = item->val.uCount;
    for (size_t i = 0; i < map_count; i++) {
        QCBORDecode_GetNext(context, item);
        if (item->uLabelType != QCBOR_TYPE_INT64) {
            return;
        }
        suit_parameter_key_t label = item->label.int64;
        switch (label) {
        /* int */
        case SUIT_PARAMETER_UPDATE_PRIORITY:
            if (item->uDataType != QCBOR_TYPE_INT64) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: %ld", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label, item->val.int64);
            comma = true;
            break;

        /* uint */
        case SUIT_PARAMETER_USE_BEFORE:
        case SUIT_PARAMETER_COMPONENT_SLOT:
        case SUIT_PARAMETER_IMAGE_SIZE:
        case SUIT_PARAMETER_SOURCE_COMPONENT:
        case SUIT_PARAMETER_MINIMUM_BATTERY:
            if (item->uDataType != QCBOR_TYPE_INT64) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: %ld", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label, item->val.int64);
            comma = true;
            break;

        /* bool */
        case SUIT_PARAMETER_STRICT_ORDER:
        case SUIT_PARAMETER_SOFT_FAILURE:
            if (item->uDataType != QCBOR_TYPE_TRUE && item->uDataType != QCBOR_TYPE_FALSE) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: %s", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label, (item->uDataType == QCBOR_TYPE_TRUE) ? "true" : "false");
            comma = true;
            break;

        /* bstr */
        case SUIT_PARAMETER_VENDOR_IDENTIFIER:
        case SUIT_PARAMETER_CLASS_IDENTIFIER:
        case SUIT_PARAMETER_FETCH_ARGS:
        case SUIT_PARAMETER_CONTENT:
        case SUIT_PARAMETER_INVOKE_ARGS:
        case SUIT_PARAMETER_DEVICE_IDENTIFIER:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            suit_print_hex(item->val.string.ptr, item->val.string.len);
            comma = true;
            break;

        /* tstr */
        case SUIT_PARAMETER_URI:
            if (item->uDataType != QCBOR_TYPE_TEXT_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            suit_print_string(item->val.string);
            comma = true;
            break;

        /* bstr .cbor SUIT_Digest */
        case SUIT_PARAMETER_IMAGE_DIGEST:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            if (suit_decode_digest(item->val.string, &v.digest) != SUIT_SUCCESS) {
                return;
            }
            suit_print_digest(&v.digest, indent_space + indent_delta, indent_delta);
            comma = true;
            break;

        /* bstr .cbor SUIT_Encryption_Info */
        case SUIT_PARAMETER_ENCRYPTION_INFO:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            suit_print_encryption_info(item->val.string, indent_space + indent_delta, indent_delta);
            comma = true;
            break;

        /* bstr .cbor SUIT_Version */
        case SUIT_PARAMETER_VERSION:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            if (suit_decode_version_match(item->val.string, &v.version_match) != SUIT_SUCCESS) {
                return;
            }
            suit_print_version_match(&v.version_match, indent_space + indent_delta, indent_delta);
            comma = true;
            break;

        /* bstr .cbor SUIT_Wait_Event */
        case SUIT_PARAMETER_WAIT_INFO:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            if (suit_decode_wait_event(item->val.string, &v.wait_info) != SUIT_SUCCESS) {
                return;
            }
            suit_print_wait_event(&v.wait_info, indent_space + indent_delta, indent_delta);
            comma = true;
            break;

        /* bstr .cbor SUIT_Component_Metadata */
        case SUIT_PARAMETER_COMPONENT_METADATA:
            if (item->uDataType != QCBOR_TYPE_BYTE_STRING) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ %s / %d: ", indent_space + indent_delta, "", suit_parameter_key_to_str(label), label);
            if (suit_decode_component_metadata(item->val.string, &v.component_metadata) != SUIT_SUCCESS) {
                return;
            }
            suit_print_component_metadata(&v.component_metadata, indent_space + indent_delta, indent_delta);
            break;

        case 0:
            if (!in_system_property) {
                // invalid parameter
                return;
            }
            // system-component-id
            if (item->uDataType != QCBOR_TYPE_ARRAY) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ system-component-id / 0: ", indent_space + indent_delta, "");
            if (suit_decode_component_identifiers_from_item(context, item, false, &v.component_identifier)) {
                return;
            }
            suit_print_component_identifier(&v.component_identifier);
            comma = true;
            break;
        }
    }
    printf("\n%*s}", indent_space, "");
}

void suit_print_system_property_claims_from_item(QCBORDecodeContext *context,
                                                 QCBORItem *item,
                                                 uint32_t indent_space,
                                                 uint32_t indent_delta)
{
    if (item->uDataType != QCBOR_TYPE_MAP) {
        return;
    }
    suit_print_suit_parameters_from_item(context, item, true, indent_space, indent_delta);
}

void suit_print_record_from_item(QCBORDecodeContext *context,
                                 QCBORItem *item,
                                 uint32_t indent_space,
                                 uint32_t indent_delta)
{
    if (item->uDataType != QCBOR_TYPE_ARRAY || item->val.uCount < 5 ) {
        return;
    }
    size_t ext_len = 5 - item->val.uCount;
    printf("[\n");

    QCBORDecode_GetNext(context, item);
    if (item->uDataType != QCBOR_TYPE_ARRAY) {
        return;
    }
    printf("%*s/ suit-record-manifest-id: / [", indent_space + indent_delta, "");
    size_t array_length = item->val.uCount;
    bool comma = false;
    for (size_t i = 0; i < array_length; i++) {
        QCBORDecode_GetNext(context, item);
        if (item->uDataType != QCBOR_TYPE_INT64) {
            return;
        }
        if (comma) {
            printf(", ");
        }
        printf("%ld", item->val.int64);
    }
    printf("],\n");

    QCBORDecode_GetNext(context, item);
    if (item->uDataType != QCBOR_TYPE_INT64) {
        return;
    }
    printf("%*s/ suit-record-manifest-section: / %ld,\n", indent_space + indent_delta, "", item->val.int64);

    if (suit_qcbor_get_next_uint(context, item) != SUIT_SUCCESS) {
        return;
    }
    printf("%*s/ suit-record-section-offset: / %lu,\n", indent_space + indent_delta, "", item->val.uint64);

    if (suit_qcbor_get_next_uint(context, item) != SUIT_SUCCESS) {
        return;
    }
    printf("%*s/ suit-record-component-index: / %lu,\n", indent_space + indent_delta, "", item->val.uint64);

    QCBORDecode_GetNext(context, item);
    if (item->uDataType != QCBOR_TYPE_MAP) {
        return;
    }
    suit_print_suit_parameters_from_item(context, item, false, indent_space + indent_delta, indent_delta);

    for (size_t i = 0; i < ext_len; i++) {
        QCBORDecode_GetNext(context, item);
        // TODO: $$SUIT_Record_Extensions
    }
    printf("\n%*s]", indent_space, "");
}

const char *suit_report_reason_to_str(suit_report_reason_t reason)
{
    switch (reason) {
    case SUIT_REPORT_REASON_OK:
        return "suit-report-reason-ok";
    case SUIT_REPORT_REASON_CBOR_PARSE:
        return "suit-report-reason-cbor-parse";
    case SUIT_REPORT_REASON_COSE_UNSUPPORTED:
        return "suit-report-reason-cose-unsupported";
    case SUIT_REPORT_REASON_ALG_UNSUPPORTED:
        return "suit-report-reason-alg-unsupported";
    case SUIT_REPORT_REASON_UNAUTHORIZED:
        return "suit-report-reason-unauthorized";
    case SUIT_REPORT_REASON_COMMAND_UNSUPPORTED:
        return "suit-report-reason-command-unsupported";
    case SUIT_REPORT_REASON_COMPONENT_UNSUPPORTED:
        return "suit-report-reason-component-unsupported";
    case SUIT_REPORT_REASON_COMPONENT_UNAUTHORIZED:
        return "suit-report-reason-component-unauthorized";
    case SUIT_REPORT_REASON_PARAMETER_UNSUPPORTED:
        return "suit-report-reason-parameter-unsupported";
    case SUIT_REPORT_REASON_SEVERING_UNSUPPORTED:
        return "suit-report-reason-severing-unsupported";
    case SUIT_REPORT_REASON_CONDITION_FAILED:
        return "suit-report-reason-condition-failed";
    case SUIT_REPORT_REASON_OPERATION_FAILED:
        return "suit-report-reason-operation-failed";
    case SUIT_REPORT_REASON_INVOKE_PENDING:
        return "suit-report-reason-invoke-pending";
    default:
        return "suit-report-reason-unknown";
    }
}

void suit_print_result(QCBORDecodeContext *context,
                       QCBORItem *item,
                       uint32_t indent_space,
                       uint32_t indent_delta)
{
    bool comma = true;
    if (item->uDataType != QCBOR_TYPE_MAP) {
        return;
    }

    printf("{\n");
    size_t map_count = item->val.uCount;
    for (size_t i = 0; i < map_count; i++) {
        QCBORDecode_GetNext(context, item);
        if (item->uLabelType != QCBOR_TYPE_INT64) {
            return;
        }
        switch (item->label.int64) {
        case SUIT_REPORT_RESULT_CODE:
            if (item->uDataType != QCBOR_TYPE_INT64) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ suit-report-result-code / 5: %ld", indent_space, "", item->val.int64);
            comma = true;
            break;
        case SUIT_REPORT_RESULT_RECORD:
            if (item->uDataType != QCBOR_TYPE_ARRAY) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ suit-report-result-record / 6: ", indent_space, "");
            suit_print_record_from_item(context, item, indent_space + indent_delta, indent_delta);
            comma = true;
            break;
        case SUIT_REPORT_RESULT_REASON:
            if (item->uDataType != QCBOR_TYPE_INT64) {
                return;
            }
            if (comma) {
                printf(",\n");
            }
            printf("%*s/ suit-report-result-reason / 7: %ld / %s /", indent_space, "", item->val.int64, suit_report_reason_to_str(item->val.int64));
            comma = true;
            break;
        default:
            // TODO: SUIT_Capability_Report and $$SUIT_Report_Extensions
            return;
        }
    }
}

void suit_print_usefulbufc_report(UsefulBufC suit_report,
                                  uint32_t indent_space,
                                  uint32_t indent_delta)
{
    bool comma = false;
    suit_digest_t digest;

    QCBORDecodeContext context;
    QCBORItem item;
    QCBORDecode_Init(&context, suit_report, QCBOR_DECODE_MODE_NORMAL);

    QCBORDecode_EnterMap(&context, &item);
    if (item.uDataType != QCBOR_TYPE_MAP) {
        return;
    }
    printf("{\n");
    size_t map_count = item.val.uCount;
    for (size_t i = 0; i < map_count; i++) {
        QCBORDecode_GetNext(&context, &item);
        if (item.uLabelType != QCBOR_TYPE_INT64) {
            return;
        }
        switch (item.label.int64) {
        case SUIT_REPORT_REFERENCE:
            {
                if (item.uDataType != QCBOR_TYPE_ARRAY || item.val.uCount != 2) {
                    return;
                }
                if (comma) {
                    printf(",\n");
                }
                printf("%*s/ suit-reference / 99: [\n", indent_space + indent_delta, "");
                printf("%*s/ suit-report-manifest-uri: / ", indent_space + indent_delta * 2, "");
                QCBORDecode_GetNext(&context, &item);
                if (item.uDataType != QCBOR_TYPE_TEXT_STRING) {
                    return;
                }
                suit_print_string(item.val.string);
                printf(",\n%*s/ suit-report-manifest-digest: / ", indent_space + indent_delta * 2, "");
                if (suit_decode_digest_from_item(&context, &item, true, &digest) != SUIT_SUCCESS) {
                    return;
                }
                suit_print_digest(&digest, indent_space + indent_delta * 2, indent_delta);
                printf("\n%*s]", indent_space + indent_delta, "");
                comma = true;
            }
            break;
        case SUIT_REPORT_NONCE:
            {
                if (item.uDataType != QCBOR_TYPE_BYTE_STRING) {
                    return;
                }
                if (comma) {
                    printf(",\n");
                }
                printf("%*s/ suit-report-nonce / 2: ", indent_space + indent_delta, "");
                suit_print_hex(item.val.string.ptr, item.val.string.len);
                comma = true;
            }
            break;
        case SUIT_REPORT_RECORDS:
            {
                if (item.uDataType != QCBOR_TYPE_ARRAY) {
                    return;
                }
                if (comma) {
                    printf(",\n");
                }
                printf("%*s/ suit-report-records / 3: [", indent_space + indent_delta, "");
                size_t array_size = item.val.uCount;
                bool comma_l2 = false;
                for (size_t i = 0; i < array_size; i++) {
                    printf("\n%*s", indent_space + indent_delta * 2, "");
                    QCBORDecode_GetNext(&context, &item);
                    switch (item.uDataType) {
                    case QCBOR_TYPE_ARRAY:
                        // SUIT_Record
                        if (comma_l2) {
                            printf(",\n");
                        }
                        suit_print_record_from_item(&context, &item, indent_space + indent_delta * 2, indent_delta);
                        comma_l2 = true;
                        break;
                    case QCBOR_TYPE_MAP:
                        // system-property-claims
                        if (comma_l2) {
                            printf(",\n");
                        }
                        suit_print_system_property_claims_from_item(&context, &item, indent_space + indent_delta * 2, indent_delta);
                        comma_l2 = true;
                        break;
                    default:
                        return;
                    }
                }
                printf("\n%*s]", indent_space + indent_delta, "");
                comma = true;
            }
            break;
        case SUIT_REPORT_RESULT:
            {
                if (item.uDataType != QCBOR_TYPE_TRUE && item.uDataType != QCBOR_TYPE_MAP) {
                    return;
                }
                if (comma) {
                    printf(",\n");
                }
                printf("%*s/ suit-report-result / 4: ", indent_space + indent_delta, "");
                if (item.uDataType == QCBOR_TYPE_TRUE) {
                    printf("true");
                }
                else if (item.uDataType == QCBOR_TYPE_MAP) {
                    suit_print_result(&context, &item, indent_space + indent_delta * 2, indent_delta);
                }
                comma = true;
            }
            break;
        }
    }
    printf("\n%*s}", indent_space, "");
    QCBORDecode_ExitMap(&context);
}

suit_err_t suit_print_report(suit_report_args_t report_args)
{
    printf("report callback :\n    ");

    cbor_tag_key_t tag = suit_judge_cose_tag_from_buf(report_args.suit_report);
    switch (tag) {
    case 0:
        // raw SUIT_Report
        suit_print_usefulbufc_report(report_args.suit_report, 4, 2);
        break;
    case CBOR_TAG_COSE_SIGN1:
        suit_print_signature(report_args.suit_report, 4, 2, suit_print_usefulbufc_report);
        break;
    default:
        return SUIT_ERR_NOT_IMPLEMENTED;
    }

    printf("\n\ncallback : print SUIT Report SUCCESS\n\n");

    return SUIT_SUCCESS;
}
