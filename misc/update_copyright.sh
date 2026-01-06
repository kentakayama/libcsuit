#!/bin/bash

#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

set -x
YEAR=$(date +%Y)

# replace
find .. \
    -path "../3rdparty" -prune -o \
    -path "../html" -prune -o \
    -path "../.git" -prune -o \
    -path "../build" -prune -o \
    -path "../tmp" -prune -o \
    -path "../misc/cloc" -prune -o \
    -type f -print | xargs sed -i -E "s/Copyright \(c\) 2020-[0-9]{4}/Copyright (c) 2020-${YEAR}/g"

# check
find .. \
    -path "../3rdparty" -prune -o \
    -path "../html" -prune -o \
    -path "../.git" -prune -o \
    -path "../build" -prune -o \
    -path "../tmp" -prune -o \
    -path "../misc/cloc" -prune -o \
    -type f -print | xargs grep "Copyright" --no-filename | sort | uniq