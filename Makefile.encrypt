#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

CFLAGS = $(CMD_C) -Wall -g
LDFLAGS = $(CMD_LD) -lt_cose -lqcbor -lm
INC = $(CMD_INC) -I ./inc -I ./examples/inc
TARGET_AES = ./bin/suit_manifest_encrypt_aes
SRC_AES = examples/suit_manifest_encrypt_aes_main.c
TARGET_ESDH = ./bin/suit_manifest_encrypt_esdh
SRC_ESDH = examples/suit_manifest_encrypt_esdh_main.c
TARGET_DECRYPT = ./bin/suit_manifest_decrypt
SRC_DECRYPT = examples/suit_manifest_decrypt_main.c

SRCS = \
	examples/suit_examples_common.c \
	src/suit_common.c \
	src/suit_digest.c \
	src/suit_cose.c \
	src/suit_manifest_encode.c \
	src/suit_manifest_decode.c \
	src/suit_manifest_print.c
OBJS = $(addprefix ./obj/,$(patsubst %.c,%.o,$(SRCS)))

ifeq ($(MBEDTLS),1)
    # use MbedTLS
    CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C=1
    LDFLAGS += -lmbedcrypto
else
    # use OpenSSL
    LDFLAGS += -lcrypto
endif

.PHONY: all
all: $(TARGET_AES) $(TARGET_ESDH) $(TARGET_DECRYPT)

include Makefile.common

./obj/%.o: %.c | ./obj/src ./obj/examples
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(TARGET_AES): $(OBJS) ./obj/examples/suit_manifest_encrypt_aes_main.o | ./bin
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_ESDH): $(OBJS) ./obj/examples/suit_manifest_encrypt_esdh_main.o | ./bin
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_DECRYPT): $(OBJS) ./obj/examples/suit_manifest_decrypt_main.o | ./bin
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: run
run: $(TARGET_AES) $(TARGET_ESDH)
	$(TARGET_AES) ./testfiles/raw_image.bin ./testfiles/encrypted_image_aes.bin ./testfiles/encryption_info_aes.cose
	$(TARGET_AES) ./testfiles/config.json ./testfiles/encrypted_config_aes.bin ./testfiles/encryption_info_config_aes.cose
	$(TARGET_ESDH) ./testfiles/raw_image.bin ./testfiles/encrypted_image_esdh.bin ./testfiles/encryption_info_esdh.cose
	$(TARGET_ESDH) ./testfiles/config.json ./testfiles/encrypted_config_esdh.bin ./testfiles/encryption_info_config_esdh.cose

.PHONY: test
test: $(TARGET_DECRYPT)
	$(TARGET_DECRYPT) ./testfiles/raw_image.bin ./testfiles/encrypted_image_aes.bin ./testfiles/encryption_info_aes.cose
	$(TARGET_DECRYPT) ./testfiles/config.json ./testfiles/encrypted_config_aes.bin ./testfiles/encryption_info_config_aes.cose
	$(TARGET_DECRYPT) ./testfiles/raw_image.bin ./testfiles/encrypted_image_esdh.bin ./testfiles/encryption_info_esdh.cose
	$(TARGET_DECRYPT) ./testfiles/config.json ./testfiles/encrypted_config_esdh.bin ./testfiles/encryption_info_config_esdh.cose

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET_AES) $(TARGET_ESDH)

