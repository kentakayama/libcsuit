#!/usr/bin/env python3
#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

import re

pat = re.compile(r"[0-9a-f]+ ([0-9a-f]+) [a-zA-Z] ([a-zA-Z0-9_.]+)")

result = {
    "manifest": [0, []],
    "app": [0, []],
    "libcsuit": [0, []],
    "t_cose": [0, []],
    "QCBOR": [0, []],
    "psa": [0, []],
    "mbedtls": [0, []],
    "other": [0, []]
}

f = open("min_nm.txt")
for line in f.read().splitlines():
    m = pat.match(line)
    if m:
        size_str, name = m.groups()
        size = int(size_str, 16)

        if name.startswith("manifest_buf"):
            result["manifest"][0] += size
            result["manifest"][1].append(name)
        elif name.startswith(("trust_anchor_", "main", "__wrap", )):
            result["app"][0] += size
            result["app"][1].append(name)
        elif name.startswith((
            "LIBCSUIT_", "suit_", "is_suit_directive_only",
            )):
            result["libcsuit"][0] += size
            result["libcsuit"][1].append(name)
        elif name.startswith((
            "t_cose", "parse_cose_header_parameters", "header_parameter",
            "hash_bstr", "hash_alg", "create_tbs_hash", "short_circuit_",
            "signature_algorithm_id_is_supported", "supported_algs",
            "defined_short_",
            )):
            result["t_cose"][0] += size
            result["t_cose"][1].append(name)
        elif name.startswith((
            "QCBOR", "Useful", "Inetrnal", "IEEE", "Decode", "Con", "Check",
            "RewindMapOrArray", "SearchAndError", "StringTagMap", "TagSpec",
            "UnMapTagNumber", "SearchAndEnter", "MapSearch", "ExitBounded",
            "InternalEnter", "AppendCBORHead", "DoubleConvertAll",
            "Exponent", "Mantissa", "MemPool_", "Process", "UInt64", "Int64",
            "Insert", "Increment",
            )):
            result["QCBOR"][0] += size
            result["QCBOR"][1].append(name)
        elif name.startswith(("psa_", "mbedtls_psa")):
            result["psa"][0] += size
            result["psa"][1].append(name)
        elif name.startswith((
            "mbedtls", "secp", "ec", "entropy", "hmac_drbg_reseed_core",
            "NIST_KW_ICV", "SIGMA_CHARS",
            "LHs", "FS", "FT", "PRK", "RCON", "RHs", "RSb" "RT", "SB",
            "aes_", "add_", "aria_", "asn1_", "base64", "block_cipher_df",
            "brainpool", "camellia_", "ccm", "ccs_", "chacha20_", "cmac_",
            "ct_test", "ctr_drbg", "curve", "des3_", "des_", "gcm_",
            "key_", "iv_", "kw_", "kwp_", "msg_len_test_data", "msg_test_data",
            "oid_", "password_test_data", "pem_", "pers_", "pk_", "plen_test_data",
            "poly1305", "pt_", "res_test_data", "result_", "ripemd160_",
            "rsa_", "salt_test_data", "self_test_rng", "slen_test_data",
            "sha1_", "sha256_", "sha512_", "tag_test_data", "weak_key_table",
            "supported_init", "supported_digests", "md5_test_", "x25519_bad_point_",
            "tag_len_test_data", "ad_test_data", "additional_test_data", "test_",
            "xts_aes", "get_zeros", "get_pkcs", "get_one_and_zeros", "get_no_padding",
            "hmac_drbg_", "chachapoly_", "it_cnt_test_data", "odd_parity_table",
            )):
            result["mbedtls"][0] += size
            result["mbedtls"][1].append(name)
        else:
            result["other"][0] += size
            result["other"][1].append(name)

import os
script_path = os.path.dirname(__file__)
binary_path = script_path + "/../bin/suit_manifest_process"
os.system(f"strip {binary_path}")
process_binary_size = os.path.getsize(binary_path)

other_size = process_binary_size
other_size -= result["manifest"][0]
other_size -= result["app"][0]
other_size -= result["libcsuit"][0]
other_size -= result["t_cose"][0]
other_size -= result["QCBOR"][0]
other_size -= result["psa"][0]
other_size -= result["mbedtls"][0]

print( f"|-----------------|--------:|\n" \
       f"| Example 0 | {result['manifest'][0]} |\n" \
       f"| app | {result['app'][0]} |\n" \
       f"| libcsuit | {result['libcsuit'][0]} |\n" \
       f"| t_cose | {result['t_cose'][0]} |\n" \
       f"| QCBOR | {result['QCBOR'][0]} |\n" \
       f"| psa | {result['psa'][0]} |\n" \
       f"| mbedtls | {result['mbedtls'][0]} |\n" \
       f"| other | {other_size} |\n" \
       f"| TOTAL | {process_binary_size} |")

#print(result["manifest"])
#print(result["app"])
#print(result["libcsuit"])
#print(result["t_cose"])
#print(result["QCBOR"])
#print(result["psa"])
#print(result["mbedtls"])
#print(result["other"])
