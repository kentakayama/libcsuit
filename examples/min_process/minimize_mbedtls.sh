#!/bin/bash

cd ../../3rdparty/mbedtls/

# MbedTLS internal
## Disable All at first
scripts/config.py unset-all MBEDTLS_*

## Enable SHA-256 and Deterministic ECDSA with P-256
scripts/config.py set MBEDTLS_ECP_C
scripts/config.py set MBEDTLS_ECDSA_C
scripts/config.py set MBEDTLS_ECDSA_DETERMINISTIC
scripts/config.py set MBEDTLS_ASN1_WRITE_C
scripts/config.py set MBEDTLS_ASN1_PARSE_C
scripts/config.py set MBEDTLS_ECP_DP_SECP256R1_ENABLED
scripts/config.py set MBEDTLS_BIGNUM_C
scripts/config.py set MBEDTLS_SHA256_C
scripts/config.py set MBEDTLS_HMAC_DRBG_C
scripts/config.py set MBEDTLS_ENTROPY_C
scripts/config.py set MBEDTLS_MD_C
scripts/config.py set MBEDTLS_USE_PSA_CRYPTO
scripts/config.py set MBEDTLS_PSA_CRYPTO_C

# PSA Interface
## Disable All at first
scripts/config.py -f include/psa/crypto_config.h unset-all PSA_*

## Enable SHA-256 and Deterministic ECDSA with P-256
scripts/config.py -f include/psa/crypto_config.h set PSA_WANT_ALG_SHA_256 1
scripts/config.py -f include/psa/crypto_config.h set PSA_WANT_ALG_ECDSA 1
scripts/config.py -f include/psa/crypto_config.h set PSA_WANT_ALG_DETERMINISTIC_ECDSA 1
