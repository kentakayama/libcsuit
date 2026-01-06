/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   csuit.h

    \brief  Includes all of libcsuit headers.
 */

#ifndef CSUIT_H
#define CSUIT_H

#include "suit_common.h"
#include "suit_manifest_encode.h"
#include "suit_manifest_decode.h"
#include "suit_manifest_print.h"
#include "suit_manifest_process.h"
#include "suit_manifest_callbacks.h"
#include "suit_reporting_engine.h"

/* these headers depend crypto library such as OpenSSL and MbedTLS */
#include "suit_cose.h" /* RFC 9052 and 9053 COSE */
#include "suit_digest.h" /* RFC 9054 COSE */

#endif /* CSUIT_H */
