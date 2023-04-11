/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
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

/* these headers depend crypto library such as OpenSSL and MbedTLS */
#include "suit_cose.h" /* RFC8152 COSE */
#include "suit_digest.h" /* hash */

#endif /* CSUIT_H */
