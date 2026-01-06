#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

TARGET_AES = ./suit_manifest_encrypt_aes
TARGET_ESDH = ./suit_manifest_encrypt_esdh
TARGET_DECRYPT = ./suit_manifest_decrypt

.PHONY: all
all: $(TARGET_AES) $(TARGET_ESDH) $(TARGET_DECRYPT)

CFLAGS ?= -Wall -g
INC ?= -I../inc -I../../inc #-I../../../3rdparty/QCBOR/inc -I../../../3rdparty/t_cose/inc -I../../../3rdparty/mbedtls/inc
LDFLAGS ?= ../../libcsuit.a -lt_cose -lqcbor -lm

SRC_AES = suit_manifest_encrypt_aes_main.c
SRC_ESDH = suit_manifest_encrypt_esdh_main.c
SRC_DECRYPT = suit_manifest_decrypt_main.c

include ../common.mk

$(TARGET_AES): ../suit_examples_common.o suit_manifest_encrypt_aes_main.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_ESDH): ../suit_examples_common.o suit_manifest_encrypt_esdh_main.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_DECRYPT): ../suit_examples_common.o suit_manifest_decrypt_main.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: run
run: $(TARGET_AES) $(TARGET_ESDH)
	$(TARGET_AES) $(TESTFILES_DIR)/raw_image.bin $(TESTFILES_DIR)/encrypted_image_aes.bin $(TESTFILES_DIR)/encryption_info_aes.cose || exit 1
	$(TARGET_AES) $(TESTFILES_DIR)/config.json $(TESTFILES_DIR)/encrypted_config_aes.bin $(TESTFILES_DIR)/encryption_info_config_aes.cose || exit 1
	$(TARGET_ESDH) $(TESTFILES_DIR)/raw_image.bin $(TESTFILES_DIR)/encrypted_image_esdh.bin $(TESTFILES_DIR)/encryption_info_esdh.cose || exit 1
	$(TARGET_ESDH) $(TESTFILES_DIR)/config.json $(TESTFILES_DIR)/encrypted_config_esdh.bin $(TESTFILES_DIR)/encryption_info_config_esdh.cose || exit 1

.PHONY: test
test: $(TARGET_DECRYPT)
	$(TARGET_DECRYPT) $(TESTFILES_DIR)/raw_image.bin $(TESTFILES_DIR)/encrypted_image_aes.bin $(TESTFILES_DIR)/encryption_info_aes.cose || exit 1
	$(TARGET_DECRYPT) $(TESTFILES_DIR)/config.json $(TESTFILES_DIR)/encrypted_config_aes.bin $(TESTFILES_DIR)/encryption_info_config_aes.cose || exit 1
	$(TARGET_DECRYPT) $(TESTFILES_DIR)/raw_image.bin $(TESTFILES_DIR)/encrypted_image_esdh.bin $(TESTFILES_DIR)/encryption_info_esdh.cose || exit 1
	$(TARGET_DECRYPT) $(TESTFILES_DIR)/config.json $(TESTFILES_DIR)/encrypted_config_esdh.bin $(TESTFILES_DIR)/encryption_info_config_esdh.cose || exit 1

.PHONY: clean
clean:
	$(RM) *.o $(TARGET_AES) $(TARGET_ESDH) $(TARGET_DECRYPT)
