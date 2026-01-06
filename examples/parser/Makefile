#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

TARGET := ./suit_manifest_parser

.PHONY: all
all: $(TARGET)

SHELL := /bin/bash
CFLAGS ?= -Wall -g
INC ?= -I../inc -I../../inc #-I../../../3rdparty/QCBOR/inc -I../../../3rdparty/t_cose/inc -I../../../3rdparty/mbedtls/inc
LDFLAGS ?= ../../libcsuit.a -lt_cose -lqcbor -lm
SRCS = \
	suit_manifest_parser_main.c \
	../suit_examples_common.c
OBJS = $(notdir $(SRCS:.c=.o))

DIAG2CBOR=diag2cbor.rb

include ../common.mk

$(TARGET): $(OBJS) ../../libcsuit.a
	$(CC) $(LDWRAP) -o $@ $(OBJS) $(LDFLAGS)

TEST_NUM := 0 1 2A 2B 3 4 5 U I D AF AFS AW EW ED S0 S2 S3 U0 U1
EXPS := $(foreach num,$(TEST_NUM),${TESTFILES_DIR}/suit_manifest_exp$(num).suit)
define test-one
	set -o pipefail; $(TARGET) $1 2 | tee tmp.result
	@grep '^    ' tmp.result | $(DIAG2CBOR) > tmp.printed.cbor
	@diff $1 tmp.printed.cbor || exit 1
	@echo "[SUCCESS] Succcessfully parsed and printed $1"
	@$(RM) tmp.result tmp.printed.cbor

endef

.PHONY: run
run: $(TARGET)
	$(foreach exp,$(EXPS),$(call test-one,$(exp)))

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
