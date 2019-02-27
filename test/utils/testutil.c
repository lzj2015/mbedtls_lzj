
#include "testutil.h"
#include <stdio.h>


int test_true(const char *file, int line, const char *s, int b)
{
    if (b)
        return 1;
    printf("<%s,\t%d>:\tTest value fail\n", file, line);
    // test_fail_message(NULL, file, line, "bool", s, "true", "==", "false");
    return 0;
}

int test_false(const char *file, int line, const char *s, int b)
{
    if (!b)
        return 1;
    printf("<%s,\t%d>:\tTest value fail\n", file, line);
    // test_fail_message(NULL, file, line, "bool", s, "false", "==", "true");
    return 0;
}

int test_mem_eq(const char *file, int line, const char *st1, const char *st2,
                const void *s1, size_t n1, const void *s2, size_t n2)
{
    if (s1 == NULL && s2 == NULL)
        return 1;
    if (n1 != n2 || s1 == NULL || s2 == NULL || memcmp(s1, s2, n1) != 0) {
        // test_fail_memory_message(NULL, file, line, "memory", st1, st2, "==",
        //                          s1, n1, s2, n2);
	    printf("<%s,\t%d>:\ttest_fail_memory_message\n", file, line);
        return 0;
    }
    return 1;
}

int test_mem_ne(const char *file, int line, const char *st1, const char *st2,
                const void *s1, size_t n1, const void *s2, size_t n2)
{
    if ((s1 == NULL) ^ (s2 == NULL))
        return 1;
    if (n1 != n2)
        return 1;
    if (s1 == NULL || memcmp(s1, s2, n1) == 0) {
	    printf("<%s,\t%d>:\ttest_fail_memory_message\n", file, line);
        // test_fail_memory_message(NULL, file, line, "memory", st1, st2, "!=",
        //                          s1, n1, s2, n2);
        return 0;
    }
    return 1;
}