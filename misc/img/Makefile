#
# Copyright (c) 2020-2026 SECOM CO., LTD. All Rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#

.PHONY: all
all: process.svg

%.svg: %.aasvg
	aasvg --backdrop --spaces=0 < $< > $@