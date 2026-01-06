#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# NOTE: Uses examples/min_process/config.h via -include config.h flag
# This enables LIBCSUIT_DISABLE_XXX optimizations

TARGET := ./suit_manifest_process_min

.PHONY: all
all: $(TARGET)

CC := musl-gcc
CFLAGS := -Os -s -fno-ident -fno-asynchronous-unwind-tables
CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C

CRYPTO_PATH := $(realpath ../../3rdparty/mbedtls)
CRYPTO_INC := $(CRYPTO_PATH)/include
CRYPTO_LIB := $(CRYPTO_PATH)/library/libmbedcrypto.a
QCBOR_PATH := $(realpath ../../3rdparty/QCBOR)
QCBOR_INC := $(QCBOR_PATH)/inc
QCBOR_LIB := $(QCBOR_PATH)/libqcbor.a
TCOSE_PATH := $(realpath ../../3rdparty/t_cose)
TCOSE_INC := $(TCOSE_PATH)/inc
TCOSE_LIB := $(TCOSE_PATH)/libt_cose.a

INC := -include config.h -I./ -I../inc -I../../inc -I$(CRYPTO_INC) -I$(QCBOR_INC) -I$(TCOSE_INC)
LDFLAGS := -Wl,--gc-sections

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
	../../scr/suit_manifest_decode.c \
	../../src/suit_manifest_process.c \
	suit_manifest_process_main.c
OBJS = $(notdir $(SRCS:.c=.o))

.PHONY: mbedtls
mbedtls:
	./minimize_mbedtls.sh
	CC=$(CC) $(MAKE) -C $(CRYPTO_PATH) library/libmbedcrypto.a

.PHONY: QCBOR
QCBOR:
	CC=$(CC) $(MAKE) -C $(QCBOR_PATH) libqcbor.a

.PHONY: t_cose
t_cose:
	CC=$(CC) $(MAKE) -C $(TCOSE_PATH) -f Makefile.psa CRYPTO_INC=-I$(CRYPTO_INC) CRYPTO_LIB=$(CRYPTO_LIB) QCBOR_INC=-I$(QCBOR_INC) QCBOR_LIB=$(QCBOR_LIB) OTHER_OPTS="-DT_COSE_DISABLE_ES384 -DT_COSE_DISABLE_ES512 -DT_COSE_DISABLE_PS256 -DT_COSE_DISABLE_PS384 -DT_COSE_DISABLE_PS512 -DT_COSE_DISABLE_SHORT_CIRCUIT_SIGN -DT_COSE_DISABLE_COSE_SIGN -DT_COSE_DISABLE_KEYWRAP -DT_COSE_DISABLE_ESDH" libt_cose.a

%.o: ../../src/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

%.o: ../%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(TARGET): mbedtls QCBOR t_cose $(OBJS)
	$(CC) $(LDWRAP) -o $@ $(OBJS) $(TCOSE_LIB) $(QCBOR_LIB) $(CRYPTO_LIB) $(LDFLAGS)

.PHONY: run
run: $(TARGET)
	size $(TARGET)
	ls -lh $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
