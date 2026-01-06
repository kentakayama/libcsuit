#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

TARGET_SIGN := ./suit_manifest_sign
OBJ_SIGN := suit_manifest_sign_main.o
TARGET_MAC := ./suit_manifest_mac
OBJ_MAC := suit_manifest_sign_mac_main.o
TARGET_TAM := ./suit_manifest_delegate_sign
OBJ_TAM := suit_manifest_sign_delegate_main.o

.PHONY: all
all: $(TARGET_SIGN) $(TARGET_MAC) $(TARGET_TAM)

CFLAGS ?= -Wall -g
INC ?= -I../inc -I../../inc #-I../../../3rdparty/QCBOR/inc -I../../../3rdparty/t_cose/inc -I../../../3rdparty/mbedtls/inc
LDFLAGS ?= ../../libcsuit.a -lt_cose -lqcbor -lm

include ../common.mk

$(OBJ_SIGN): suit_manifest_sign_main.c
	$(CC) $(CFLAGS) -DSUIT_MANIFEST_SIGNER_TRUST_ANCHOR=1 $(INC) -o $@ -c $<

$(TARGET_SIGN): $(OBJ_SIGN) ../suit_examples_common.o ../../libcsuit.a
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_MAC): suit_manifest_sign_main.c
	$(CC) $(CFLAGS) -DSUIT_MANIFEST_SIGNER_MAC=1 $(INC) -o $@ -c $<

$(TARGET_MAC): $(OBJ_MAC) ../suit_examples_common.o ../../libcsuit.a
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_TAM): suit_manifest_sign_main.c
	$(CC) $(CFLAGS) -DSUIT_MANIFEST_SIGNER_TAM=1 $(INC) -o $@ -c $<

$(TARGET_TAM): $(OBJ_TAM) ../suit_examples_common.o ../../libcsuit.a
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) *.o $(TARGET_SIGN) $(TARGET_MAC) $(TARGET_TAM)
