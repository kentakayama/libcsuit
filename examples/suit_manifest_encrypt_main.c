/*
 * Copyright (c) 2020 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include "qcbor/qcbor.h"
#include "t_cose/t_cose_encrypt_enc.h"
#include "t_cose/t_cose_recipient_enc_keywrap.h"
#include "trust_anchor_a128_secret_key.h"
#include "csuit/suit_manifest_print.h"
#include "suit_examples_common.h"

#define MAX_FILE_BUFFER_SIZE            2048

int main(int argc, char *argv[]) {
    // check arguments.
    if (argc < 3) {
        printf("%s <raw image> <encrypted image> <cose file>]", argv[0]);
        return EXIT_FAILURE;
    }
    char *src_file = argv[1];
    char *enc_file = argv[2];
    char *cose_file = argv[3];
    enum t_cose_err_t t_cose_err;
    printf("%s %s %s %s\n", argv[0], src_file, enc_file, cose_file);

    // Load secret key
    suit_mechanism_t mechanism;
    suit_err_t result = suit_key_init_a128kw_secret_key(trust_anchor_a128_secret_key, &mechanism.key);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create A128KW secret key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanism.cose_tag = CBOR_TAG_COSE_ENCRYPT;
    mechanism.use = true;

    // Prepare
    UsefulBuf_MAKE_STACK_UB(raw_buf, MAX_FILE_BUFFER_SIZE);
    raw_buf.len = read_from_file(src_file, raw_buf.ptr, raw_buf.len);

    struct t_cose_recipient_enc_keywrap kw_recipient;
    struct t_cose_encrypt_enc encrypt_context;
    UsefulBuf_MAKE_STACK_UB(encrypted_payload_buf, MAX_FILE_BUFFER_SIZE);
    UsefulBufC encrypted_payload;
    UsefulBuf_MAKE_STACK_UB(cose_message_buf, MAX_FILE_BUFFER_SIZE);
    UsefulBufC cose_message;

    t_cose_recipient_enc_keywrap_init(&kw_recipient, mechanism.key.cose_algorithm_id);
    t_cose_recipient_enc_keywrap_set_key(&kw_recipient, mechanism.key.cose_key, Q_USEFUL_BUF_FROM_SZ_LITERAL("kid-1"));

    t_cose_encrypt_enc_init(&encrypt_context, T_COSE_OPT_MESSAGE_TYPE_ENCRYPT, T_COSE_ALGORITHM_A128GCM);
    t_cose_encrypt_add_recipient(&encrypt_context, (struct t_cose_recipient_enc *)&kw_recipient);

    t_cose_err = t_cose_encrypt_enc_detached(&encrypt_context, UsefulBuf_Const(raw_buf), NULL_Q_USEFUL_BUF_C, encrypted_payload_buf, cose_message_buf, &encrypted_payload, &cose_message);
    if (t_cose_err != T_COSE_SUCCESS) {
        printf("\nmain : Failed to encrypt.\n");
        return EXIT_FAILURE;
    }

    printf("main : Succeed to encrypt.\nmain : Write to %s and %s.\n", enc_file, cose_file);
    write_to_file(enc_file, encrypted_payload.ptr, encrypted_payload.len);
    write_to_file(cose_file, cose_message.ptr, cose_message.len);

    //suit_free_key(&mechanism.key);
    return EXIT_SUCCESS;
}
