#include "dtree.h"
#include "test.h"

void test_first_dev(void)
{
	const int expect = 3;
	struct dtree_dev_t *curr = NULL;
	int count = 0;

	while((curr = dtree_next())) {
		const char *name  = dtree_dev_name(curr);
		dtree_addr_t base = dtree_dev_base(curr);

		printf("DEV '%s' at 0x%08X\n", name, base);

		dtree_dev_free(curr);
		count += 1;
	}

	fail_on_true(dtree_iserror(), "An error occured during traversing the device tree");
	fail_on_true(count < expect, "Some device were not traversed");
	fail_on_true(count > expect, "More devices were traversed then expected");
}

int main(void)
{
	int err = dtree_open("device-tree");
	halt_on_error(err, "Can not open testing device-tree");

	test_first_dev();

	dtree_close();
}

