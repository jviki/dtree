#ifndef DTREE_TEST
#define DTREE_TEST

#include "dtree.h"
#include <stdio.h>
#include <stdlib.h>

void _test_start(const char *func, const char *file, int lineno)
{
	fprintf(stderr, "Running '%s' (%s:%d)\n", func, file, lineno);
}
#define test_start() _test_start(__func__, __FILE__, __LINE__)

void _test_end(const char *func)
{
	fprintf(stderr, "SUCCESS: '%s'\n", func);
}
#define test_end() _test_end(__func__)

void _test_fail(const char *func, const char *file, int lineno, const char *desc)
{
	fprintf(stderr, "Test %s (%s:%d) has FAILED\n", func, file, lineno);

	if(dtree_iserror())
		fprintf(stderr, "ERROR: %s [%s]\n", desc, dtree_errstr());
	else
		fprintf(stderr, "ERROR: %s\n", desc);
}
#define test_fail(desc) _test_fail(__func__, __FILE__, __LINE__, (desc))

void _test_warn(const char *func, const char *file, int lineno, const char *desc)
{
	fprintf(stderr, "Warning in %s (%s:%d)\n", func, file, lineno);
	fprintf(stderr, "%s\n", desc);
}
#define test_warn(desc) _test_warn(__func__, __FILE__, __LINE__, (desc))

#define halt_on_true(t, msg) \
	if((t) != 0) {test_fail((msg)); exit(1);}
#define halt_on_false(f, msg)     halt_on_true(!(f), (msg))
#define halt_on_success(err, msg) halt_on_true((err) == 0, (msg))
#define halt_on_error(t, msg)     halt_on_true((err) != 0, (msg))

#define fail_on_true(t, msg) \
	if((t) != 0) {test_fail((msg)); return;}

#define fail_on_false(f, msg)     fail_on_true(!(f), (msg))
#define fail_on_success(err, msg) fail_on_true((err) == 0, (msg))
#define fail_on_error(t, msg)     fail_on_true((err) != 0, (msg))

#define warn_on_true(t, msg) \
	if((t) != 0) {test_warn((msg));}
#define warn_on_false(f, msg)     warn_on_true(!(f), (msg))

static inline
void print_compat(struct dtree_dev_t *dev)
{
	const char **compat = dtree_dev_compat(dev);

	for(size_t i = 0; compat[i] != NULL; ++i) {
		const char *sep = ", ";
		if(compat[i + 1] == NULL)
			sep = "\n";

		printf("%s%s", compat[i], sep);
	}
}

#endif

