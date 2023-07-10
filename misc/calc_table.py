#!/usr/bin/env python3
import sys
import os
import re
import pandas as pd
import matplotlib.pyplot as plt

pat = re.compile(r"\| ([a-zA-Z0-9_ ]+) \| ([a-zA-Z0-9_ +&]+) \|")

args = sys.argv
arch = "unknown" if len(args) <= 1 else args[1]
script_path = os.path.dirname(__file__)
f = open(f"{script_path}/result_size_{arch}.txt")

tmp = {}
column = "UNKNOWN"
for line in f.read().splitlines():
    m = pat.match(line)
    if m:
        column_0, column_1 = m.groups()
        if column_0 == "               ":
            column = column_1
            tmp[column_1] = []
        else:
            tmp[column].append(int(column_1))

df = pd.DataFrame(dtype="int", index=[
    "SUIT Manifest 0",
    "app",
    "libcsuit",
    "t_cose",
    "QCBOR",
    "psa",
    "mbedtls",
    "other",
    "TOTAL"])

for key in tmp.keys():
    df[key] = tmp[key]

print(df)
df.to_csv(f"{script_path}/table_size_{arch}.csv")

ndf = df.rename(columns = {"+optimize compiler": "+opt compiler", "+minimize t_cose": "+min t_cose", "+minimize mbedtls": "+min mbedtls", "+minimize t_cose&mbedtls": "+min t_cose&mbedtls", "+minimize libcsuit": "+min libcsuit"})

tmp = [ndf[column]["TOTAL"] for column in ndf.columns.values]
max_total = max(tmp)

plt.figure()
ndf = ndf["SUIT Manifest 0":"other"].transpose()
ndf.plot.bar(stacked=True)
plt.xlabel(f"Size Optimizations ({arch})", fontsize=12)
plt.xticks(rotation = 0)
plt.ylim([0, (max_total // 100000 + 1) * 100000])
plt.yticks(plt.yticks()[0], ["{:,}".format(int(i)) for i in plt.yticks()[0]])

for i in range(len(tmp)):
    plt.text(i, tmp[i] + 2000, f"{tmp[i]:,}", ha='center', va='bottom')

plt.gcf().set_size_inches(7.2, 4.8)

plt.savefig(f"{script_path}/bar_size_{arch}.png")
plt.close("all")
