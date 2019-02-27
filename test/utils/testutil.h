#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/*
 * Equality test for memory blocks where NULL is a legitimate value.
 * These calls return 1 if the two memory blocks compare true.
 * Otherwise, they return 0 and pretty-print diagnostics.
 * These should not be called directly, use the TEST_xxx macros below instead.
 */
int test_mem_eq(const char *, int, const char *, const char *,
                const void *, size_t, const void *, size_t);
int test_mem_ne(const char *, int, const char *, const char *,
                const void *, size_t, const void *, size_t);

/*
 * Check a boolean result for being true or false.
 * They return 1 if the condition is true (i.e. the value is non-zero).
 * Otherwise, they return 0 and pretty-prints diagnostics using |s|.
 * These should not be called directly, use the TEST_xxx macros below instead.
 */
int test_true(const char *file, int line, const char *s, int b);
int test_false(const char *file, int line, const char *s, int b);

# define TEST_mem_eq(a, m, b, n) test_mem_eq(__FILE__, __LINE__, #a, #b, a, m, b, n)
# define TEST_mem_ne(a, m, b, n) test_mem_ne(__FILE__, __LINE__, #a, #b, a, m, b, n)

# define TEST_true(a)         test_true(__FILE__, __LINE__, #a, (a) != 0)
# define TEST_false(a)        test_false(__FILE__, __LINE__, #a, (a) != 0)

#define TEST_TRUE_N(FUNC, N, VERBOSE, ERR)  \
{                             \
    bool _pass = true;             \
    for (int _i=0; _i<N; _i++)  { \
        if (!FUNC(_i)) {          \
            if (VERBOSE) {           \
                printf(#FUNC"\t%d:\tPASS\n", _i);  \
            } \
        } else  {                                \
            if (VERBOSE)   {         \
                printf(#FUNC"\t%d:\tNOT PASS\n", _i);  \
            }\
            _pass = false; \
            ERR += 1; \
            break;    \
        } \
    } \
    if (_pass) \
        printf(#FUNC" PASS\n");         \
    else \
        printf(#FUNC" NOT PASS\n");         \
}


#endif //TEST_UTIL_H
