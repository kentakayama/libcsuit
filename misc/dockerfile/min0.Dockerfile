# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
# SPDX-License-Identifier: BSD-2-Clause
# libcsuit minimize
FROM libcsuit_min1:latest

CMD nm -S bin/suit_manifest_process > min_nm.txt && \
    python3 misc/analyze_size.py
