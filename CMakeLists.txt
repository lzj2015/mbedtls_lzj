# 0. Options for Compiler
option(DEBUG "debug" OFF)
option(SSL_TEST "test" OFF)

# For Modules
option(WITH_SM4 "With SM4 Module" ON)
option(WITH_RSA "With RSA Module" ON)
option(WITH_SM2 "With SM2 Module" ON)

option(ENABLE_ZLIB_SUPPORT "Build mbed TLS with zlib library." OFF)
option(LINK_WITH_PTHREAD "Explicitly link mbed TLS library to pthread." OFF)

# 1. Set cmake version
cmake_minimum_required(VERSION 3.2)

# 2. Set project name
PROJECT(SSL)

# 3. set environment variable
# 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"

# set name for build
SET(BUILD_NAME ${PROJECT_NAME})

# set debug
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(DEBUG ON)
ENDIF()

SET(BUILD_SHARED_NAME ${PROJECT_NAME}_SHARED)
SET(BUILD_STATIC_NAME ${PROJECT_NAME}_STATIC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

IF(BUILD_TARGET)
  message("\nBuild: ${BUILD_TARGET}")
ENDIF()

# iOS Setting
IF(BUILD_TARGET STREQUAL iOS)
    SET(CMD "xcrun -sdk iphoneos --show-sdk-path")
    EXEC_PROGRAM(${CMD} OUTPUT_VARIABLE iOS_SDK_PATH)
    SET(CMD "xcrun -sdk iphoneos --find clang")
    EXEC_PROGRAM(${CMD} OUTPUT_VARIABLE CLANG_PATH)
    set(CMAKE_C_COMPILER ${CLANG_PATH})
    #-F${iOS_SDK_PATH}/System/Library/Frameworks
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -isysroot ${iOS_SDK_PATH} -Wunused-command-line-argument -arch arm64 -arch armv7 -miphoneos-version-min=8.0 -fembed-bitcode")
    message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
    message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
ENDIF()

# ------------------
# Compiler Environment Configure
#
message("\nCMake Complier IS:")

IF(DEBUG)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -O0 -g3")
ENDIF(DEBUG)

if(CMAKE_COMPILER_IS_GNUCC)
    # some warnings we want are not available with old GCC versions
    # note: starting with CMake 2.8 we could use CMAKE_C_COMPILER_VERSION
    message("GNUCC\n")
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -fPIC -D_POSIX_SOURCE=600 -D_POSIX_C_SOURCE=200112L -std=c99 -Wall -Wextra -W -Wdeclaration-after-statement -Wwrite-strings")
    if (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wlogical-op")
    endif()
    if (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
    endif()
    set(CMAKE_C_FLAGS_RELEASE     "-O2")
    set(CMAKE_C_FLAGS_DEBUG       "-O0 -g3")
    set(CMAKE_C_FLAGS_COVERAGE    "-O0 -g3 --coverage")
    set(CMAKE_C_FLAGS_ASAN        "-Werror -fsanitize=address -fno-common -O3")
    set(CMAKE_C_FLAGS_ASANDBG     "-Werror -fsanitize=address -fno-common -O1 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls ")
    set(CMAKE_C_FLAGS_CHECK       "-Werror -Os")
    set(CMAKE_C_FLAGS_CHECKFULL   "${CMAKE_C_FLAGS_CHECK} -Wcast-qual")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_COMPILER_IS_CLANG)
    message("CLang\n")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -W -Wdeclaration-after-statement -Wwrite-strings -Wpointer-arith -Wimplicit-fallthrough -Wshadow")
    set(CMAKE_C_FLAGS_RELEASE     "-O2")
    set(CMAKE_C_FLAGS_DEBUG       "-O0 -g3")
    set(CMAKE_C_FLAGS_COVERAGE    "-O0 -g3 --coverage")
    set(CMAKE_C_FLAGS_ASAN        "-Werror -fsanitize=address -fno-common -fsanitize=undefined -fno-sanitize-recover -O3")
    set(CMAKE_C_FLAGS_ASANDBG     "-Werror -fsanitize=address -fno-common -fsanitize=undefined -fno-sanitize-recover -O1 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls ")
    set(CMAKE_C_FLAGS_MEMSAN      "-Werror -fsanitize=memory -O3")
    set(CMAKE_C_FLAGS_MEMSANDBG   "-Werror -fsanitize=memory -O1 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2")
    set(CMAKE_C_FLAGS_CHECK       "-Werror -Os")
endif(CMAKE_COMPILER_IS_CLANG)

if(MSVC)
    # Strictest warnings, and treat as errors
    message("MicroSoft Visual C\n")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
endif(MSVC)

# ------------------
# Compiler Module Configure
#
message("Compiler Module Configure:")

# SM4

IF(WITH_SM4)
    message("SM4 ON")
    set(__SM4_ENABLE__ 1)
ELSE(WITH_SM4)
    message("SM4 OFF")
ENDIF(WITH_SM4)

IF(WITH_RSA)
    message("RSA ON")
    set(__RSA_ENABLE__ 1)
ELSE(WITH_RSA)
    message("RSA OFF")
ENDIF(WITH_RSA)

IF(WITH_SM2)
    message("SM2 ON")
    set(__SM2_ENABLE__ 1)
ELSE(WITH_SM2)
    message("SM2 OFF")
ENDIF(WITH_SM2)

SET(HEADER_DIR "include/SSL")

# mbedtls zlib support
if(ENABLE_ZLIB_SUPPORT)
    find_package(ZLIB)

    if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIR})
    endif(ZLIB_FOUND)
endif(ENABLE_ZLIB_SUPPORT)


if(LINK_WITH_PTHREAD)
	find_package(THREADS)

	if(NOT THREADS_FOUND)
	     message(FATAL_ERROR "pthread not find, link pthread error.")
	endif(NOT THREADS_FOUND)
endif(LINK_WITH_PTHREAD)

# 5. Generate config file
configure_file(ssl_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/SSL/ssl_config.h @ONLY)

#
IF(BUILD_TARGET STREQUAL Android)
  message(FATAL_ERROR
  "Finished to generate <ssl_config.h>! \n"
  "If you want to compile, please run 'ndk-build NDK_PROJECT_PATH=./ NDK_APPLICATION_MK=./Application.mk'."
  )
ENDIF()

# 6. head file path，头文件目录
INCLUDE_DIRECTORIES(include)

#sub cmake
add_subdirectory(library)
add_subdirectory(include)

# test
IF(SSL_TEST)
    add_subdirectory(test)
ELSE(SSL_TEST)
    message("${PROJECT_NAME} TEST OFF")
ENDIF(SSL_TEST)
