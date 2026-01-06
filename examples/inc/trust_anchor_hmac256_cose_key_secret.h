/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef TRUST_ANCHOR_HMAC256_COSE_KEY_SECRET_H
#define TRUST_ANCHOR_HMAC256_COSE_KEY_SECRET_H
#include "qcbor/UsefulBuf.h"

const unsigned char trust_anchor_hmac256_cose_key_secret_buf[] = {
    0xA4,                                   //# map(4)
       0x01,                                //# unsigned(1) / 1 = key /
       0x04,                                //# unsigned(4) / 4 = Symmetric /
       0x02,                                //# unsigned(2) / 2 = kid /
       0x58, 0x20,                          //# bytes(32)
          0x16, 0x60, 0x96, 0xC9, 0x21, 0x14, 0x91, 0x5B,
          0xE3, 0xC2, 0xFA, 0x50, 0x47, 0x9C, 0x43, 0x00,
          0x1E, 0x31, 0xA6, 0x75, 0xD0, 0x73, 0x22, 0x7F,
          0x44, 0xA2, 0x81, 0xD6, 0x0F, 0xF8, 0xFD, 0x79,
       0x03,                                //# unsigned(3) / 3 = alg /
       0x05,                                //# unsigned(5) / 5 = HMAC with SHA-256 /
       0x20,                                //# negative(0) / -1 = k /
       0x58, 0x20,                          //# bytes(32)
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
}; // "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

const UsefulBufC trust_anchor_hmac256_cose_key_secret = {
    .ptr = trust_anchor_hmac256_cose_key_secret_buf,
    .len = sizeof(trust_anchor_hmac256_cose_key_secret_buf)
};
#endif /* TRUST_ANCHOR_HMAC256_COSE_KEY_SECRET_H */
