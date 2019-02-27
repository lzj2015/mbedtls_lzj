#ifndef ANDROID_MK_VER
#include <AisinoSSL/aisinossl_config.h>
#endif

#include <AisinoSSL/mbedtls/bignum.h>
#include <AisinoSSL/rsa/rsa_cloud.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <AisinoSSL/sm2/sm2.h>
#include <AisinoSSL/mbedtls/hmac_drbg.h>
#include <AisinoSSL/sm3/sm3.h>
int test_main();
int test_file(const char *path);

int main(int argc, char **argv) {

	if (argc <= 1) {
		printf("Please input test mode:\n");
		printf("1. M = Math test. No argument\n");
		printf("2. F = File test. Args: <Path>\n");
		return 0;
	}

	// Choose Test Mode
	switch (argv[1][0]) {
	case 'M':
		test_main();
		break;
	case 'F':
		if (argc <= 2) {
			printf("Please input working directory path.\n");
			return 0;
		}
		test_file(argv[2]);
		break;
	default:
		printf("Invalid test mode!\n");
	}
	return 0;
}
