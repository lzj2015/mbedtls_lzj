#include <AisinoSSL/sm3/sm3.h>
#include "utils/testutil.h"
#include <AisinoSSL/internal/openssl_aid.h>


typedef struct {
    size_t size;
    const unsigned char *data;
}  SIZED_DATA;

typedef struct {
    size_t size;
    unsigned char *data;
}  MUTABLE_SIZED_DATA;

#define u8 unsigned char

/**********************************************************************
 *
 * Test of sm3
 * void sms4_cbc_encrypt(const unsigned char *in, unsigned char *out,
    size_t len, const sms4_key_t *key, unsigned char *iv, int enc);
 ***/

static const char *testhex[] = {
    /* 0 "abc" */
    "abc",
    /* 1 "abcd" 16 times */
    "abcdabcdabcdabcdabcdabcdabcdabcd"
    "abcdabcdabcdabcdabcdabcdabcdabcd",
};

unsigned char dgsthex[2][32] = {
    {
        0x66, 0xc7, 0xf0, 0xf4, 0x62, 0xee, 0xed, 0xd9,
        0xd1, 0xf2, 0xd4, 0x6b, 0xdc, 0x10, 0xe4, 0xe2,
        0x41, 0x67, 0xc4, 0x87, 0x5c, 0xf2, 0xf7, 0xa2,
        0x29, 0x7d, 0xa0, 0x2b, 0x8f, 0x4b, 0xa8, 0xe0
    },
    {
        0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1,
        0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d,
        0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65,
        0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32
    }
};

int sm3_test(int verbose)
{
    unsigned char dgst[512];
    unsigned int dgstlen = 32;
    int i;

    for (i = 0; i < 2; i++) {
        sm3_context ctx;
        sm3_init(&ctx);
        sm3_starts(&ctx);
        sm3_update(&ctx, (const unsigned char*)testhex[i], strlen(testhex[i]));
        sm3_finish(&ctx, dgst);

        if (memcmp(dgsthex[i], dgst, dgstlen) != 0) {
            if (verbose)
                printf("error calculating SM3 on %s\n", testhex[i]);
            return 1;
            // printf("got %s instead of %s\n", p, dgsthex[i]);
            // err++;
        } else {
            if (verbose)
                printf("SM3 test %d ok\n", i+1);
        }
    }
    return 0;
}
