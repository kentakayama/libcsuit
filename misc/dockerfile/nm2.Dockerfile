# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
# SPDX-License-Identifier: BSD-2-Clause
# +minimize t_cose&mbedtls
FROM libcsuit_min2:latest

WORKDIR /root/libcsuit
COPY misc/analyze_size.py misc/

CMD nm -S bin/suit_manifest_process > min_nm.txt && \
    python3 misc/analyze_size.py
