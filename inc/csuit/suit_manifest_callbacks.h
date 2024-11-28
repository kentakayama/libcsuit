/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_callbacks.h

    \brief  Declarations of callbacks trigged while processing
 */

#ifndef SUIT_MANIFEST_CALLBACKS_H
#define SUIT_MANIFEST_CALLBACKS_H

#include "suit_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
    \brief  SUIT fetch callback triggerd in libcsuit

    \param[in]      fetch_args      Fetch and suit-report arguments. See \ref suit_fetch_args_t.
    \param[out]     fetch_ret       Fetch result. See \ref suit_fetch_ret_t.
    Triggered on \ref SUIT_DIRECTIVE_FETCH.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_fetch_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-fetch
    requesting to fetch an image and dependent SUIT Manifest into component.
*/
suit_err_t suit_fetch_callback(suit_fetch_args_t fetch_args, suit_fetch_ret_t *fetch_ret);

/*!
    \brief  SUIT store callback triggerd in libcsuit
    \param[in]      store_args      Store and suit-report arguments. See \ref suit_store_args_t.
    Triggered on \ref SUIT_DIRECTIVE_FETCH of integrated-payload or integrated-dependency.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_store_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-write, suit-directive-copy,
    suit-directive-swap and suit-directive-unlink against component(s),
    and at suit-directive-fetch against integrated payload,
    requesting to store an image and dependent SUIT Manifest into component or
    to unlink a component.
*/
suit_err_t suit_store_callback(suit_store_args_t store_args);

/*!
    \brief  SUIT invoke callback triggerd in libcsuit
    \param[in]      invoke_args        Invoke and suit-report arguments. See \ref suit_invoke_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_invoke_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-invoke
    requesting to invoke a component.
*/
suit_err_t suit_invoke_callback(suit_invoke_args_t invoke_args);

/*!
    \brief  SUIT condition callback triggerd in libcsuit
    \param[in]      condition_args     Condition and suit-report arguments. See \ref suit_condition_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_condition_callback. See Makefile.process as an example.
    The libcsuit may trigger this function at suit-condition-*
    requesting to check the condition.
*/
suit_err_t suit_condition_callback(suit_condition_args_t condition_args);

/*!
    \brief  SUIT set-version callback triggerd in libcsuit
    \param[in]      set_version         Array of int a.k.a. SUIT_Condition_Version_Comparison_Value. See \ref suit_int64_array_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_set_version_callback. See Makefile.process as an example.
    The libcsuit may trigger this function at suit-set-version requesting to check the condition.
*/
suit_err_t suit_set_version_callback(suit_int64_array_t set_version);

/*!
    \brief  SUIT wait callback triggerd in libcsuit
    \param[in]      wait_args     Wait and suit-report arguments. See \ref suit_wait_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_wait_callback. See Makefile.process as an example.
    The libcsuit may trigger this function at suit-directive-wait
    requesting to check the wait.
*/
suit_err_t suit_wait_callback(suit_wait_args_t wait_args);

/*!
    \brief  SUIT report callback trigged in libcsuit
    \param[in]      report_args     Suit-report arguments and errors. See \ref suit_report_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_report_callback.
    The libcsuit may trigger this function based on SUIT_Rep_Policy.
    requesting to check the condition.
*/
suit_err_t suit_report_callback(suit_report_args_t report_args);

#ifdef __cplusplus
}
#endif

#endif /* SUIT_MANIFEST_CALLBACKS_H */
