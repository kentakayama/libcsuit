/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process.h

    \brief  Declarations of structures and functions for SUIT Manifest Processor
 */

#ifndef SUIT_MANIFEST_PROCESS_H
#define SUIT_MANIFEST_PROCESS_H

#include "qcbor/qcbor_spiffy_decode.h"

#include "csuit/suit_common.h"
#include "csuit/suit_digest.h"
#include "csuit/suit_cose.h"
#include "csuit/suit_reporting_engine.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBCSUIT_MAX_REPORT_PRAMETER_NUM 2

typedef struct suit_parameter_args {
    uint64_t                    exists;

    UsefulBufC                  vendor_id;
    UsefulBufC                  class_id;
    UsefulBufC                  device_id;

    UsefulBufC                  image_digest_buf;
    uint64_t                    component_slot;

    /*! default True */
    suit_parameter_bool_t       strict_order;

    /*!
     * default True if suit-directive-try-each is invoked,
     * default False if suit-directive-run-sequence is invoked
     */
    suit_parameter_bool_t       soft_failure;

    uint64_t                    image_size;

    UsefulBufC                  content;
    UsefulBufC                  uri;

    uint64_t                    source_component;

    /*! used in suit-directive-fetch */
    UsefulBufC                  fetch_args;

    /*! used in suit-directive-invoke */
    UsefulBufC                  invoke_args;


    /* in draft-ietf-suit-update-management */
    /*! used in suit-condition-use-before */
    uint64_t                    use_before;
    /*! used in suit-condition-minimum-battery */
    uint64_t                    minimum_battery;
    /*! XXX: used in suit-condition-update-authorized */
    int64_t                     update_priority;
    /*! used in suit-condition-version */
    UsefulBufC                  version_match_buf;

    /*! used in suit-directive-wait */
    UsefulBufC                  wait_info_buf;

    /*! used in suit-directive-fetch, suit-directive-write, ... */
    UsefulBufC                  component_metadata_buf;

    /* in draft-ietf-suit-trust-domains */


    /* in draft-ietf-suit-firmware-encryption */
    /*! Pointer and length to the COSE_Encrypt */
    UsefulBufC                  encryption_info;
} suit_parameter_args_t;

typedef struct suit_union_parameter {
    union {
        int64_t                     i64;
        struct {
            uint64_t                u64; // image-size may used with image-digest
            UsefulBufC              str; // image-digest may used with image-size
        };
        /*! default True */
        suit_parameter_bool_t       b;
    };
} suit_union_parameter_t;

typedef union {
    uint16_t all;
    struct {
        /*!
         * \brief 1: Skip if the requested section is missing.
         *
         * 0: libcsuit returns \ref SUIT_ERR_MANIFEST_KEY_NOT_FOUND.
         * NOTE: must be 0 inside process-dependency
         * see https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-02#name-suit-directive-process-depe
         */
        uint16_t allow_missing          : 1;

        /*!
         * 1: Request libcsuit to trigger suit_store_callback()
         * if suit-manifest-component-id is specified.
         */
        uint16_t manifest_component_id  : 1;
        /*! 1: Request libcsuit to process suit-set-version section. */
        uint16_t set_version            : 1;
        /*! 1: Request libcsuit to process suit-dependency-resolution section. */
        uint16_t dependency_resolution  : 1;
        /*! 1: Request libcsuit to process suit-payload-fetch section. */
        uint16_t payload_fetch          : 1;
        /*! 1: Request libcsuit to process suit-candidate-verification section. */
        uint16_t candidate_verification : 1;
        /*! 1: Request libcsuit to process suit-install section. */
        uint16_t install                : 1;
        /*! 1: Request libcsuit to process suit-uninstall section. */
        uint16_t uninstall              : 1;

        /*! 1: Request libcsuit to process suit-validate section. */
        uint16_t validate               : 1;
        /*! 1: Request libcsuit to process suit-load section. */
        uint16_t load                   : 1;
        /*! 1: Request libcsuit to process suit-invoke section. */
        uint16_t invoke                 : 1;

        /*!
         * \brief 1: Request libcsuit to process suit-text section.
         *
         * This parameter is ignored now.
         */
        uint16_t text                   : 1;
        /*!
         * \brief 1: Request libcsuit to process suit-coswid section.
         *
         * This parameter is ignored now.
         */
        uint16_t coswid                 : 1;
    };
} suit_process_flag_t;

/*!
 * This passes enough data to construct SUIT_Report.
 */
