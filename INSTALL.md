## Install

This library uses two build systems, namely cmake and classical makefiles.
If you want to install libcsuit, 

### Using Makefiles

**(a) Use OpenSSL**

```bash
make
make install
```

**(b) Use MbedTLS**

```bash
make MBEDTLS=1
make MBEDTLS=1 install
```

> [!NOTE]
> QCBOR, t_cose and OpenSSL or MbedTLS are also required to be installed.

To generate Doxygen document use the following command:
```
make doc
```
and open `html/index.html`.

### Using CMake

The cmake file allows building code for OpenSSL and for Mbed TLS based on a parameter passed to cmake.
If you decide to use OpenSSL then you need to download and install it before building this library.
The OpenSSL library and the include files need to be included in the search path.

First, create a directory for the entire project. Inside this directory put the code to build mbedtls (if used), qcbor, t_cose and libcsuit.

**(a) Use OpenSSL**

```
make -C 3rdparty/QCBOR/ libqcbor.a
QCBOR_INC=-I../QCBOR/inc QCBOR_LIB=../QCBOR/libqcbor.a -C make -C 3rdparty/t_cose/ -f Makefile.ossl libt_cose.a
mkdir build
cd build
cmake ..
make
```

**(b) Use MbedTLS**

```
make -C 3rdparty/mbedtls/
make -C 3rdparty/QCBOR/ libqcbor.a
CRYPTO_INC=-I../mbedtls/include CRYPTO_LIB=../mbedtls/library/libmbedcrypto.a QCBOR_INC=-I../QCBOR/inc QCBOR_LIB=../QCBOR/libqcbor.a make -C 3rdparty/t_cose/ -f Makefile.psa libt_cose.a
mkdir build
cd build
cmake -DMBEDTLS=1 ..
make
```
