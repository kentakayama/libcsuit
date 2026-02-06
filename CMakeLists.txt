cmake_minimum_required(VERSION 3.13)
project(libcsuit
	DESCRIPTION "C library for SUIT Manifest"
	LANGUAGES C
	VERSION 1.0.2)

set(CMAKE_C_FLAGS "-Wall -O3 -pedantic")
set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -g -DDEBUG") # use `-DCMAKE_BUILD_TYPE=Debug'

# imported QCBOR
add_library(qcbor_static STATIC IMPORTED)
set_target_properties(qcbor_static PROPERTIES
  	IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/3rdparty/QCBOR/libqcbor.a"
  	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/3rdparty/QCBOR/inc"
)

# imported t_cose
add_library(tcose_static STATIC IMPORTED)
set_target_properties(tcose_static PROPERTIES
  	IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/3rdparty/t_cose/libt_cose.a"
  	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/3rdparty/t_cose/inc"
)

# Common libraries for our example targets. Use the imported static
# targets when available, otherwise fall back to system crypto.
set(PROJECT_STATIC_LIBS qcbor_static tcose_static)

if(MBEDTLS)

	set(CRYPTO_LIBRARIES "mbedtls") #  "mbedx509" "mbedcrypto"
	option(USE_PKCS11_HELPER_LIBRARY "Build mbed TLS with the pkcs11-helper library." OFF)
	option(ENABLE_ZLIB_SUPPORT "Build mbed TLS with zlib library." OFF)
	option(ENABLE_PROGRAMS "Build mbed TLS programs." OFF)
	option(ENABLE_TESTING "Build mbed TLS tests." OFF)

	set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/lib)

	# imported mbedcrypto
	add_library(mbedcrypto_static STATIC IMPORTED)
	set_target_properties(mbedcrypto_static PROPERTIES
  		IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/3rdparty/mbedtls/library/libmbedcrypto.a"
  		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/3rdparty/mbedtls/include"
	)

	# Use custom config file for Mbed TLS
	#add_definitions(-DMBEDTLS_CONFIG_FILE="${CMAKE_SOURCE_DIR}/misc/config/mbedtls_config.h")

	option(USE_STATIC_MBEDTLS_LIBRARY "Build mbed TLS static library." off)
	option(USE_SHARED_MBEDTLS_LIBRARY "Build mbed TLS shared library." on)

	if (CMAKE_SYSTEM MATCHES "OpenBSD")
		option(LINK_WITH_PTHREAD "Explicitly link mbed TLS library to pthread." on)
	endif()

	add_definitions(-DMBEDTLS=1)

	# Compile code for use with the PSA Crypto API
	add_definitions(-DLIBCSUIT_PSA_CRYPTO_C=1)

	list(APPEND PROJECT_STATIC_LIBS mbedcrypto_static)
	set(CRYPTO_LIBRARIES mbedcrypto_static)

else()

	set(CRYPTO_LIBRARIES "crypto")

endif()

set(PROJECT_LINK_LIBS ${PROJECT_STATIC_LIBS} ${CRYPTO_LIBRARIES} -lm)

set(SOURCE_LIB
	src/suit_common.c
	src/suit_digest.c
	src/suit_cose.c
	src/suit_manifest_print.c
	src/suit_manifest_encode.c
	src/suit_manifest_decode.c
	src/suit_manifest_process.c
	src/suit_manifest_callbacks.c
	src/suit_reporting_engine.c
	src/suit_report_print.c
)
add_library(csuit STATIC ${SOURCE_LIB})
target_link_libraries(csuit PUBLIC ${PROJECT_LINK_LIBS})

include_directories(examples/inc)
include_directories(inc)

set(SOURCE_DECODE
	examples/suit_examples_common.c
	examples/parser/suit_manifest_parser_main.c
)
# Build parser executable and place binary into examples/parser/
add_executable(suit_manifest_parser ${SOURCE_DECODE})
set_target_properties(suit_manifest_parser PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/parser"
)
target_link_libraries(suit_manifest_parser PRIVATE csuit ${PROJECT_LINK_LIBS})

set(SOURCE_ENCODE
	examples/suit_examples_common.c
	examples/encode/suit_manifest_encode_main.c
)
# Build encode executable and place binary into examples/encode/
add_executable(suit_manifest_encode ${SOURCE_ENCODE})
set_target_properties(suit_manifest_encode PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/encode"
)
target_link_libraries(suit_manifest_encode PRIVATE csuit ${PROJECT_LINK_LIBS})

set(SOURCE_PROCESS
	examples/suit_examples_common.c
	examples/process/suit_manifest_process_main.c
)
# Build process executable and place binary into examples/process/
add_executable(suit_manifest_process ${SOURCE_PROCESS})
set_target_properties(suit_manifest_process PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/process"
)
target_link_options(suit_manifest_process PRIVATE "LINKER:--wrap=suit_fetch_callback")
target_link_options(suit_manifest_process PRIVATE "LINKER:--wrap=suit_store_callback")
target_link_options(suit_manifest_process PRIVATE "LINKER:--wrap=suit_invoke_callback")
target_link_options(suit_manifest_process PRIVATE "LINKER:--wrap=suit_condition_callback")
target_link_options(suit_manifest_process PRIVATE "LINKER:--wrap=suit_report_callback")
target_link_libraries(suit_manifest_process PRIVATE csuit ${PROJECT_LINK_LIBS})
