/*
 * Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include "csuit/csuit.h"
#include "qcbor/qcbor_spiffy_decode.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../examples/inc/trust_anchor_esp256_cose_key_public.h"

#define BUFFER_SIZE 4096
#define REPORT_SIZE 1024

unsigned char testfiles_suit_manifest_exp0_suit[] = {
    0xd8, 0x6b, 0xa2, 0x02, 0x58, 0x73, 0x82, 0x58, 0x24, 0x82, 0x2f, 0x58,
    0x20, 0x66, 0x58, 0xea, 0x56, 0x02, 0x62, 0x69, 0x6d, 0xd1, 0xf1, 0x3b,
    0x78, 0x22, 0x39, 0xa0, 0x64, 0xda, 0x7c, 0x6c, 0x5c, 0xba, 0xf5, 0x2f,
    0xde, 0xd4, 0x28, 0xa6, 0xfc, 0x83, 0xc7, 0xe5, 0xaf, 0x58, 0x4a, 0xd2,
    0x84, 0x43, 0xa1, 0x01, 0x28, 0xa0, 0xf6, 0x58, 0x40, 0x86, 0x79, 0xa7,
    0xbd, 0xe3, 0x16, 0xa9, 0xc8, 0xa7, 0xdd, 0x70, 0x1f, 0x3f, 0x15, 0x3f,
    0x01, 0x13, 0x68, 0x74, 0x9f, 0xce, 0x6c, 0x12, 0xe4, 0xe2, 0xc9, 0xf7,
    0x04, 0xd3, 0xbd, 0x10, 0x4f, 0xb7, 0x75, 0x59, 0xd2, 0xd9, 0x72, 0x3a,
    0xc9, 0xce, 0x55, 0x0d, 0xc6, 0x9e, 0x7b, 0xf8, 0xcc, 0x3a, 0x1a, 0x46,
    0x0d, 0xc6, 0xf2, 0xb8, 0x51, 0x78, 0x0b, 0xd2, 0x3c, 0xf6, 0x66, 0x9c,
    0x29, 0x03, 0x58, 0x71, 0xa5, 0x01, 0x01, 0x02, 0x00, 0x03, 0x58, 0x5f,
    0xa2, 0x02, 0x81, 0x81, 0x41, 0x00, 0x04, 0x58, 0x56, 0x86, 0x14, 0xa4,
    0x01, 0x50, 0xfa, 0x6b, 0x4a, 0x53, 0xd5, 0xad, 0x5f, 0xdf, 0xbe, 0x9d,
    0xe6, 0x63, 0xe4, 0xd4, 0x1f, 0xfe, 0x02, 0x50, 0x14, 0x92, 0xaf, 0x14,
    0x25, 0x69, 0x5e, 0x48, 0xbf, 0x42, 0x9b, 0x2d, 0x51, 0xf2, 0xab, 0x45,
    0x03, 0x58, 0x24, 0x82, 0x2f, 0x58, 0x20, 0x00, 0x11, 0x22, 0x33, 0x44,
    0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01,
    0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76,
    0x54, 0x32, 0x10, 0x0e, 0x19, 0x87, 0xd0, 0x01, 0x0f, 0x02, 0x0f, 0x07,
    0x43, 0x82, 0x03, 0x0f, 0x09, 0x43, 0x82, 0x17, 0x02
};
unsigned int testfiles_suit_manifest_exp0_suit_len = 237;

size_t g_pos;
enum callback_type {
    SUIT_CALLBACK_INVALID = 0, // sentinel
    SUIT_CALLBACK_STORE,
    SUIT_CALLBACK_FETCH,
    SUIT_CALLBACK_CONDITION,
    SUIT_CALLBACK_INVOKE,
};
union callback_arg {
    struct {
        suit_con_dir_key_t key;
        suit_union_parameter_t condition;
    };
    UsefulBufC invoke_arg;
};
struct expected_callback_func_args {
    enum callback_type type;
    UsefulBufC src;
    UsefulBufC dst;
    UsefulBufC manifest_digest;
    uint64_t manifest_sequence_number;
    union callback_arg arg;
};
#define NUM_CALLBACKS 32
struct expected_callback_func_args g_args[NUM_CALLBACKS];

uint8_t vendor_id[] = {
    0xfa, 0x6b, 0x4a, 0x53, 0xd5, 0xad, 0x5f, 0xdf, 0xbe, 0x9d, 0xe6, 0x63, 0xe4, 0xd4, 0x1f, 0xfe
};
uint8_t class_id[] = {
    0x14, 0x92, 0xaf, 0x14, 0x25, 0x69, 0x5e, 0x48, 0xbf, 0x42, 0x9b, 0x2d, 0x51, 0xf2, 0xab, 0x45, 
};
uint8_t image_digest[] = {
    0x82, // array(2)
    0x2f, // negative(15) = -16, SHA-256
    0x58, 0x20, // bytes(32)
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 
};

suit_err_t __wrap_suit_fetch_callback(suit_fetch_args_t args, suit_callback_ret_t *ret)
{
    CU_FAIL()
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_store_callback(suit_store_args_t args, suit_callback_ret_t *ret)
{
    CU_FAIL();
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_invoke_callback(suit_invoke_args_t invoke_args)
{
    CU_ASSERT_FATAL(g_pos < NUM_CALLBACKS);
    CU_ASSERT_EQUAL_FATAL(SUIT_CALLBACK_INVOKE, g_args[g_pos].type);
    CU_ASSERT_EQUAL(UsefulBuf_Compare(invoke_args.manifest_digest, g_args[g_pos].manifest_digest), 0);

    CU_ASSERT_EQUAL(UsefulBuf_Compare(invoke_args.encoded_component_identifier, g_args[g_pos].dst), 0);
    CU_ASSERT_EQUAL_FATAL(invoke_args.args_len, g_args[g_pos].arg.invoke_arg.len);
    CU_ASSERT_EQUAL(memcmp(invoke_args.args, g_args[g_pos].arg.invoke_arg.ptr, invoke_args.args_len), 0);

    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_condition_callback(suit_condition_args_t condition_args, suit_callback_ret_t *condition_ret)
{
    CU_ASSERT_FATAL(g_pos < NUM_CALLBACKS);
    CU_ASSERT_EQUAL_FATAL(SUIT_CALLBACK_CONDITION, g_args[g_pos].type);
    CU_ASSERT_EQUAL(UsefulBuf_Compare(condition_args.manifest_digest, g_args[g_pos].manifest_digest), 0);

    CU_ASSERT_EQUAL(UsefulBuf_Compare(condition_args.dst, g_args[g_pos].dst), 0);
    CU_ASSERT_EQUAL(condition_args.condition, g_args[g_pos].arg.key);
    switch (condition_args.condition) {
    /* compare bstr or tstr */
    case SUIT_CONDITION_VENDOR_IDENTIFIER:
    case SUIT_CONDITION_CLASS_IDENTIFIER:
    case SUIT_CONDITION_DEVICE_IDENTIFIER:
    case SUIT_CONDITION_CHECK_CONTENT:
    case SUIT_CONDITION_IMAGE_NOT_MATCH:
        CU_ASSERT_EQUAL(UsefulBuf_Compare(condition_args.expected.str, g_args[g_pos].arg.condition.str), 0);
        break;

    /* compare bstr and uint64 */
    case SUIT_CONDITION_IMAGE_MATCH:
    case SUIT_CONDITION_VERSION:        
        CU_ASSERT_EQUAL(UsefulBuf_Compare(condition_args.expected.str, g_args[g_pos].arg.condition.str), 0);
        CU_ASSERT_EQUAL(condition_args.expected.u64, g_args[g_pos].arg.condition.u64);
        break;

    /* compare int64 */
    case SUIT_CONDITION_USE_BEFORE:
        CU_ASSERT_EQUAL(condition_args.expected.i64, g_args[g_pos].arg.condition.i64);
        break;

    /* compare uint64 */
    case SUIT_CONDITION_COMPONENT_SLOT:
    case SUIT_CONDITION_MINIMUM_BATTERY:
    case SUIT_CONDITION_UPDATE_AUTHORIZED:
        CU_ASSERT_EQUAL(condition_args.expected.u64, g_args[g_pos].arg.condition.u64);
        break;

    /* should not be called */
    case SUIT_CONDITION_ABORT:
    case SUIT_CONDITION_IS_DEPENDENCY:
    case SUIT_CONDITION_DEPENDENCY_INTEGRITY:
    default:
        CU_FAIL();
    }

    g_pos++;
    return SUIT_SUCCESS;
}

