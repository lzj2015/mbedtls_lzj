/*
 * @Author: Weijie Li
 * @Date: 2017-11-27 17:14:32
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2017-12-27 16:31:01
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils/testutil.h"
#ifndef ANDROID_MK_VER
#include <AisinoSSL/aisinossl_config.h>
#endif

#ifdef __SM4_ENABLE__
#include <AisinoSSL/sm4/sm4.h>

int test_sm4_cbc128(int idx);
int test_sm4_ctr128(int idx);
int test_sm4_wrap(int idx);
int test_sm4_gcm128(int idx);
int sm4test_main(int argc, char **argv);

int test_sm4_gcm128_file(int idx, const char *dir_path);

#ifdef SM4_WHITEBOX_ENABLE
    int test_sm4_wb128(int idx);
    int test_sm4_wb_ctr128(int idx);
    int test_sm4_wb_cbc128(int idx);
    int math_test();

    int test_sm4_wb_gcm128_file(int idx, const char *dir_path);
#endif /* SM4_WHITEBOX_ENABLE */

#endif /* __SM4_ENABLE__ */

#ifdef __AES_ENABLE__
#include <AisinoSSL/mbedtls/aes.h>

int test_aes_cbc128(int idx);
int test_aes_ctr128(int idx);
int test_aes_wrap(int idx);
int test_aes_gcm128(int idx);

int test_aes_gcm128_file(int idx, const char *dir_path);
#endif /* __AES_ENABLE__ */

#include <AisinoSSL/sm3/sm3.h>

#ifdef __SM2_ENABLE__
#include <AisinoSSL/sm2/sm2.h>
#endif /* __SM2_ENABLE__ */

int sm3_test(int verbose);
int sha256_test(int verbose);


int test_main()
{
    printf("Start Test:\n");

 #ifdef DUMMY_ROUND
     printf("dummy round debug mode\n");
 #endif

    int err = 0;
    int verbose = 0;

#ifdef __AES_ENABLE__
    TEST_TRUE_N(test_aes_cbc128, 3, verbose, err);

    TEST_TRUE_N(test_aes_ctr128, 3, verbose, err);

    TEST_TRUE_N(test_aes_wrap, 3, verbose, err);

    TEST_TRUE_N(test_aes_gcm128, 20, verbose, err);
#endif /* __AES_ENABLE__ */

#ifdef __SM4_ENABLE__
    TEST_TRUE_N(test_sm4_cbc128, 3, verbose, err);

    TEST_TRUE_N(test_sm4_ctr128, 3, verbose, err);

    TEST_TRUE_N(test_sm4_wrap, 3, verbose, err);

    TEST_TRUE_N(test_sm4_gcm128, 20, verbose, err);

#ifdef SM4_WHITEBOX_ENABLE
    TEST_TRUE_N(test_sm4_wb128, 4, verbose, err);
    TEST_TRUE_N(test_sm4_wb_ctr128, 3, verbose, err);
    TEST_TRUE_N(test_sm4_wb_cbc128, 3, verbose, err);

    //  math_test();
#endif /* SM4_WHITEBOX_ENABLE */

#endif /* __SM4_ENABLE__ */

    if (sm3_test(verbose) == 0) {
        printf("SM3: PASS\n");
    }
    else {
        printf("SM3: NOT PASS\n");
        err += 1;
    }

    if (sha256_test(verbose) == 0) {
        printf("SHA256: PASS\n");
    } else {
        printf("SHA256: NOT PASS\n");
        err += 1;
    }

    sm4test_main(0, NULL);

    printf("ERR: %d\n", err);


	return err;
}


int test_file(const char *path)
{
    int err = 0;
    int len = strlen(path);
    char dir_path[len + 2];
    memcpy(dir_path, path, len + 1);
    if(path[len - 1] != '/'){
        memcpy(dir_path + len, "/", 2);
    }

    #ifdef __SM4_ENABLE__

    if (test_sm4_gcm128_file(0, dir_path) == 0) {
        printf("sm4 gcm file: PASS\n");
    } else {
        printf("sm4 gcm file: NOT PASS\n");
        err++;
    }

    #ifdef SM4_WHITEBOX_ENABLE
        if (test_sm4_wb_gcm128_file(0, dir_path) == 0) {
            printf("sm4 whitebox gcm file: PASS\n");
        }else{
            printf("sm4 whitebox gcm file: NOT PASS\n");
            err++;
        }
    #endif /* SM4_WHITEBOX_ENABLE */

    #endif /* __SM4_ENABLE__ */

    #ifdef __AES_ENABLE__

    if (test_aes_gcm128_file(0, dir_path) == 0) {
        printf("aes gcm file: PASS\n");
    } else {
        printf("aes gcm file: NOT PASS\n");
        err++;
    }
    #endif /* __AES_ENABLE__ */

    return err;
}
