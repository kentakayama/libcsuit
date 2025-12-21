/*
    Copyright (c) 2020-2025 SECOM CO., LTD. All Rights reserved.
    
    SPDX-License-Identifier: BSD-2-Clause
 */

 /*!
    \file   suit_reporting_engine.h

    \brief  Declarations of structures and functions for SUIT Reporting Engine
 */

#ifndef SUIT_REPORTING_ENGINE_H
#define SUIT_REPORTING_ENGINE_H

#include "qcbor/UsefulBuf.h"
#include "qcbor/qcbor_encode.h"

#if defined(LIBCSUIT_USE_T_COSE_1)
#include "t_cose/t_cose_common.h"
#else
#include "t_cose/t_cose_sign_sign.h"
#include "t_cose/t_cose_signature_sign_main.h"
#include "t_cose/t_cose_key.h"
#endif /* LIBCSUIT_USE_T_COSE_1 */

#include "csuit/suit_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct suit_report_context;
struct suit_union_parameter;

/*!
    \brief      Initializes the SUIT Reporting Engine

    \param[in]  report_context  Pointer to the Reporting Engine itself.
    \param[in]  buf_size        Allocated buffer size for the Reporting Engine for encoding SUIT Report.

    Call this function first to encode SUIT_Report.
    The size of report_context must be sizeof(suit_report_context_t) + buf_size.
    Optionally, you may call suit_report_add_sender_key() after this function.
    Then, call suit_report_start_encoding().
 */
suit_err_t suit_report_init_engine(
    struct suit_report_context *report_context,
    size_t buf_size);

/*!
    \brief      Registers a SUIT Report private key to the SUIT Reporting Engine

    \param[in]  report_context      Pointer to the Reporting Engine.
    \param[in]  cose_tag            0 (raw), 18 (COSE_Sign1), 17 (COSE_Mac0) or 16 (COSE_Encrypt0).
    \param[in]  cose_algorithm_id   The COSE Algorithm identifier, such as -7 (ES256).
    \param[in]  sender_key          COSE_Key used as a sender key.

    The cose_algorithm_id will be ignored if the cose_key has.
 */
suit_err_t suit_report_add_sender_key(
    struct suit_report_context *report_context,
    const int cose_tag,
    int cose_algorithm_id,
    suit_key_t *sender_key);

/*!
    \brief      Starts the SUIT Reporting Engine

    \param[in]  report_context      Pointer to the Reporting Engine.
    \param[in]  nonce               The suit-nonce for replay protection.

    Before calling this function, at least suit_report_init_engine() must be called.
 */
suit_err_t suit_report_start_encoding(
    struct suit_report_context *report_context,
    UsefulBufC nonce);

// Following structs and functions are expected to be used by the SUIT Manifest Processor

/* draft-ietf-suit-report */

typedef enum suit_report_key {
    SUIT_REPORT_INVALID             = 0,
    SUIT_REPORT_NONCE               = 2,
    SUIT_REPORT_RECORDS             = 3,
    SUIT_REPORT_RESULT              = 4,
    SUIT_REPORT_RESULT_CODE         = 5,
    SUIT_REPORT_RESULT_RECORD       = 6,
    SUIT_REPORT_RESULT_REASON       = 7,
    SUIT_REPORT_CAPABILITY_REPORT   = 8,
    SUIT_REPORT_REFERENCE           = 99,
} suit_report_key_t;

typedef enum suit_report_record_key {
    SUIT_REPORT_MANIFEST_ID         = 0,
    SUIT_REPORT_MANIFEST_SECTION    = 1,
    SUIT_REPORT_SECTION_OFFSET      = 2,
    SUIT_REPORT_COMPONENT_INDEX     = 3,
    SUIT_REPORT_RECORD_PROPERTIES   = 4,
} suit_report_record_key_t;

