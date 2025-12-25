#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

TARGET := ./suit_manifest_encode

.PHONY: all
all: $(TARGET)

CFLAGS ?= -Wall -g
INC ?= -I../inc -I../../inc
LDFLAGS ?= -lt_cose -lqcbor -lm

include ../common.mk

SRCS = \
	suit_manifest_encode_main.c \
	../suit_examples_common.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS) ../../libcsuit.a
	$(CC) -o $@ $^ $(LDFLAGS)

OUTPUT := $(TESTFILES_DIR)/suit_manifest_expX.cbor
.PHONY: run
run: $(TARGET)
	$(TARGET) $(OUTPUT) || exit 1

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
