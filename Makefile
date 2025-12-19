#
# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

NAME = libcsuit
CFLAGS ?= -Os
WARNING_CFLAGS ?= -Wall -Wextra -Wformat=2 -Wno-format-nonliteral
LOCAL_CFLAGS = $(WARNING_CFLAGS) -fPIC -I ./inc
LOCAL_CFLAGS += -ffunction-sections -fdata-sections
# link me with -Wl,--gc-sections

ifeq ($(MBEDTLS),1)
    # use MbedTLS
    LOCAL_CFLAGS += -DLIBCSUIT_PSA_CRYPTO_C=1
else
    # use OpenSSL
    MBEDTLS=0
endif


SRCS = \
	src/suit_common.c \
	src/suit_manifest_process.c \
	src/suit_reporting_engine.c \
	src/suit_manifest_callbacks.c \
	src/suit_manifest_decode.c \
	src/suit_manifest_encode.c \
	src/suit_manifest_print.c \
	src/suit_cose.c \
	src/suit_digest.c

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

OBJS = $(addprefix ./obj/,$(patsubst %.c,%.o,$(SRCS)))

.PHONY: all
all: $(NAME).a

include Makefile.common

.PHONY: so
so: $(NAME).so

.PHONY: doc
doc:
	doxygen Doxyfile

$(NAME).a: $(OBJS)
	$(AR) -r $@ $^

$(NAME).so: $(OBJS)
	$(CC) -shared $^ $(LOCAL_CFLAGS) $(CFLAGS) -o $@

./obj/%.o: %.c | ./obj/src
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -o $@ -c $<

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

.PHONY: build_test
build_test: $(NAME).a
	$(MAKE) -C test MBEDTLS=$(MBEDTLS) CMD_INC="$(CMD_INC)" CMD_LD="$(CMD_LD)"

.PHONY: test
test: build_test
	$(MAKE) -C test MBEDTLS=$(MBEDTLS) run

.PHONY: clean
clean:
	$(MAKE) -C test clean
	$(RM) $(OBJS) $(NAME).a $(NAME).so
