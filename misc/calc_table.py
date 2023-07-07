#!/usr/bin/env python3
import os
import re
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.pylab as pylab

pat = re.compile(r"\| ([a-zA-Z0-9_ ]+) \| ([a-zA-Z0-9_ +]+) \|")

script_path = os.path.dirname(__file__)
f = open(script_path + "/result_size.txt")

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
df.to_csv(script_path + "/table_size.csv")

plt.figure()
ndf = df.rename(columns = {"+optimize compiler": "+opt compiler", "+minimize t_cose": "+min t_cose", "+minimize mbedtls": "+min mbedtls", "+minimize libcsuit": "+min libcsuit"})

tmp = [ndf[column]["TOTAL"] for column in ndf.columns.values]

ndf = ndf["SUIT Manifest 0":"other"].transpose()
ndf.plot.bar(stacked=True)
plt.xlabel("Size Optimizations", fontsize=12)
plt.xticks(rotation = 0)
plt.ylim([0, 600000])
plt.yticks(plt.yticks()[0], ["{:,}".format(int(i)) for i in plt.yticks()[0]])

for i in range(len(tmp)):
    plt.text(i, tmp[i] + 2000, f"{tmp[i]:,}", ha='center', va='bottom')

plt.savefig(script_path + "/bar_size.png")
plt.close("all")
