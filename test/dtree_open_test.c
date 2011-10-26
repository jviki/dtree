#include "dtree.h"
#include "test.h"

void test_pass_null(void)
{
	int err = dtree_open(NULL);
	if(err == 0)
		test_fail("Successful when passing NULL");
	if(!dtree_iserror())
		test_fail("Error is not indicated by dtree_iserror()");
}

void test_nonexistent_dir(void)
{
	int err = dtree_open("/xxx/yyy/zzz");
	if(err == 0)
		test_fail("Successful when passing non-existent dir: /xxx/yyy/zzz");
	if(!dtree_iserror())
		test_fail("Error is not indicated by dtree_iserror()");
}

void test_pass_invalid(void)
{
	int err = dtree_open(__FILE__);
	if(err == 0)
		test_fail("Successful when passing " __FILE__ " as root dir");
	if(!dtree_iserror())
		test_fail("Error is not indicated by dtree_iserror()");
}

/**
 * Test on the real device-tree file system.
 * It should pass on target platform.
 */
void test_pass_mostly_valid(void)
{
	int err = dtree_open("/proc/device-tree");
	if(err != 0) {
		test_fail("Open of /proc/device-tree failed, is it present on the system?");
		return;
	}
	if(dtree_iserror())
		test_fail("Error is indicated by dtree_iserror(), but should not be");

	dtree_close();
}

int main(void)
{
	test_pass_null();
	test_nonexistent_dir();
	test_pass_invalid();
	test_pass_mostly_valid();
}