typedef enum suit_report_reason {
    SUIT_REPORT_REASON_OK                       = 0,
    SUIT_REPORT_REASON_CBOR_PARSE /* FAILURE */ = 1, /*! A parsing error was encountered by the CBOR parser. */
    SUIT_REPORT_REASON_COSE_UNSUPPORTED         = 2, /*! An unsupported COSE ([RFC9052]) structure or header was encountered. */
    SUIT_REPORT_REASON_ALG_UNSUPPORTED          = 3, /*! An unsupported COSE algorithm was encountered. */
    SUIT_REPORT_REASON_UNAUTHORIZED             = 4, /*! Signature/MAC verification failed. */
    SUIT_REPORT_REASON_COMMAND_UNSUPPORTED      = 5, /*! An unsupported command was encountered.*/
    SUIT_REPORT_REASON_COMPONENT_UNSUPPORTED    = 6, /*! The manifest declared a component/prefix that does not exist. */
    SUIT_REPORT_REASON_COMPONENT_UNAUTHORIZED   = 7, /*! The manifest declared a component that is not accessible by the signer. */
    SUIT_REPORT_REASON_PARAMETER_UNSUPPORTED    = 8, /*! The manifest used a parameter that does not exist. */
    SUIT_REPORT_REASON_SEVERING_UNSUPPORTED     = 9, /*! The manifest used severable fields but the Manifest Processor does not support them. */
    SUIT_REPORT_REASON_CONDITION_FAILED         = 10, /*! A condition failed with soft- failure off. */
    SUIT_REPORT_REASON_OPERATION_FAILED         = 11, /*! A command failed (e.g., download/copy/swap/write) */
    SUIT_REPORT_REASON_INVOKE_PENDING           = 12, /*! Invocation is about to be attempted and the final outcome is not yet known. */
} suit_report_reason_t;

/*!
    \brief  Returns SUIT_REPORT_READON_* from libcsuit internal error code.
 */
suit_report_reason_t suit_report_reason_from_suit_err(suit_err_t result);

typedef enum suit_report_capability_report_key {
    SUIT_REPORT_CAPABILITY_INVALID          = 0,
    SUIT_REPORT_CAPABILITY_COMPONENT        = 1,
    SUIT_REPORT_CAPABILITY_COMMAND          = 2,
    SUIT_REPORT_CAPABILITY_PARAMETERS       = 3,
    SUIT_REPORT_CAPABILITY_CRYPT_ALGO       = 4,
    SUIT_REPORT_CAPABILITY_ENVELOPE         = 5,
    SUIT_REPORT_CAPABILITY_MANIFEST         = 6,
    SUIT_REPORT_CAPABILITY_COMMON           = 7,
    SUIT_REPORT_CAPABILITY_TEXT             = 8,
    SUIT_REPORT_CAPABILITY_TEXT_COMPONENT   = 9,
    SUIT_REPORT_CAPABILITY_DEPENDENCY       = 10,
} suit_report_capability_report_key_t;

typedef struct suit_manifest_tree {
    size_t      len;
    uint8_t     manifest_index[SUIT_MAX_ARRAY_LENGTH];
} suit_manifest_tree_t;

enum suit_report_state {
    SUIT_REPORTING_ENGINE_NOT_INITIALIZED = 0,
    SUIT_REPORTING_ENGINE_INITIALIZED,
    SUIT_REPORTING_ENGINE_KEY_LOADED,
    SUIT_REPORTING_ENGINE_STARTED,
    SUIT_REPORTING_ENGINE_SUIT_DIGEST_STORED,
    SUIT_REPORTING_ENGINE_IN_REPORT_RECORD,
    SUIT_REPORTING_ENGINE_IN_SYSTEM_PROPERTY_CLAIMS,
    SUIT_REPORTING_ENGINE_RECORDING_STOPPED,
    SUIT_REPORTING_ENGINE_SUIT_REPORT_IS_ENCODED,
    SUIT_REPORTING_ENGINE_FINALIZED,
};

/*!
    \brief  A context for the SUIT Reporting Engine

    Allocate this object with:

        suit_report_context_t *report_context =
            malloc(sizeof(suit_report_context_t) + SUIT_REPORT_BUFFER_SIZE);

    and free it with

        free(report_context);
        report_context = NULL;

    If the size of the SUIT_Report expected small, you may allocate it on the stack

        uint8_t tmp[sizeof(suit_report_context_t) + SUIT_REPORT_BUFFER_SIZE];
        suit_report_context_t report_context = (suit_report_context_t)tmp;

    You don't need to free it.
 */
typedef struct suit_report_context {
    enum suit_report_state state;

    // COSE operations are conducted if the protection_mechanism.key.cose_algorithm_id is not 0.
    // NOTE: Currently only CBOR_TAG_COSE_SIGN1 is supported.
    // suit_mechanism_t    protection_mechanism;

    QCBOREncodeContext  cbor_encoder;

    // only COSE_Sign1 (18) is supported, and 0 means the raw SUIT_Report
    int cose_protection_mechanism;
    int cose_algorithm_id;
    union {
        struct {
            struct t_cose_sign_sign_ctx         sign_ctx;
            struct t_cose_signature_sign_main   signer;
        };
    };

    // temporal storage for the SUIT_Digest in the suit-authentication-wrapper.
    // NOTE: the actual memory should not be freed before calling suit_report_reference_uri().
    suit_digest_t digest;

    uint8_t current_index;

    // pointer to the encoded (signed or encrypted) SUIT_Report
    UsefulBufC suit_report;

    // Allocated buffer and CBOR encoding context for SUIT_Report.
    //
    // `buf` should be initialized by caller with enough memory.
    // If buf is not set, the SUIT Manifest Processor will not produce SUIT_Report.
    uint8_t buf[];
} suit_report_context_t;

