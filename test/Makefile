#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
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

$(TARGET): main.c ../libcsuit.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INC)

../libcsuit.a:
	$(MAKE) -C ../ MBEDTLS=$(MBEDTLS)

.PHONY: run
run:
	$(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)
