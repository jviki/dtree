#include "dtree.h"
#include "test.h"

void test_pass_null(void)
{
	int err = dtree_open(NULL);
	fail_on_success(err, "Successful when passing NULL");
	fail_on_false(dtree_iserror(), "Error is not indicated by dtree_iserror()");
}

void test_nonexistent_dir(void)
{
	int err = dtree_open("/xxx/yyy/zzz");
	fail_on_success(err, "Successful when passing non-existent dir: /xxx/yyy/zzz");
	fail_on_false(dtree_iserror(), "Error is not indicated by dtree_iserror()");
}

void test_pass_invalid(void)
{
	int err = dtree_open(__FILE__);
	fail_on_success(err, "Successful when passing " __FILE__ " as root dir");
	fail_on_false(dtree_iserror(), "Error is not indicated by dtree_iserror()");
}

/**
 * Test on the real device-tree file system.
 * It should pass on target platform.
 */
void test_pass_mostly_valid(void)
{
	int err = dtree_open("/proc/device-tree");
	fail_on_error(err, "Open of /proc/device-tree failed, is it present on the system?");
	warn_on_true(dtree_iserror(), "Error is indicated by dtree_iserror(), but should not be");
	dtree_close();
}

/**
 * Gets information from the testing device-tree
 * in current directory.
 */
void test_open_test_dtree(void)
{
	int err = dtree_open("device-tree");
	fail_on_error(err, "Can not open testing device-tree");
	warn_on_true(dtree_iserror(), "Error state is set, but should not be");
	dtree_close();
}

/**
 * Tests whether the error flag is cleared in correct way.
 */
void test_clear_error(void)
{
	int err = dtree_open(NULL);
	fail_on_success(err, "Open of NULL was successful");

	warn_on_false(dtree_iserror(), "No error is indicated");
	dtree_close();

	fail_on_false(dtree_iserror(), "The error was cleared after close");

	err = dtree_open("device-tree");
	fail_on_error(err, "Can not open testing device-tree");
	fail_on_true(dtree_iserror(), "The error was not cleared");
}

int main(void)
{
	test_pass_null();
	test_nonexistent_dir();
	test_pass_invalid();
	test_open_test_dtree();
	test_pass_mostly_valid();
	test_clear_error();
}

