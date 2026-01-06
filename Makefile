#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

NAME := libcsuit

.PHONY: all
all: $(NAME).a

CFLAGS ?= -Os
WARNING_CFLAGS ?= -Wall -Wextra -Wformat=2 -Wno-format-nonliteral
LOCAL_CFLAGS := $(WARNING_CFLAGS) -fPIC -I ./inc -ffunction-sections -fdata-sections
# link me with -Wl,--gc-sections

include common.mk

SRCS = \
	src/suit_manifest_process.c \
	src/suit_manifest_callbacks.c \
	src/suit_manifest_decode.c \
	src/suit_manifest_encode.c \
	src/suit_manifest_print.c \
	src/suit_reporting_engine.c \
	src/suit_report_print.c \
	src/suit_cose.c \
	src/suit_digest.c \
	src/suit_common.c

PUBLIC_INTERFACE = \
	inc/csuit/config.h \
	inc/csuit/csuit.h \
	inc/csuit/suit_common.h \
	inc/csuit/suit_cose.h \
	inc/csuit/suit_digest.h \
	inc/csuit/suit_manifest_decode.h \
	inc/csuit/suit_manifest_encode.h \
	inc/csuit/suit_manifest_process.h \
	inc/csuit/suit_manifest_callbacks.h \
	inc/csuit/suit_manifest_print.h \
	inc/csuit/suit_reporting_engine.h

OBJS = $(SRCS:.c=.o)

.PHONY: so
so: $(NAME).so

.PHONY: doc
doc:
	doxygen Doxyfile

$(NAME).a: $(OBJS)
	$(AR) -r $@ $^

$(NAME).so: $(OBJS)
	$(CC) -shared $^ $(LOCAL_CFLAGS) $(CFLAGS) -o $@

%.o: %.c
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) $(INC) -o $@ -c $<

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

define install-header
	install -m 644 $1 $2/$(nodir $1)

endef

.PHONY: instal
install: $(NAME).a $(PUBLIC_INTERFACE)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(NAME).a $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/csuit
	$(foreach header,$(PUBLIC_INTERFACE),$(call install-header,$(header),$(DESTDIR)$(PREFIX)/include/csuit))

.PHONY: install_so
install_so: $(NAME).so $(PUBLIC_INTERFACE)
	install -m 755 $(NAME).so $(DESTDIR)$(PREFIX)/lib/$(NAME).so.1.0.0
	ln -sf $(NAME).so.1 $(DESTDIR)$(PREFIX)/lib/$(NAME).so
	ln -sf $(NAME).so.1.0.0 $(DESTDIR)$(PREFIX)/lib$(NAME).so.1

.PHONY: uninstall
uninstall:
	$(RM) -d $(DESTDIR)$(PREFIX)/include/csuit/*
	$(RM) -d $(DESTDIR)$(PREFIX)/include/csuit/
	$(RM) $(addprefix $(DESTDIR)$(PREFIX)/lib/, \
		$(NAME).a $(NAME).so $(NAME).so.1 $(NAME).so.1.0.0)

.PHONY: build_deps
build_deps:
	$(MAKE) -C 3rdparty

.PHONY: build_test
build_test: build_deps $(NAME).a
	$(MAKE) -C test MBEDTLS=$(MBEDTLS)
	$(MAKE) -C examples MBEDTLS=$(MBEDTLS)

.PHONY: test
test: build_test
	$(MAKE) -C test MBEDTLS=$(MBEDTLS) run
	$(MAKE) -C examples MBEDTLS=$(MBEDTLS) run

.PHONY: examples
examples: $(NAME).a
	$(MAKE) -C examples MBEDTLS=$(MBEDTLS)

.PHONY: clean
clean:
	$(MAKE) -C 3rdparty clean
	$(MAKE) -C test clean
	$(MAKE) -C examples clean
	$(RM) $(OBJS) $(NAME).a $(NAME).so
