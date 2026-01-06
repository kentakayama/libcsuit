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
	$(MAKE) -C QCBOR libqcbor.a
ifeq ($(MBEDTLS), 1)
	$(MAKE) -C mbedtls
endif
	$(MAKE) -C t_cose -f $(TCOSE_MAKEFILE) QCBOR_INC="$(QCBOR_INC)" QCBOR_LIB="$(QCBOR_LIB)" CRYPTO_INC="$(CRYPTO_INC)" CRYPTO_LIB="$(CRYPTO_LIB)" libt_cose.a

.PHONY: clean
clean:
	$(MAKE) -C QCBOR clean
	$(MAKE) -C mbedtls clean
	cd mbedtls && git checkout .
	$(MAKE) -C t_cose -f Makefile.ossl clean
	$(MAKE) -C t_cose -f Makefile.psa clean