suit_err_t __wrap_suit_report_callback(suit_report_args_t arg)
{
    // pass
    return SUIT_SUCCESS;
}

void test_process_example0(void)
{
    g_pos = 0;
    uint8_t example0_digest_bytes[] = {
        0x82, // array(2)
        0x2f, // negative(15) = -16, SHA-256
        0x58, 0x20, // bytes(32)
        0x66, 0x58, 0xea, 0x56, 0x02, 0x62, 0x69, 0x6d, 0xd1, 0xf1, 0x3b, 0x78, 0x22, 0x39, 0xa0, 0x64, 0xda, 0x7c, 0x6c, 0x5c, 0xba, 0xf5, 0x2f, 0xde, 0xd4, 0x28, 0xa6, 0xfc, 0x83, 0xc7, 0xe5, 0xaf,
    };
    UsefulBufC example0_digest = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(example0_digest_bytes);
    uint8_t example0_component0_bytes[] = {
        0x81, 0x41, 0x00, // [ h'00' ]
    };
    UsefulBufC example0_component = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(example0_component0_bytes);

    /* set expected sequence of callback functions and their arguments for example 0 */
    memset(g_args, 0, sizeof(g_args));
    // validate, shared-sequence
    g_args[0] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_CONDITION,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.key = SUIT_CONDITION_VENDOR_IDENTIFIER,
        .arg.condition = (suit_union_parameter_t){
            .str = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(vendor_id)
        }
    };
    g_args[1] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_CONDITION,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.key = SUIT_CONDITION_CLASS_IDENTIFIER,
        .arg.condition = (suit_union_parameter_t){
            .str = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(class_id)
        }
    };
    // validate
    g_args[2] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_CONDITION,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.key = SUIT_CONDITION_IMAGE_MATCH,
        .arg.condition = (suit_union_parameter_t){
            .u64 = 34768,
            .str = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(image_digest)
        }
    };
    // invoke, shared-sequence
    g_args[3] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_CONDITION,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.key = SUIT_CONDITION_VENDOR_IDENTIFIER,
        .arg.condition = (suit_union_parameter_t){
            .str = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(vendor_id)
        }
    };
    g_args[4] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_CONDITION,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.key = SUIT_PARAMETER_CLASS_IDENTIFIER,
        .arg.condition = (suit_union_parameter_t){
            .str = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(class_id)
        }
    };
    g_args[5] = (struct expected_callback_func_args){
        .type = SUIT_CALLBACK_INVOKE,
        .manifest_digest = example0_digest,
        .dst = example0_component,
        .arg.invoke_arg = NULLUsefulBufC
    };

    suit_report_context_t *reporting_engine = malloc(sizeof(suit_report_context_t) + REPORT_SIZE);
    CU_ASSERT_PTR_NOT_NULL(reporting_engine);
    suit_processor_context_t *processor_context = malloc(sizeof(suit_processor_context_t) + BUFFER_SIZE);
    CU_ASSERT_PTR_NOT_NULL(reporting_engine);
    
    suit_report_init_engine(reporting_engine, REPORT_SIZE);
    UsefulBufC nonce = NULLUsefulBufC;
    suit_report_start_encoding(reporting_engine, nonce);

    UsefulBuf manifest_buf;
    bool report_invoke_pending = true;
    suit_processor_init(processor_context, BUFFER_SIZE, reporting_engine, report_invoke_pending, &manifest_buf);
    memcpy(manifest_buf.ptr, testfiles_suit_manifest_exp0_suit, testfiles_suit_manifest_exp0_suit_len);
    manifest_buf.len = testfiles_suit_manifest_exp0_suit_len;
    suit_process_flag_t process_flags;
    process_flags.all = UINT16_MAX;
    process_flags.uninstall = 0;

    CU_ASSERT_EQUAL_FATAL(suit_processor_add_manifest(processor_context, UsefulBuf_Const(manifest_buf), process_flags), SUIT_SUCCESS);
    suit_key_t signer_key;
    CU_ASSERT_EQUAL_FATAL(suit_set_suit_key_from_cose_key(trust_anchor_esp256_cose_key_public, &signer_key), SUIT_SUCCESS);
    CU_ASSERT_EQUAL_FATAL(suit_processor_add_recipient_key(processor_context, CBOR_TAG_COSE_SIGN1, T_COSE_ALGORITHM_ESP256, &signer_key), SUIT_SUCCESS);
    CU_ASSERT_EQUAL_FATAL(suit_process_envelope(processor_context), SUIT_SUCCESS);
}
