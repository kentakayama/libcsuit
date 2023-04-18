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
 *  Public callback function for suit-condition-*. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_condition_callback(suit_condition_args_t condition_args)
{
    return suit_print_condition(condition_args);
}

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
 *  Public callback function to create SUIT Report. See csuit/suit_manifest_callbacks.h
 */
suit_err_t suit_report_callback(suit_report_args_t report_args)
{
    return suit_print_report(report_args);
}


