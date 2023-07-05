# Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
# SPDX-License-Identifier: BSD-2-Clause
# +minimize mbedtls
FROM debian:latest

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install curl git musl-tools make python3
ENV CC="musl-gcc -static"

RUN git clone -b v3.1.0 --depth 1 https://github.com/Mbed-TLS/mbedtls.git /root/mbedtls
COPY misc/config/mbedtls_config.h /root/mbedtls/include/mbedtls/
COPY misc/config/crypto_config.h /root/mbedtls/include/psa/
WORKDIR /root/mbedtls
RUN CFLAGS="-Os -fdata-sections -ffunction-sections" make install -j`nproc`

RUN git clone --depth 1 https://github.com/laurencelundblade/QCBOR.git /root/QCBOR
WORKDIR /root/QCBOR
RUN make libqcbor.a CMD_LINE="-fdata-sections -ffunction-sections" install

RUN git clone --depth 1 https://github.com/laurencelundblade/t_cose.git /root/t_cose
WORKDIR /root/t_cose
RUN make -f Makefile.psa CMD_LINE="-fdata-sections -ffunction-sections -DT_COSE_DISABLE_SHORT_CIRCUIT_SIGN -DT_COSE_DISABLE_ES384 -DT_COSE_DISABLE_ES512 -DT_COSE_DISABLE_PS256 -DT_COSE_DISABLE_PS384 -DT_COSE_DISABLE_PS512 -DT_COSE_DISABLE_EDDSA" libt_cose.a install

WORKDIR /root/libcsuit
COPY src /root/libcsuit/src/
COPY inc /root/libcsuit/inc/
COPY examples /root/libcsuit/examples/
COPY ["Makefile", "Makefile.min_process", "/root/libcsuit/"]
COPY misc/config/max_config.h /root/libcsuit/inc/csuit/config.h
RUN mkdir -p ./bin

RUN make MBEDTLS=1 install
RUN make -f Makefile.min_process CFLAGS="-Os -fdata-sections -ffunction-sections -DT_COSE_DISABLE_SHORT_CIRCUIT_SIGN -DT_COSE_DISABLE_ES384 -DT_COSE_DISABLE_ES512 -DT_COSE_DISABLE_PS256 -DT_COSE_DISABLE_PS384 -DT_COSE_DISABLE_PS512 -DT_COSE_DISABLE_EDDSA" LDFLAGS="-Wl,--gc-sections" MBEDTLS=1

CMD ls -la bin/suit_manifest_process && \
    ./bin/suit_manifest_process; echo "exit: $?"
