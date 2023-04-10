# [libcsuit](https://github.com/kentakayama/libcsuit/)
**libcsuit** is a C library for encoding and decoding [IETF SUIT manifests](https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest).
The manifest contains meta-data about the firmware image. The manifest is protected against modification and provides information
about the software/firmware author.

For more information on how the IETF SUIT manifest is used to protect firmware updates of IoT devices, please look at the
 [IETF SUIT architecture](https://datatracker.ietf.org/doc/html/rfc9019) document and the
 [IETF SUIT](https://datatracker.ietf.org/wg/suit/about/) working group.

Supported features are listed [SUPPORTED.md](./SUPPORTED.md).

## Overview

This implementation uses
 - the [QCBOR](https://github.com/laurencelundblade/QCBOR) library for encoding and decoding CBOR structures,
 - the [t_cose](https://github.com/laurencelundblade/t_cose) library for cryptographic processing of COSE structures,
 - OpenSSL or Mbed TLS (based on the PSA Crypto API) for cryptographic algorithms.

This implementation offers the functionality defined in 
- [draft-ietf-suit-manifest-22](https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest-22)
- [draft-ietf-suit-trust-domains-02](https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-02)
- [draft-ietf-suit-update-management-01](https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management-01)
- [draft-ietf-suit-firmware-encryption-11](https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption-11)
- ~~[draft-ietf-suit-report-05](https://datatracker.ietf.org/doc/html/draft-ietf-suit-report-05)~~

Example programs are offered for testing.

## Getting started
We recommend you to execute examples with docker
because it may install some files into your file system
depends on the input SUIT Manifest.

```bash
git clone https://github.com/kentakayama/libcsuit
cd ./libcsuit
```

**(a) Use OpenSSL**
```
docker build -t libcsuit_ossl -f ossl.Dockerfile .
docker run -t libcsuit_ossl ./bin/suit_manifest_process ./testfiles/suit_maniefst_expS1.cbor
```

**(b) Use Mbed TLS**
```
docker build -t libcsuit_psa -f psa.Dockerfile .
docker run -t libcsuit_psa ./bin/suit_manifest_process ./testfiles/suit_maniefst_expS1.cbor
```

**(c) Use OpenSSL 3**
```
docker build -t libcsuit_ossl3 -f ossl3.Dockerfile .
docker run -t libcsuit_ossl3 ./bin/suit_manifest_process ./testfiles/suit_maniefst_expS1.cbor
```

See [example SUIT Manifest](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS3.md) and [this tutorial](./libcsuit_progress_afterIETF116.pdf).  
See [INSTALL.md](./INSTALL.md) for other usages.

## License and Copyright
BSD 2-Clause License

Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
