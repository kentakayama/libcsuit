#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# NOTE: Uses examples/min_process/config.h via -I./ flag
# This enables LIBCSUIT_DISABLE_XXX optimizations

TARGET = ../../bin/suit_manifest_process_min

.PHONY: all
all: $(TARGET)

include ../common.mk

#CC = musl-gcc
CFLAGS = $(CMD_C) -Os -s -fno-ident -fno-asynchronous-unwind-tables
INC = $(CMD_INC) -I./ -I../inc -I../../inc #-I../../../3rdparty/QCBOR/inc -I../../../3rdparty/t_cose/inc -I../../../3rdparty/mbedtls/inc
LDFLAGS = $(CMD_LD) -lt_cose -lqcbor -lm -Wl,--gc-sections

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

$(TARGET): $(OBJS)
	$(CC) $(LDWRAP) -o $@ $(OBJS) $(LDFLAGS)
	strip $(TARGET)

.PHONY: run
run: $(TARGET)
	$(TARGET) ../../testfiles/suit_manifest_exp0.suit

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
