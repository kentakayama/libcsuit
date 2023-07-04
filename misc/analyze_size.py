#!/usr/bin/env python3

import re

pat = re.compile(r"[0-9a-f]+ ([0-9a-f]+) [a-zA-Z] ([a-zA-Z0-9_.]+)")

result = {
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
        print(size, name)

        if name.startswith(("trust_anchor_", "main", "__wrap", "manifest_buf")):
            result["app"][0] += size
            result["app"][1].append(name)
        elif name.startswith(("LIBCSUIT_", "suit_")):
            result["libcsuit"][0] += size
            result["libcsuit"][1].append(name)
        elif name.startswith(("t_cose", "parse_cose_header_parameters", "header_parameter", "hash_bstr", "hash_alg", "create_tbs_hash")):
            result["t_cose"][0] += size
            result["t_cose"][1].append(name)
        elif name.startswith(("QCBOR", "Useful", "Inetrnal", "IEEE", "Decode", "Con", "Check", "RewindMapOrArray", "SearchAndError", "StringTagMap", "TagSpec", "UnMapTagNumber", "SearchAndEnter", "MapSearch", "ExitBounded", "InternalEnter")):
            result["QCBOR"][0] += size
            result["QCBOR"][1].append(name)
        elif name.startswith(("psa_", "mbedtls_psa")):
            result["psa"][0] += size
            result["psa"][1].append(name)
        elif name.startswith(("mbedtls", "secp", "ec", "entropy", "hmac_drbg_reseed_core")):
            result["mbedtls"][0] += size
            result["mbedtls"][1].append(name)
        else:
            result["other"][0] += size
            result["other"][1].append(name)

print(result["app"])
print(result["libcsuit"])
print(result["t_cose"])
print(result["QCBOR"])
print(result["psa"])
print(result["mbedtls"])
print(result["other"])

import os
script_path = os.path.dirname(__file__)
process_binary_size = os.path.getsize(script_path + "/../bin/suit_manifest_process")

other_size = process_binary_size
other_size -= result["app"][0]
other_size -= result["libcsuit"][0]
other_size -= result["t_cose"][0]
other_size -= result["QCBOR"][0]
other_size -= result["psa"][0]
other_size -= result["mbedtls"][0]

print(f"calculated other: {other_size}")