typedef struct suit_report_args {
    /* encoded (protected) SUIT_Report */
    UsefulBufC suit_report;
} suit_report_args_t;

/*!
    \brief  Parameters to request storing data as component identifier.

    Used on suit-directive-write, suit-directive-copy, suit-directive-swap,
    suit-directive-unlink and suit-directive-fetch (only for integrated payloads).
 */
typedef struct suit_invoke_args {
    suit_component_identifier_t component_identifier;
    /* basically byte-string value, so may not '\0' terminated */
    uint8_t args[SUIT_MAX_ARGS_LENGTH];
    size_t args_len;

    suit_rep_policy_t report_policy;
} suit_invoke_args_t;

typedef enum suit_store_key {
    SUIT_STORE  = 0,
    SUIT_COPY   = 1,
    SUIT_SWAP   = 2,
    SUIT_UNLINK = 3,
} suit_store_key_t;

/*!
   \brief   Parameters to request storing data as component identifier.

   Used on suit-directive-write, suit-directive-copy, suit-directive-swap,
   suit-directive-unlink and suit-directive-fetch (only for integrated payloads).
 */
typedef struct suit_store_args {
    suit_rep_policy_t report_policy;

    /*! Destination SUIT_Component_Identifier */
    suit_component_identifier_t dst;
    /*! Used if \ref operation is SUIT_COPY or SUIT_SWAP */
    suit_component_identifier_t src;
    /*! Pointer and length to the content to be written */
    UsefulBufC src_buf;

    /*! Pointer and length to the COSE_Encrypt */
    UsefulBufC encryption_info;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];

    /*! Extra arguments derived from fetch-args */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
    UsefulBufC fetch_args;
#endif

    /*! Metadata for storing data */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
    UsefulBufC component_metadata_buf;
#endif

    /*! SUIT_STORE, SUIT_COPY, SUIT_SWAP, or SUIT_UNLINK */
    suit_store_key_t operation;
} suit_store_args_t;

/*!
 * \brief   Parameters to request fetching and storing data as component identifier.
 *
 * Used on suit-directive-fetch.
 */
typedef struct suit_fetch_args {
    suit_rep_policy_t report_policy;

    /*! Destination SUIT_Component_Identifier */
    suit_component_identifier_t dst;
    /*! Length of uri */
    size_t uri_len;
    /*! URI terminated with '\0' */
    char uri[SUIT_MAX_URI_LENGTH + 1];

    /*!
     *  Pointer to allocated memory in the caller.
     *  This could be NULL if the caller wants callee
     *  to allocate space corresponding to the component identifier.
     */
    void *ptr;
    /*!
     *  The length of the allocated buffer.
     */
    size_t buf_len;

    /*!
     *  Set by suit-parameter-fetch-args.
     */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS)
    UsefulBufC args;
#endif

    /*!
     *  Set by suit-parameter-component-metadata.
     */
#if !defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_METADATA)
    UsefulBufC component_metadata_buf;
#endif

    /* in draft-ietf-suit-firmware-encryption */
    /*! Pointer and length to the COSE_Encrypt */
    UsefulBufC                  encryption_info;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];
} suit_fetch_args_t;

/*!
    \brief  Return value from fetch/store/condition callback.
 */
typedef struct suit_callback_ret {
    /**
     *  The length of the fetched payload.
     */
    size_t buf_len;

    suit_report_reason_t reason;
    // currently, up to two parameters are consumed by one callback    
    suit_parameter_key_t consumed_parameter_keys[LIBCSUIT_MAX_REPORT_PRAMETER_NUM];
    bool on_src;
} suit_callback_ret_t;

/*!
 *  \brief  Parameters to request checking condition.
 */
typedef struct suit_condition_args {
    suit_rep_policy_t report_policy;

    /*! Destination SUIT_Component_Identifier */
    suit_component_identifier_t dst;

    /*! suit-condition-* label */
    suit_con_dir_key_t condition;

    /*! To be expected values */
    suit_union_parameter_t expected;
} suit_condition_args_t;

/*!
    \brief  A context for the SUIT Manifest Processor
    
    Allocate this object with:
    
        suit_processor_context_t *processor_context =
            malloc(sizeof(suit_processor_context_t) + SUIT_PROCESSOR_BUFFER_SIZE);
    
    and free it with
    
        free(processor_context);
        processor_context = NULL;
    
    If the size for the SUIT Manifest, to be fetched payload, etc. are expected small, you may allocate it on the stack:
    
        uint8_t tmp[sizeof(suit_processor_context_t) + SUIT_REPORT_BUFFER_SIZE];
        suit_processor_context_t    processor_context = (suit_processor_context_t    )tmp;
    
    You don't need to free it.
 */
