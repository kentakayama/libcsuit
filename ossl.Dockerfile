# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
# SPDX-License-Identifier: BSD-2-Clause
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get -y install clang make libcunit1-dev libssl-dev ruby
RUN gem install cbor-diag

COPY . .
RUN make build_test

CMD make test