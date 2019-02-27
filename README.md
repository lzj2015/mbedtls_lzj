# SSL

## Module Introduction

1. mbedtls module
2. sm2
3. rsa mobile signature module
4. sm4 
5. sm3

## build and install

Except Android,
you can create a project and build SSL by cmake.

Just like:

```
$ cmake [params] PATH/TO/SSL
$ make
$ make install
```

If you want to build Android library,
please read `examples/build for Android` in this docs.

### params

#### debug / release mode

1. Debug `-DCMAKE_BUILD_TYPE=Debug`
2. Release `-DCMAKE_BUILD_TYPE=Release`

#### build executable test program

```
-DSSL_TEST=On
```

#### Ignore some required modules

Default compile SM4, RSA, SM2 Modules,
you can ignore some of them:

- Ignore SM4 Module: `-DWITH_SM4=Off`
- Ignore RSA Module: `-DWITH_RSA=Off`
- Ignore SM2 Module: `-DWITH_SM2=Off`

#### set install path

```
-DCMAKE_INSTALL_PREFIX=INSTALL/PATH
```

### Examples commands


#### build for iOS in xcode

```
cmake -GXcode -DCMAKE_BUILD_TYPE=Debug /path/to/source
```

#### build for macOS

```
cmake  -DSSL_TEST=On  -DCMAKE_BUILD_TYPE=Debug /path/to/source
```

#### build for Android

```
cmake  -DSSL_TEST=On -DCMAKE_BUILD_TYPE=Debug /path/to/source
```

**NOTE**: cmake toolchain has to be android ndk toochain.

SUPPORT All OS.

#### build for Windows

Environment:

- Windows 7 or laster
- Visual Studio 2015 or laster

Commands for creating VS project:

```
$ mkdir build
$ cd build
$ cmake build ..
```

Open Visual Studio Project File `SSL.sln` and BUILD **RELEASE** version.

After buidling, you can find `SSL.lib` in `release` directory.

## SSL Defines

If you `#include <SSL/SSL_config.h>`,
you can easily know which modules are complied.

- `__AES_ENABLE__`: Enabled AES
- `__SM4_ENABLE__`: Enabled SM4
- `__RSA_ENABLE__`: Enabled RSA
- `__SM2_ENABLE__`: Enabled SM2
- `SM4_WHITEBOX_ENABLE`: Enabled SM4 WhiteBox

## Problems

1. Some Operating System DO NOT SUPPORT test code, so please Add argument: `-DSSL_TEST=Off`


mail: liuzhj28@gmail.com
