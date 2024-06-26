/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef TAM_PUBLIC_KEY_H
#define TAM_PUBLIC_KEY_H

const unsigned char tam_es256_public_key[] = {
    0x04 /* POINT_CONVERSION_UNCOMPRESSED */,
    0x0e, 0x90, 0x8a, 0xa8, 0xf0, 0x66, 0xdb, 0x1f,
    0x08, 0x4e, 0x0c, 0x36, 0x52, 0xc6, 0x39, 0x52,
    0xbd, 0x99, 0xf2, 0xa5, 0xbd, 0xb2, 0x2f, 0x9e,
    0x01, 0x36, 0x7a, 0xad, 0x03, 0xab, 0xa6, 0x8b,
    0x77, 0xda, 0x1b, 0xd8, 0xac, 0x4f, 0x0c, 0xb4,
    0x90, 0xba, 0x21, 0x06, 0x48, 0xbf, 0x79, 0xab,
    0x16, 0x4d, 0x49, 0xad, 0x35, 0x51, 0xd7, 0x1d,
    0x31, 0x4b, 0x27, 0x49, 0xee, 0x42, 0xd2, 0x9a
};

#endif /* TAM_PUBLIC_KEY_H */
