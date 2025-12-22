#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

MBEDTLS_DIR ?= ../../3rdparty/mbedtls
QCBORE_DIR ?= ../../3rdparty/QCBOR
T_COSE_DIR ?= ../../3rdparty/t_cose

%.o: ../../src/%.c
	$(CC) $(CFLAGS) $(INC) $(CMD_LINE) -o $@ -c $<

%.o: ../%.c
	$(CC) $(CFLAGS) $(INC) $(CMD_LINE) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $(CMD_LINE) -o $@ -c $<

../../libcsuit.a:
	$(MAKE) -C ../../ MBEDTLS=$(MBEDTLS) libcsuit.a
