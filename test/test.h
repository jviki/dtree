#ifndef DTREE_TEST
#define DTREE_TEST

#include <stdio.h>

void _test_fail(const char *func, const char *file, int lineno, const char *desc)
{
	fprintf(stderr, "Test %s (%s:%d) has FAILED\n", func, file, lineno);
	fprintf(stderr, "%s\n", desc);
}
#define test_fail(desc) _test_fail(__func__, __FILE__, __LINE__, (desc))

#endif

