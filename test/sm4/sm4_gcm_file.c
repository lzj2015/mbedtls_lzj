#include <AisinoSSL/sm4/sm4.h>
#include <AisinoSSL/mbedtls/md.h>
#include <time.h>
#include "../utils/testutil.h"

#define TEST_TIMES 7
#define IN_PATH  "input.txt"
#define OUT_PATH  "output.txt"
#define C_PATH  "mycipher.txt"
#define TEST_MSG "abcdefghijklmnopqrstyvwxyz1234567890"

static const unsigned char test_k[] = {
  0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
  0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
  };
static const unsigned char test_iv[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static int _test_sm4_gcm128_initfile(int times, char *in_path) {
	FILE *fp;
	if ((fp = fopen(in_path, "wb")) && fp == NULL) {
		return 0xFF01;
	}

	for (int i = 0; i < times; i++) {
		fputs(TEST_MSG, fp);
	}

	fclose(fp);
  return 0;
}

static int _test_sm4_gcm128_encrypt_file(char *in_path, char *c_path) {
	SM4_KEY sm4_key;
  sm4_set_encrypt_key(&sm4_key, test_k);

	sm4_gcmf_context ctx;
	sm4_gcmf_init(&ctx, &sm4_key); // Init GCMF Context

	sm4_gcmf_set_iv(&ctx, test_iv, sizeof(test_iv)); // Set GCMF IV
	int ret = sm4_gcmf_encrypt_file(&ctx, in_path, c_path); // Encrypt

	sm4_gcmf_free(&ctx); // Release GCMF Context
  return ret;
}

static int _test_sm4_gcm128_decrypt_file(char *c_path, char *out_path) {
	SM4_KEY sm4_key;
  sm4_set_encrypt_key(&sm4_key, test_k);

	sm4_gcmf_context ctx;
	sm4_gcmf_init(&ctx, &sm4_key);

	sm4_gcmf_set_iv(&ctx, test_iv, sizeof(test_iv)); // Set GCMF IV
	int ret = sm4_gcmf_decrypt_file(&ctx, c_path, out_path); // Decrypt

	sm4_gcmf_free(&ctx); // Release GCMF Context
  return 0;
}

int test_sm4_gcm128_file(int idx, const char *dir_path) {
  	int ret, i;
  	clock_t start, end;
  	unsigned char plainHash[32], recoverHash[32];

		int len = strlen(dir_path);

		char in_path[len + strlen(IN_PATH) + 1];
		memcpy(in_path, dir_path, len);
		memcpy(in_path + len, IN_PATH, strlen(IN_PATH) + 1);

		char out_path[len + strlen(OUT_PATH) + 1];
		memcpy(out_path, dir_path, len);
		memcpy(out_path + len, OUT_PATH, strlen(OUT_PATH) + 1);

		char c_path[len + strlen(C_PATH) + 1];
		memcpy(c_path, dir_path, len);
		memcpy(c_path + len, C_PATH, strlen(C_PATH) + 1);

  	int times[] = {
  		1, 10, 100, 1000, 10000, 100000, 1000000
  	};

  	for (i = 0; i < TEST_TIMES; i++) {
  		memset(plainHash, 0, 32 * sizeof(unsigned char));
  		memset(recoverHash, 0, 32 * sizeof(unsigned char));

  		ret = _test_sm4_gcm128_initfile(times[i], in_path);
  		if (ret != 0) break;

  		start = clock();
  		ret = _test_sm4_gcm128_encrypt_file(in_path, c_path);
  		end = clock();
  		if (ret != 0) break;
  		printf("#%d SM4 GCM FILE ENCRYPT: time = %.0fms\n", i, difftime(end, start));

  		start = clock();
  		ret = _test_sm4_gcm128_decrypt_file(c_path, out_path);
  		end = clock();
  		if (ret != 0) break;
  		printf("#%d SM4 GCM FILE ENCRYPT: time = %.0fms\n", i, difftime(end, start));

  		ret = mbedtls_md_file(mbedtls_md_info_from_string("SM3"), in_path, plainHash);
  		if (ret != 0) break;

  		ret = mbedtls_md_file(mbedtls_md_info_from_string("SM3"), out_path, recoverHash);
  		if (ret != 0) break;

			ret = memcmp(plainHash, recoverHash, 20 * sizeof(unsigned char));

  		printf("#%d SM4 GCM FILE Hash CMP: %d\n", i, ret);

			if (ret != 0) break;
  	}

  	if (ret != 0) {
  		printf("SM4 GCM FILE ENCRYPT ERROR: times = %d, ret = %X\n", i, ret);
  	}

  	return ret;
}
