/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_callbacks.c

    \brief  This implements libcsuit callbacks for processing

    These callbacks shall be replaced by used defined functions,
    because they requires Storage I/O, Network I/O and other environment dependent operations.
 */

#include "csuit/suit_manifest_encode.h"
#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_callbacks.h"
#include "csuit/suit_manifest_print.h"

/*
 *  Public callback function for such as suit-directive-write. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_store_callback(suit_store_args_t store_args)
{
    return suit_print_store(store_args);
}

/*
 *  Public callback function for suit-directive-fetch. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_fetch_callback(suit_fetch_args_t fetch_args,
                               suit_fetch_ret_t *fetch_ret)
{
    return suit_print_fetch(fetch_args, fetch_ret);
}

/*
 *  Public callback function for such as suit-directive-invoke. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_invoke_callback(suit_invoke_args_t invoke_args)
{
    return suit_print_invoke(invoke_args);
}

/*
 *  Public callback function for suit-condition-*. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_condition_callback(suit_condition_args_t condition_args)
{
    return suit_print_condition(condition_args);
}

/*
 *  Public callback function for suit-set-version. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_set_version_callback(suit_int64_array_t set_version)
{
    return suit_print_set_version(set_version);
}

/*
 *  Public callback function for suit-directive-wait. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_wait_callback(suit_wait_args_t wait_args)
{
    return suit_print_wait(wait_args);
}

/*
 *  Public callback function to create SUIT Report. See csuit/suit_manifest_callbacks.h
 */
#if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
// suit_err_t suit_raw_report(suit_report_args_t *report_args, suit_report_ret_t *report_ret)
// {
//     suit_err_t result = SUIT_SUCCESS;
//     QCBOREncodeContext                  cbor_encoder;
//     QCBORError                          cbor_error;

//     QCBOREncode_Init(&cbor_encoder, report_args->buf);

//     result = suit_encode_raw_report(report_args, &cbor_encoder);
//     if (result != SUIT_SUCCESS) {
//         return result;
//     }

//     cbor_error = QCBOREncode_Finish(&cbor_encoder, &report_ret->encoded);
//     if (cbor_error != QCBOR_SUCCESS) {
//         return SUIT_ERR_FAILED_TO_SIGN;
//     }

//     return SUIT_SUCCESS;
// }

// suit_err_t suit_sign1_report(const suit_report_args_t *report_args, suit_report_ret_t *report_ret)
// {
//     suit_err_t result = SUIT_SUCCESS;

//     // use t_cose two-step sign to save memory
//     QCBOREncodeContext                  cbor_encoder;
//     QCBORError                          cbor_error;
//     struct t_cose_sign_sign_ctx         sign_ctx;
//     struct q_useful_buf_c               payload;
//     struct t_cose_signature_sign_main   signer;

//     QCBOREncode_Init(&cbor_encoder, report_args->buf);

//     t_cose_sign_sign_init(&sign_ctx, T_COSE_OPT_MESSAGE_TYPE_SIGN1);

//     t_cose_signature_sign_main_init(&signer, report_args->inputs.protection_mechanism.key.cose_algorithm_id);

//     t_cose_signature_sign_main_set_signing_key(&signer, report_args->inputs.protection_mechanism.key.cose_key, report_args->inputs.protection_mechanism.kid);

//     t_cose_sign_add_signer(&sign_ctx, t_cose_signature_sign_from_main(&signer));

//     t_cose_err = t_cose_sign_encode_start(&sign_ctx, &cbor_encoder);
//     if (t_cose_err != T_COSE_SUCCESS) {
//         return SUIT_ERR_FAILED_TO_SIGN;
//     }

//     QCBOREncode_BstrWrap(&cbor_encoder);
//     result = suit_encode_raw_reoprt(report_args);
//     if (result != SUIT_SUCCESS) {
//         return result;
//     }
//     QCBOREncode_CloseBstrWrap2(&cbor_encoder, false, &payload);

//     t_cose_err = t_cose_sign_encode_finish(&sign_ctx, NULL_Q_USEFUL_BUF_C, payload, &cbor_encoder);
//     if (t_cose_err != T_COSE_SUCCESS) {
//         return SUIT_ERR_FAILED_TO_SIGN;
//     }

//     cbor_error = QCBOREncode_Finish(&cbor_encoder, &report_ret->encoded);
//     if (cbor_error != QCBOR_SUCCESS) {
//         return SUIT_ERR_FAILED_TO_SIGN;
//     }

//     return SUIT_SUCCESS;
// }

// suit_err_t suit_protect_report(suit_report_args_t report_args, suit_report_ret_t *report_ret)
// {
//     suit_err_t result = SUIT_SUCCESS;
// #if !defined(LIBCSUIT_DISABLE_SUIT_REPORT)
//     switch (report_args.inputs.protection_mechanism.cose_tag) {
//     case CBOR_TAG_COSE_ENCRYPT0:
//         //result = suit_encrypt0_report(report_args)
//         result = SUIT_ERR_NOT_IMPLEMENTED;
//         break;
//     case CBOR_TAG_COSE_MAC0:
//         //result = suit_mac0_report(report_args)
//         result = SUIT_ERR_NOT_IMPLEMENTED;
//         break;
//     case CBOR_TAG_COSE_SIGN1:
//         result = suit_sign1_report(&report_args, report_ret);
//         break;
//     case 0:
//         // raw SUIT_Report
//         result = suit_raw_report(&report_args, report_ret);
//         break;
//     default:
//         result = SUIT_ERR_NOT_IMPLEMENTED;
//     }

// #endif /* LIBCSUIT_DISABLE_SUIT_REPORT */
//     return result;
// }

suit_err_t suit_report_callback(suit_report_args_t report_args,
                                suit_report_ret_t *report_ret)
{
    suit_print_report(report_args);
    return suit_protect_report(report_args, report_ret);
}
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
