#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

# Compute absolute path to parent common.mk and include it once to avoid
# multiple relative-include variants that confuse make's include mechanism.
THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR := $(abspath $(dir $(THIS_MAKEFILE)))
include $(THIS_DIR)/../common.mk


%.o: ../../src/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

%.o: ../%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

../../libcsuit.a:
	$(MAKE) -C ../../ MBEDTLS=$(MBEDTLS) libcsuit.a