typedef struct suit_processor_context {
    union {
        uint8_t status;
        struct {
            uint8_t initialized: 1;
            uint8_t manifest_loaded: 1;
            uint8_t recipient_key_loaded: 1;
            uint8_t processing: 1;
            uint8_t reported: 1;
            uint8_t done: 1;
            uint8_t padding: 1;
            uint8_t report_invoke_pending: 1;
        };
    } u;

    /* a pointer to the SUIT Reporting Engine, can be NULL */
    suit_report_context_t *reporting_engine;

    /* sections requested to process */
    suit_process_flag_t process_flags;

    UsefulBufC manifest;
    suit_digest_t expected_manifest_digest;
    suit_parameter_args_t parameters[SUIT_MAX_INDEX_NUM];

    // size_t key_len;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];

    // to be reported
    suit_err_t final_state;
    suit_report_reason_t reason;
    suit_manifest_tree_t dependency_tree;
    suit_manifest_key_t manifest_key;
    size_t section_offset;
    suit_con_dir_key_t condition_or_directive;
    uint8_t component_index;
    const suit_component_identifier_t *component;
    // currently, up to two parameters are consumed by one callback
    suit_parameter_key_t parameter_keys[LIBCSUIT_MAX_REPORT_PRAMETER_NUM];
    suit_union_parameter_t parameter_value;

    size_t left_len;
    UsefulBuf allocated;
    uint8_t buf[];
} suit_processor_context_t;

typedef struct suit_extracted {
#if !defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
    suit_dependencies_t dependencies;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID)
    suit_component_identifier_t manifest_component_id;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_SET_VERSION)
    suit_int64_array_t set_version;
#endif
    uint8_t components_len;
    suit_component_with_index_t components[SUIT_MAX_INDEX_NUM];
    suit_payloads_t payloads;

    UsefulBufC manifest;
    suit_digest_t manifest_digest;
    UsefulBufC shared_sequence;
#if !defined(LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI)
    UsefulBufC reference_uri;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
    UsefulBufC dependency_resolution;
    suit_digest_t dependency_resolution_digest;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
    UsefulBufC payload_fetch;
    suit_digest_t payload_fetch_digest;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_CANDIDATE_VERIFICATION)
    UsefulBufC candidate_verification;
    suit_digest_t candidate_verification_digest;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
    UsefulBufC install;
    suit_digest_t install_digest;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
    UsefulBufC uninstall;
#endif
    UsefulBufC validate;
#if !defined(LIBCSUIT_DISABLE_MANIFEST_LOAD)
    UsefulBufC load;
#endif
#if !defined(LIBCSUIT_DISABLE_MANIFEST_INVOKE)
    UsefulBufC invoke;
#endif
} suit_extracted_t;

suit_err_t suit_process_digest(QCBORDecodeContext *context,
                               suit_digest_t *digest,
                               UsefulBufC *digest_buf);

