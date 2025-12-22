#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

# Include guard to prevent infinite recursion
ifdef COMMON_MK_INCLUDED
$(error common.mk is already included - circular include detected)
endif
COMMON_MK_INCLUDED := 1

# Get the directory of this makefile (only once)
TOPDIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

TESTFILES_DIR := $(TOPDIR)/testfiles/

# Crypto provider selection: mbedtls or openssl
MBEDTLS ?= 0

ifeq ($(MBEDTLS), 1)
    # use MbedTLS with PSA Crypto API
    CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C=1
    TCOSE_MAKEFILE := Makefile.psa
else
    # use OpenSSL
    TCOSE_MAKEFILE := Makefile.ossl
endif

ifeq ($(USE_SYSTEM_LIBS), 1)
    # System-wide installed libraries
    QCBOR_INC := -I/usr/local/include
    QCBOR_LIB := -lqcbor -lm
    TCOSE_INC := -I/usr/local/include
    TCOSE_LIB := -lt_cose
ifeq ($(MBEDTLS), 1)
    CRYPTO_INC := -I/usr/local/include
    CRYPTO_LIB := -lmbedcrypto
else
    CRYPTO_INC := -I/usr/local/include
    CRYPTO_LIB := -lcrypto
endif
else
    QCBOR_PATH := $(TOPDIR)/3rdparty/QCBOR
    QCBOR_INC := -I$(QCBOR_PATH)/inc
    QCBOR_LIB := -L$(QCBOR_PATH) -lqcbor
    TCOSE_PATH := $(TOPDIR)/3rdparty/t_cose
    TCOSE_INC := -I$(TCOSE_PATH)/inc
    TCOSE_LIB := -L$(TCOSE_PATH) -lt_cose
ifeq ($(MBEDTLS), 1)
    CRYPTO_PATH := $(TOPDIR)/3rdparty/mbedtls
    CRYPTO_INC := -I$(CRYPTO_PATH)/include
    CRYPTO_LIB := -L$(CRYPTO_PATH)/library -lmbedcrypto
else
    CRYPTO_INC := -I/usr/local/include
    CRYPTO_LIB := -lcrypto
endif
endif

INC += $(QCBOR_INC) $(TCOSE_INC) $(CRYPTO_INC)
LDFLAGS += $(QCBOR_LIB) $(TCOSE_LIB) $(CRYPTO_LIB)
