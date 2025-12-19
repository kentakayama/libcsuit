/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#ifndef SUIT_EXAMPLES_COMMON_H
#define SUIT_EXAMPLES_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csuit/suit_common.h"
#include "csuit/suit_manifest_decode.h"
#include "csuit/suit_manifest_process.h"

ssize_t read_from_file(const char *file_path, uint8_t *buf, const size_t buf_len);
ssize_t write_to_file(const char *file_path, const void *buf, const size_t buf_len);
ssize_t write_to_file_component_metadata(const char *file_path,
                                         const void *buf,
                                         const size_t buf_len,
                                         UsefulBufC component_metadata_buf,
                                         suit_callback_ret_t *ret);

#endif  /* SUIT_EXAMPLES_COMMON_H */
