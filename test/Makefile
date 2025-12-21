#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

CFLAGS = $(CMD_C) -Wall -g
LDFLAGS = $(CMD_LD) -lt_cose -lqcbor -lcunit -lm
INC = $(CMD_INC) -I ../inc/
TARGET = ./libcsuit_test
RESULT = result.txt

ifeq ($(MBEDTLS),1)
    # use MbedTLS
    CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C=1
    #LDFLAGS += -lmbedtls -lmbedx509
    LDFLAGS += -lmbedcrypto
else
    # use OpenSSL
    LDFLAGS += -lcrypto
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): main.c ../libcsuit.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INC)

../libcsuit.a:
	$(MAKE) -C ../ MBEDTLS=$(MBEDTLS)

run:
	$(TARGET)

clean:
	$(RM) $(TARGET) $(RESULT)
