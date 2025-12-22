#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

.PHONY: all
all:
	$(MAKE) -C encode MBEDTLS=$(MBEDTLS)
	$(MAKE) -C parser MBEDTLS=$(MBEDTLS)
	$(MAKE) -C process MBEDTLS=$(MBEDTLS)
	$(MAKE) -C min_process MBEDTLS=$(MBEDTLS)
	$(MAKE) -C encrypt MBEDTLS=$(MBEDTLS)
	$(MAKE) -C sign MBEDTLS=$(MBEDTLS)

.PHONY: run
run:
	$(MAKE) -C encode MBEDTLS=$(MBEDTLS) run
	$(MAKE) -C parser MBEDTLS=$(MBEDTLS) run
	$(MAKE) -C process MBEDTLS=$(MBEDTLS) run
	$(MAKE) -C encrypt MBEDTLS=$(MBEDTLS) test

.PHONY: clean
clean:
	$(MAKE) -C encode MBEDTLS=$(MBEDTLS) clean
	$(MAKE) -C parser MBEDTLS=$(MBEDTLS) clean
	$(MAKE) -C process MBEDTLS=$(MBEDTLS) clean
	$(MAKE) -C min_process MBEDTLS=$(MBEDTLS) clean
	$(MAKE) -C encrypt MBEDTLS=$(MBEDTLS) clean
	$(MAKE) -C sign MBEDTLS=$(MBEDTLS) clean
