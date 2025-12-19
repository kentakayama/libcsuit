/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#ifndef TRUST_ANCHOR_A128_COSE_KEY_SECRET_H
#define TRUST_ANCHOR_A128_COSE_KEY_SECRET_H
#include "qcbor/UsefulBuf.h"

const unsigned char trust_anchor_a128_cose_key_secret_buf[] = {
    0xA4,                                   //# map(4)
       0x01,                                //# unsigned(1) / 1 = key /
       0x04,                                //# unsigned(4) / 4 = Symmetric /
       0x02,                                //# unsigned(2) / 2 = kid /
       0x58, 0x20,                          //# bytes(32)
          0x01, 0x4E, 0x1D, 0xDC, 0xBE, 0xE9, 0xB3, 0x87,
          0x75, 0xC9, 0xC9, 0x99, 0x85, 0x93, 0x21, 0x1C,
          0x22, 0x87, 0x68, 0x90, 0xF8, 0xB3, 0x76, 0x60,
          0x4F, 0xB2, 0x02, 0xA2, 0x65, 0x34, 0xEF, 0x8F,
       0x03,                                //# unsigned(3) / 3 = alg /
       0x23,                                //# negative(2) / -3 = A128KW /
       0x20,                                //# negative(0) / -1 = k /
       0x58, 0x10,                          //# bytes(16)
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
          0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
}; // "aaaaaaaaaaaaaaaa"
UsefulBufC trust_anchor_a128_cose_key_secret = {
    .ptr = trust_anchor_a128_cose_key_secret_buf,
    .len = sizeof(trust_anchor_a128_cose_key_secret_buf)
};
#endif /* TRUST_ANCHOR_A128_COSE_KEY_SECRET_H */
