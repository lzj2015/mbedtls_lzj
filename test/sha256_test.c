#include <AisinoSSL/mbedtls/sha256.h>
#include "utils/testutil.h"


int sha256_test(int verbose) {
    return mbedtls_sha256_self_test(verbose);
}
