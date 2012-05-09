#include "dtree.h"
#include "test.h"

void test_find_existent(void)
{
	test_start();
	
	struct dtree_dev_t *dev = NULL;
	int count = 0;

	while((dev = dtree_bycompat("simple-bus")) != NULL) {
		count += 1;

		const char  *name = dtree_dev_name(dev);
		dtree_addr_t base = dtree_dev_base(dev);

		printf("DEV '%s' at 0x%08X\n", name, base);
		dtree_dev_free(dev);
	}

	fail_on_true(count == 0, "Could not find any device compatible with 'simple-bus'");

	test_end();
}

void test_find_non_existent(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_bycompat("@not-implemented-device");
	fail_on_true(dev != NULL, "Device '@not-implemented-device' was found!");

	test_end();
}

void test_find_null(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_bycompat(NULL);
	fail_on_false(dev == NULL, "Device NULL was found!");

	test_end();
}

void test_find_empty(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	dev = dtree_bycompat("");
	fail_on_false(dev == NULL, "Device '' was found!");

	test_end();
}

void test_find_serial_1_00_a(void)
{
	test_start();

	struct dtree_dev_t *dev = NULL;
	int count = 0;

	while((dev = dtree_bycompat("xlnx,xps-uartlite-1.00.a")) != NULL) {
		count += 1;

		const char  *name = dtree_dev_name(dev);
		dtree_addr_t base = dtree_dev_base(dev);
		dtree_addr_t high = dtree_dev_high(dev);

		printf("DEV '%s' at 0x%08X .. 0x%08X\n", name, base, high);
		dtree_dev_free(dev);
	}

	fail_on_true(count != 2, "Expected two xlnx,xps-uartlite-1.00.a compatible components");

	test_end();
}

int main(void)
{
	int err = dtree_open("device-tree");
	halt_on_error(err, "Can not open testing device-tree");

	test_find_existent();
	dtree_reset();

	test_find_non_existent();
	dtree_reset();

	test_find_null();
	dtree_reset();

	test_find_empty();
	dtree_reset();

	test_find_serial_1_00_a();
	dtree_reset();

	dtree_close();
}

