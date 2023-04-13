/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process.h

    \brief  Declarations of structures and functions
 */

#ifndef SUIT_MANIFEST_PROCESS_H
#define SUIT_MANIFEST_PROCESS_H

#include "suit_common.h"
#include "suit_manifest_decode.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BIT(nr) (1UL << (nr))
/* draft-suit-manifest */
#define SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER BIT(SUIT_PARAMETER_VENDOR_IDENTIFIER)
#define SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER BIT(SUIT_PARAMETER_CLASS_IDENTIFIER)
#define SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST BIT(SUIT_PARAMETER_IMAGE_DIGEST)
#define SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT BIT(SUIT_PARAMETER_COMPONENT_SLOT)
#define SUIT_PARAMETER_CONTAINS_STRICT_ORDER BIT(SUIT_PARAMETER_STRICT_ORDER)
#define SUIT_PARAMETER_CONTAINS_SOFT_FAILURE BIT(SUIT_PARAMETER_SOFT_FAILURE)
#define SUIT_PARAMETER_CONTAINS_IMAGE_SIZE BIT(SUIT_PARAMETER_IMAGE_SIZE)
#define SUIT_PARAMETER_CONTAINS_CONTENT BIT(SUIT_PARAMETER_CONTENT)
#define SUIT_PARAMETER_CONTAINS_URI BIT(SUIT_PARAMETER_URI)
#define SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT BIT(SUIT_PARAMETER_SOURCE_COMPONENT)
#define SUIT_PARAMETER_CONTAINS_INVOKE_ARGS BIT(SUIT_PARAMETER_INVOKE_ARGS)
#define SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER BIT(SUIT_PARAMETER_DEVICE_IDENTIFIER)

/* draft-suit-update-management */
#define SUIT_PARAMETER_CONTAINS_USE_BEFORE BIT(SUIT_PARAMETER_USE_BEFORE)
#define SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY BIT(SUIT_PARAMETER_MINIMUM_BATTERY)
#define SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY BIT(SUIT_PARAMETER_UPDATE_PRIORITY)
#define SUIT_PARAMETER_CONTAINS_VERSION BIT(SUIT_PARAMETER_VERSION)
#define SUIT_PARAMETER_CONTAINS_WAIT_INFO BIT(SUIT_PARAMETER_WAIT_INFO)

/* draft-suit-trust-domains */
#define SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO BIT(SUIT_PARAMETER_ENCRYPTION_INFO)

void suit_process_digest(QCBORDecodeContext *context,
                         suit_digest_t *digest);

/*!
    \brief  Decode & Process SUIT binary

    \param[in]      suit_inputs     To be procceed manifests and its public keys to verify.
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
    |   keys = init_keys();             |    |                                               |
    |   m = get_manifest();             |    |                                               |
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
suit_err_t suit_process_envelope(suit_inputs_t *suit_inputs);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_MANIFEST_PROCESS_H */
