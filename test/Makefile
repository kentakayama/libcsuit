#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

TARGET = ./libcsuit_test

.PHONY: all
all: $(TARGET)

CFLAGS ?= -Wall -g
INC ?= -I ../inc/
LDFLAGS ?= -lt_cose -lqcbor -lcunit -lm

include ../common.mk

LDWRAP = \
	-Xlinker --wrap=suit_fetch_callback \
	-Xlinker --wrap=suit_store_callback \
	-Xlinker --wrap=suit_invoke_callback \
	-Xlinker --wrap=suit_condition_callback \
	-Xlinker --wrap=suit_report_callback

$(TARGET): process_test.c main.c ../libcsuit.a
	$(CC) $(LDWRAP) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INC)

../libcsuit.a:
	$(MAKE) -C ../ MBEDTLS=$(MBEDTLS)

.PHONY: run
run: $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)