/*!
    \brief  Decode & Process SUIT binary

    \param[in]      suit_inputs     To be processed manifests and its public keys to verify.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    Process one or more SUIT_Envelope(s) like below.
    Libcsuit parse suit-install, suit-invoke, ... and call some callback functions respectively.
    If any error occurred, report callback function will be called.
    By default, libcsuit just calls suit_print_store, suit_print_fetch, etc.
    You can overwrite it with linker options (see Makefile.process for example).

    The figure below describes the flow in the process in pseudocode.
    Note that the arguments and function names are not the same as the actual.

    \code{.unparsed}
    +-App-------------------------------+    +-libcsuit--------------------------------------+
    | main() {                          |    |                                               |
    |   report_key = init_signing_key();|    |                                               |
    |   suit_report_engine_init();      |===>| suit_report_engine_init() {}                  |
    |                                   |    |                                               |
    |   recipient_keys = init_keys();   |    |                                               |
    |   m = get_manifest();             |    |                                               |
    |   suit_processor_init();          |===>| suit_processor_init() {}                      |
    |   suit_process_envelope(m, keys); |===>| suit_process_envelope() {                     |
    |   }                               |    |   check_digests(keys, m);                     |
    | }                                 |    |   install(m) {                                |
    |                                   |    |     p = extract_params(m.shared, m.install);  |
    | suit_fetch_callback() {           |<===|     err = suit_fetch_callback(p.dst, p.uri);  |
    |   http_get(uri, ptr);             |    |                                               |
    |   return SUIT_SUCCESS;            |===>|                                               |
    | }                                 |    |     if (report_condition(p.report, err)) {    |
    | suit_report_callback() {          |<===|       suit_report_callback(err, at);          |
    |   report = create_suit_report();  |    |     }                                         |
    |   http_post(report, uri);         |    |                                               |
    |   return SUIT_SUCCESS;            |===>|     if (err) {                                |
    | }                                 |    |       return SUIT_ERR_ABORT;                  |
    |                                   |    |     }                                         |
    | suit_condition_callback() {       |<===|     err = suit_condition_callback(p.class_id) |
    |   class_id = get_class_id();      |    |                                               |
    |   if (class_id != p.class_id) {   |    |                                               |
    |     suit_report_callback();       |    |                                               |
    |     return SUIT_ERR_CONDITION;    |    |                                               |
    |   }                               |    |                                               |
    |   return SUIT_SUCCESS;            |===>|  }                                            |
    | }                                 |    |  invoke(m) {                                  |
    |                                   |    |    p = extract_params(m.shared, m.invoke);    |
    | suit_invoke_callback() {          |<===|    err = suit_invoke_callback(p.dst, p.args); |
    |   return system(args);            |===>|    if (report_condition(p.report, err)) {     |
    | }                                 |    |      suit_report_callback(err, at);           |
    |                                   |    |    }                                          |
    |                                   |    |  }                                            |
    +-----------------------------------+    +-----------------------------------------------+
    \endcode
 */
suit_err_t suit_process_envelope(
    suit_processor_context_t *processor_context);


/*!
    \brief      Initializes the SUIT Manifest Processor

    \param[in]  processr_context        Pointer to the SUIT Manifest Processor itself.
    \param[in]  buf_size                Allocated buffer size for the SUIT Manifest Processor while processing.
    \param[in]  report_context          Pointer to the SUIT Reporting Engine. No SUIT Report will be produced on NULL.
    \param[in]  report_invoke_pending   Whether finalize and report just before the suit-directive-fetch.
    \param[out] manifest                The buffer to be used for the SUIT Manifest.

    Call this function first to process a SUIT Manifest.
    The size of processor_context must be sizeof(suit_processor_context_t) + buf_size.
    Refer \ref suit_processor_context_t .
 */
suit_err_t suit_processor_init(
    suit_processor_context_t *processor_context,
    size_t buf_size,
    suit_report_context_t *report_context,
    bool report_invoke_pending,
    UsefulBuf *manifest);

/*!
    \brief      Assigns the SUIT Manifest to the SUIT Manifest Processor

    \param[in]  processr_context    Pointer to the SUIT Manifest Processor.
    \param[in]  manifest            The buffer initialized with the SUIT Manifest.
    \param[in]  process_flags       Indicates which section should be processed, e.g. VALIDATE & INSTALL & INVOKE.

    The manifest buffer must start from the one returned by suit_rocessor_init().
 */
suit_err_t suit_processor_add_manifest(
    suit_processor_context_t *processor_context,
    UsefulBufC manifest,
    suit_process_flag_t process_flags);

/*!
    \brief      Assigns a recipient key to the SUIT Manifest Processor

    \param[in]  processr_context    Pointer to the SUIT Manifest Processor.
    \param[in]  cose_tag            The tag, for example, 18 (COSE_Sign1), 17 (COSE_Mac0) or 96 (COSE_Encrypt).
    \param[in]  cose_algorithm_id   The COSE Algorithm identifier, such as -7 (ES256).
    \param[in]  cose_key            COSE_Key used as a sender key.

    The cose_algorithm_id will be ignored if the cose_key has.

 */
suit_err_t suit_processor_add_recipient_key(
    suit_processor_context_t *processor_context,
    int cose_tag,
    int cose_algorithm_id,
    suit_key_t *cose_key);

/*!
    \brief      Terminates the the SUIT Manifest Processor

    \param[in]  processr_context    Pointer to the SUIT Manifest Processor itself.

    You can reuse the same processor_context with suit_processor_init().
    NULL report_context is ignored.
 */
void suit_processor_free(suit_processor_context_t *processor_context);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_MANIFEST_PROCESS_H */