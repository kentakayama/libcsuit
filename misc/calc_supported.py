#!/usr/bin/env python3
import re

# const
supported_str = "^[-A-Za-z ()]+ \| [?0-9]+ \| ([-a-z]+) \| ([A-Za-z ]+) \| [A-Za-z /]+ \| ([Rr-])([Ww-])([Xx-])$"

def calc_suit_manifest():
    # const
    table_str = "Name | Label | CDDL Structure | IN | IS | Supported?"
    documents = [ "SUIT Manifest", "SUIT Multiple Trust Domains", "SUIT Update Management", "SUIT Encrypted Payload", "SUIT MUD" ]

    pos_list = [2, 4, 5, 6, 7]
    # regex pattern
    supported_pat = re.compile(supported_str)

    # outputs
    num_matched = 0
    tab = [ [ [ [0, 0] for i in range(3) ] for j in range(len(pos_list)) ] for k in range(len(documents)) ]
    print(tab)

    # start reading and writing
    f = open("TABLES_MANIFEST_SUPPORTED.md", "r")
    for line in f.read().splitlines():
        if line == table_str:
            num_matched += 1
            print(num_matched)
            continue

        if num_matched in pos_list:
            m = supported_pat.match(line)
            if m:
                print(m.groups())
                name, doc, sr, sw, sx = m.groups()
                tab[documents.index(doc)][pos_list.index(num_matched)][0][0 if sr == "R" else 1] += 1
                tab[documents.index(doc)][pos_list.index(num_matched)][1][0 if sw == "W" else 1] += 1
                tab[documents.index(doc)][pos_list.index(num_matched)][2][0 if sx == "X" else 1] += 1

    f.close()
    print(tab)

    ret_str = ""
    for i in range(len(documents)):
        ret_str += f"### {documents[i]}\n"
        ret_str += "Operation | Metadata | Condition | Directive | Parameter | Text\n"
        ret_str += "---|---|---|---|---|---\n"
        ret_str += "decode"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][0][0]}{':red_square:'*tab[i][level][0][1]}"
        ret_str += "\n"
        ret_str += "encode"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][1][0]}{':red_square:'*tab[i][level][1][1]}"
        ret_str += "\n"
        ret_str += "process"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][2][0]}{':red_square:'*tab[i][level][2][1]}"
        ret_str += "\n"
    return ret_str

def calc_suit_report():
    # const
    table_str = "Name | Label | CDDL Structure | IN | IS | Supported?"
    documents = [ "SUIT Report" ]

    pos_list = [1, 2]
    # regex pattern
    supported_pat = re.compile(supported_str)

    # outputs
    num_matched = 0
    tab = [ [ [ [0, 0] for i in range(3) ] for j in range(len(pos_list)) ] for k in range(len(documents)) ]
    print(tab)

    # start reading and writing
    f = open("TABLES_REPORT_SUPPORTED.md", "r")
    for line in f.read().splitlines():
        if line == table_str:
            num_matched += 1
            print(num_matched)
            continue

        if num_matched in pos_list:
            m = supported_pat.match(line)
            if m:
                print(m.groups())
                name, doc, sr, sw, sx = m.groups()
                tab[documents.index(doc)][pos_list.index(num_matched)][0][0 if sr == "R" else 1] += 1
                tab[documents.index(doc)][pos_list.index(num_matched)][1][0 if sw == "W" else 1] += 1
                tab[documents.index(doc)][pos_list.index(num_matched)][2][0 if sx == "X" else 1] += 1

    f.close()
    print(tab)

    ret_str = ""
    for i in range(len(documents)):
        ret_str += f"### {documents[i]}\n"
        ret_str += "Operation | Report | Capabilities\n"
        ret_str += "---|---|---\n"
        ret_str += "decode"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][0][0]}{':red_square:'*tab[i][level][0][1]}"
        ret_str += "\n"
        ret_str += "encode"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][1][0]}{':red_square:'*tab[i][level][1][1]}"
        ret_str += "\n"
        ret_str += "process"
        for level in range(len(pos_list)):
            ret_str += f" | {':green_square:'*tab[i][level][2][0]}{':red_square:'*tab[i][level][2][1]}"
        ret_str += "\n"
    return ret_str


w = open("../SUPPORTED.md", "w")
head = """# Supported features by libcsuit

## Summary
- :green_square: : supported
- :red_square: : **not** supported
"""
w.write(head)
w.write(calc_suit_manifest())
w.write(calc_suit_report())

middle = """
## NOTE: How to read the Supported Features Tables
**IN**: which document
Name | Reference
---|---
SUIT Manifest | https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest
SUIT Multiple Trust Domains | https://datatracker.ietf.org/doc/html/draft-moran-suit-trust-domains
SUIT Update Manegement | https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management
SUIT Encrypted Payload | https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption
SUIT MUD | https://datatracker.ietf.org/doc/html/draft-ietf-suit-mud
SUIT Report | https://datatracker.ietf.org/doc/html/draft-ietf-suit-report

**IS**:
- It is **REQUIRED** to (or **MUST**) implement
- It is **RECOMMENDED** to (or **SHOULD**) implement
- It is **OPTIONAL** to (or **MAY**) implement
- It is Not Mentioned (**N/M**) to implement
Only SUIT Manifest document defines REQUIRED features for minimal implementation, so that other documents may not mention.

**Supported?**: libcsuit has 3 core features: decode (Read), encode (Write) and process (eXecute).  
SUIT features are listed below for each SUIT documents.
- **R**: supported, and **-**: not supported
- **W**: supported, and **-**: not supported
- **X**: supported, and **-**: not supported


## Supported Features Tables
"""
w.write(middle)

with open("TABLES_MANIFEST_SUPPORTED.md", "r") as f:
    w.write(f.read())

with open("TABLES_REPORT_SUPPORTED.md", "r") as f:
    w.write(f.read())

w.close()

