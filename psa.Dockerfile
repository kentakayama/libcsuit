# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
# SPDX-License-Identifier: BSD-2-Clause
FROM python:3.11-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get -y install curl git gcc make xxd libcunit1-dev ruby python3 python3-pip
RUN gem install cbor-diag

COPY . .
RUN python3 -m pip install --user -r ./3rdparty/mbedtls/scripts/basic.requirements.txt
RUN make MBEDTLS=1 build_test 

CMD make test MBEDTLS=1