/*!
    \brief  This function should be called after SUIT_Digest authentication.
 */
suit_err_t suit_report_manifest_digest(
    suit_report_context_t *report_context,
    suit_digest_t digest);

/*!
    \brief  This function should be called on suit-reference-uri.
 */
suit_err_t suit_report_manifest_reference_uri(
    suit_report_context_t *report_context,
    UsefulBufC reference_uri);

/*!
    \brief      Stops encoding the suit-report-records

    \param[in]  report_context  Pointer to the context of Repoting Engine

    \return     This returns \ref SUIT_SUCCESS or \ref SUIT_ERR_WHILE_REPORTING.

    If the status of the Reporting Engine is not in
    [ \ref IN_REPORT_RECORD`, \ref IN_SYSTEM_PROPERTY_CLAIMS, \ref RECORDING_STOPPED ],
    returns \ref SUIT_ERR_WHILE_REPORTING.
    The state will be \ref SUIT_REPORTING_ENGINE_RECORDING_STOPPED.
 */
suit_err_t suit_report_stop_records(suit_report_context_t *report_context);

/*!
    \brief      Finalizes the SUIT_Report encoder

    \param[in]  report_context  Pointer to the context of Repoting Engine
    \param[in]  final_state     \ref SUIT_SUCCESS or other \ref suit_err_t which represents the final error cause
    \param[in]  reason          One of \ref suit_report_reason_t to be reported
    \param[in]  dependency_tree The Component Index list indicating the which manifest is being processed in dependency
    \param[in]  manifest_key    Command Sequence id, e.g. suit-validate = 7, suit-invoke = 9, etc.
    \param[in]  section_offset  The location where the current command in the Command Sequence
    \param[in]  component_index The Component Index in processing

    \return     This returns \ref SUIT_SUCCESS or \ref SUIT_ERR_WHILE_REPORTING.

    1. stop encoding suit-report-records
    2. encode suit-report-result
        a. true on result == SUIT_SUCCESS
        b. {result-code: suit_err_t, result-record: SUIT_Record, result-reason: SUIT_Rport_Reasons} otherwise
    3. stop encoding SUIT_Report
    4. sign, calculate MAC (not supported yet) or encrypt (not supported yet)
 */
suit_err_t suit_report_finalize(
    suit_report_context_t *report_context,
    const suit_err_t final_state,
    suit_report_reason_t reason,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value);

/*!
    \brief      Appends SUIT_Record to suit-report-records list

    \param[in]  report_context          Pointer to the context of Repoting Engine
    \param[in]  dependency_tree         The Component Index list indicating the which manifest is being processed in dependency
    \param[in]  manifest_key            Command Sequence id, e.g. suit-validate = 7, suit-invoke = 9, etc.
    \param[in]  section_offset          The location where the condition or the directive in the Command Sequence
    \param[in]  parameter_key   With parameter_value, this represents $$SUIT_Parameter
    \param[in]  parameter_value Pointer to the reported parameter value

    \return     This returns \ref SUIT_SUCCESS or \ref SUIT_ERR_WHILE_REPORTING.
 */
suit_err_t suit_report_extend_record(
    suit_report_context_t *report_context,
    const suit_manifest_tree_t dependency_tree,
    const suit_manifest_key_t manifest_key,
    const size_t section_offset,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value);

/*!
    \brief      Appends $$SUIT_Parameter to system-property-claims

    \param[in]  report_context  Pointer to the context of Repoting Engine
    \param[in]  component_index The Component Index in processing
    \param[in]  component       Pointer to the \ref suit_component_identifier_t represents SUIT_Component_Identifier
    \param[in]  parameter_key   With parameter_value, this represents $$SUIT_Parameter
    \param[in]  parameter_value Pointer to the reported parameter value

    \return     This returns \ref SUIT_SUCCESS or \ref SUIT_ERR_WHILE_REPORTING.
 */
suit_err_t suit_report_extend_system_property_claims(
    suit_report_context_t *report_context,
    const uint8_t component_index,
    const suit_component_identifier_t *component,
    const suit_parameter_key_t parameter_keys[],
    const struct suit_union_parameter *parameter_value);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_REPORTING_ENGINE_H */