#!/usr/bin/env python3

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

print(result["manifest"])
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
other_size -= result["manifest"][0]
other_size -= result["app"][0]
other_size -= result["libcsuit"][0]
other_size -= result["t_cose"][0]
other_size -= result["QCBOR"][0]
other_size -= result["psa"][0]
other_size -= result["mbedtls"][0]

print(f"calculated other: {other_size}")
print()
print( f"| TOTAL           | {process_binary_size:7,} |\n" \
       f"|-----------------|--------:|\n" \
       f"| SUIT Manifest 0 | {result['manifest'][0]:7,d} |\n" \
       f"| app             | {result['app'][0]:7,d} |\n" \
       f"| libcsuit        | {result['libcsuit'][0]:7,d} |\n" \
       f"| t_cose          | {result['t_cose'][0]:7,d} |\n" \
       f"| QCBOR           | {result['QCBOR'][0]:7,d} |\n" \
       f"| psa             | {result['psa'][0]:7,d} |\n" \
       f"| mbedtls         | {result['mbedtls'][0]:7,d} |\n" \
       f"| other           | {other_size:7,d} |\n")
