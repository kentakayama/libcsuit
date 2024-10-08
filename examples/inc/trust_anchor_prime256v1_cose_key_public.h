/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

#ifndef TRUST_ANCHOR_PRIME256V1_COSE_KEY_PUBLIC_H
#define TRUST_ANCHOR_PRIME256V1_COSE_KEY_PUBLIC_H
#include "qcbor/UsefulBuf.h"

const unsigned char trust_anchor_prime256v1_cose_key_public_buf[] = {
    0xA4,                                 //# map(4)
       0x01,                              //# unsigned(1) / 1 = kty /
       0x02,                              //# unsigned(2) / 2 = EC2 /
       0x20,                              //# negative(0) / -1 = crv /
       0x01,                              //# unsigned(1) / 1 = P-256 /
       0x21,                              //# negative(1) / -2 = x /
       0x58, 0x20,                        //# bytes(32)
          0x84, 0x96, 0x81, 0x1A, 0xAE, 0x0B, 0xAA, 0xAB,
          0xD2, 0x61, 0x57, 0x18, 0x9E, 0xEC, 0xDA, 0x26,
          0xBE, 0xAA, 0x8B, 0xF1, 0x1B, 0x6F, 0x3F, 0xE6,
          0xE2, 0xB5, 0x65, 0x9C, 0x85, 0xDB, 0xC0, 0xAD,
       0x22,                              //# negative(2) / -3 = y /
       0x58, 0x20,                        //# bytes(32)
          0x3B, 0x1F, 0x2A, 0x4B, 0x6C, 0x09, 0x81, 0x31,
          0xC0, 0xA3, 0x6D, 0xAC, 0xD1, 0xD7, 0x8B, 0xD3,
          0x81, 0xDC, 0xDF, 0xB0, 0x9C, 0x05, 0x2D, 0xB3,
          0x39, 0x91, 0xDB, 0x73, 0x38, 0xB4, 0xA8, 0x96,
};

const UsefulBufC trust_anchor_prime256v1_cose_key_public = {
    .ptr = trust_anchor_prime256v1_cose_key_public_buf,
    .len = sizeof(trust_anchor_prime256v1_cose_key_public_buf)
};
#endif /* TRUST_ANCHOR_PRIME256V1_COSE_KEY_PUBLIC_H */
