## Getting started

This library uses two build systems, namely cmake and classical makefiles.
To build with makefile you can use docker environment and we recommend you to use it.

### Using Docker(RECOMMENDED)

We recommend you to execute examples with docker
because it may install some files into your file system
depends on the input SUIT Manifest.

```bash
git clone --recursive https://github.com/kentakayama/libcsuit
cd ./libcsuit
```

**(a) Use OpenSSL**
```
docker build -t libcsuit_ossl -f ossl.Dockerfile .
docker run -t libcsuit_ossl
```

**(b) Use Mbed TLS**
```
docker build -t libcsuit_psa -f psa.Dockerfile .
docker run -t libcsuit_psa
```

### Using Makefiles

```bash
git clone --recursive https://github.com/kentakayama/libcsuit
make MBEDTLS=1 build_test
make MBEDTLS=1 test
```

To install libcsuit.a use the following command:
```
make install
```

To install libcsuit.so use the following command:
```
make install_so
```

To generate Doxygen document use the following command:
```
make doc
```
and open `html/index.html`.

### Using CMake

The cmake file allows building code for OpenSSL and for Mbed TLS based on a parameter passed to cmake.
If you decide to use OpenSSL then you need to download and install it before building this library.
The OpenSSL library and the include files need to be included in the search path.

First, create a directory for the entire project. Inside this directory put the code of qcbor, t_cose,
mbedtls (if used), and libcsuit.

Here are the commands:

```
git clone https://github.com/laurencelundblade/QCBOR.git
git clone https://github.com/laurencelundblade/t_cose.git
git clone https://github.com/ARMmbed/mbedtls.git
git clone https://github.com/kentakayama/libcsuit.git
cd QCBOR
make
sudo make install
cd ../libcsuit
```

Next, build the code using cmake

```
mkdir build
cd build
cmake -DMBEDTLS=1 ..
make
```

If you want to build the code for OpenSSL then omit the '-DMBEDTLS=1' parameter from the cmake invocation.


