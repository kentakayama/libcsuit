#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# +minimize libcsuit
FROM libcsuit_min1:latest

WORKDIR /root/libcsuit
COPY misc/analyze_size.py misc/

CMD nm -S bin/suit_manifest_process > min_nm.txt && \
    python3 misc/analyze_size.py
