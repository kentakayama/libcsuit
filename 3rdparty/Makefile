#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

.PHONY: all
all: build_deps

include ../common.mk

.PHONY: build_deps
build_deps:
	$(MAKE) -C QCBOR
ifeq ($(MBEDTLS), 1)
	$(MAKE) -C mbedtls
endif
	$(MAKE) -C t_cose -f $(TCOSE_MAKEFILE) CRYPTO_INC="$(CRYPTO_INC)" CRYPTO_LIB="$(CRYPTO_LIB)"

.PHONY: clean
clean:
	$(MAKE) -C QCBOR clean
ifeq ($(MBEDTLS), 1)
	$(MAKE) -C mbedtls clean
endif
	$(MAKE) -C t_cose -f $(TCOSE_MAKEFILE) clean
