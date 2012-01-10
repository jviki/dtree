#include "dtree.h"
#include "test.h"

void test_list_all(void)
{
	test_start();
	struct dtree_dev_t *dev = NULL;

	while((dev = dtree_next()) != NULL) {
		const char  *name = dtree_dev_name(dev);
		dtree_addr_t base = dtree_dev_base(dev);

		printf("DEV '%s' at 0x%08X\n", name, base);
		print_compat(dev);
		dtree_dev_free(dev);
	}

	test_end();
}

void test_find_existent(void)
{
	test_start();
	
	struct dtree_dev_t *dev = NULL;
	dev = dtree_byname("ethernet");
	fail_on_true(dev == NULL, "Could not find the device 'ethernet'");

	const char  *name = dtree_dev_name(dev);
	dtree_addr_t base = dtree_dev_base(dev);

	printf("DEV '%s' at 0x%08X\n", name, base);
	dtree_dev_free(dev);

	test_end();
}

void test_find_non_existent(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_byname("@not-implemented-device");
	fail_on_true(dev != NULL, "Device '@not-implemented-device' was found!");

	test_end();
}

void test_find_null(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_byname(NULL);
	fail_on_false(dev == NULL, "Device NULL was found!");

	test_end();
}

void test_find_empty(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_byname("");
	fail_on_false(dev == NULL, "Device '' was found!");

	test_end();
}

int main(void)
{
	int err = dtree_open("device-tree");
	halt_on_error(err, "Can not open testing device-tree");

	test_list_all();
	dtree_reset();

	test_find_existent();
	dtree_reset();

	test_find_non_existent();
	dtree_reset();

	test_find_null();
	dtree_reset();

	test_find_empty();
	dtree_reset();

	dtree_close();
}

