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


