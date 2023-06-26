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
