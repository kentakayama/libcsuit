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

#include "csuit/suit_manifest_process.h"
#include "csuit/suit_manifest_callbacks.h"
#include "csuit/suit_manifest_print.h"

/*
 *  Public callback function for such as suit-directive-write. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_store_callback(suit_store_args_t store_args, suit_callback_ret_t *store_ret)
{
    return suit_print_store(store_args);
}

/*
 *  Public callback function for suit-directive-fetch. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_fetch_callback(suit_fetch_args_t fetch_args,
                               suit_callback_ret_t *fetch_ret)
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
suit_err_t suit_condition_callback(suit_condition_args_t condition_args, suit_callback_ret_t *condition_ret)
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
suit_err_t suit_report_callback(suit_report_args_t report_args)
{
    return suit_print_report(report_args);
}
#endif /* !LIBCSUIT_DISABLE_SUIT_REPORT */
