#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

include ../common.mk

CFLAGS = $(CMD_C) -Wall -g
INC = $(CMD_INC) -I../inc -I../../inc #-I../../../3rdparty/QCBOR/inc -I../../../3rdparty/t_cose/inc -I../../../3rdparty/mbedtls/inc
LDFLAGS = $(CMD_LD) -lt_cose -lqcbor -lm

ifeq ($(MBEDTLS),1)
    # use MbedTLS
    CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C=1
    LDFLAGS += -lmbedcrypto
else
    # use OpenSSL
    LDFLAGS += -lcrypto
endif

LDWRAP = \
	-Xlinker --wrap=suit_fetch_callback \
	-Xlinker --wrap=suit_store_callback \
	-Xlinker --wrap=suit_invoke_callback \
	-Xlinker --wrap=suit_condition_callback

TARGET = ./suit_manifest_process
SRCS = \
	../suit_examples_common.c \
	../../src/suit_common.c \
	../../src/suit_digest.c \
	../../src/suit_cose.c \
	../../src/suit_manifest_process.c \
	../../src/suit_reporting_engine.c \
	../../src/suit_manifest_callbacks.c \
	../../src/suit_manifest_decode.c \
	../../src/suit_manifest_encode.c \
	../../src/suit_manifest_print.c \
	suit_manifest_process_main.c
OBJS = $(notdir $(SRCS:.c=.o))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDWRAP) -o $@ $(OBJS) $(LDFLAGS)

define test-one
	$(TARGET) ../../testfiles/suit_manifest_exp$(1).suit \
		-p "./tmp" \
		-u "https://cdn.example/example3.bin" -f "./testfiles/raw_image.bin" \
		-u "http://example.com/file.bin" -f "./testfiles/raw_image.bin" \
		-u "http://example.com/dependent.suit" -f "./testfiles/suit_manifest_expS0.suit" \
		-u "coaps://example.com/encrypted-firmware" -b "758c4b7bbae2c4c1d462423e0f0dc3164ffa7b85bb94d4bd6d7ed26ab32feb063385d4d3465927ec82cb5e198a59" \
		-u "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit" -f "./testfiles/suit_manifest_expU.suit" \
		-u "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.ta" -f "./testfiles/8d82573a-926d-4754-9353-32dc29997f74.ta" \
		-u "https://example.org/config.json" -f "./testfiles/config.json" || exit 1

endef

TEST_NUM := 0 1 2A 2B 3 4 5 U I D AF AFS AW EW ED S0 S2 S3 U0 U1
.PHONY: run
run: all
	$(foreach num,$(TEST_NUM),$(call test-one,$(num)))

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
