/**
 * dtree_procfs_llist_test.c
 * Copyright (C) 2012 Jan Viktorin
 */

#include "dtree_procfs.c" // this is correct, testing internals
#include "test.h"

void test_llist(void)
{
	test_start();

	llist_init();
	fail_on_false(llist_length() == 0, "Empty list has non-zero length");

	struct dtree_entry_t a;
	struct dtree_entry_t b;
	struct dtree_entry_t c;

	llist_append(&a);
	fail_on_false(llist_length() == 1, "After the first append, the length is not one");

	llist_append(&b);
	fail_on_false(llist_length() == 2, "After the second append, the length is not two");

	llist_append(&c);
	fail_on_false(llist_length() == 3, "After the third append, the length is not three");

	fail_on_false(llist_remove() == &c, "The first remove returns different item then 'c'");
	fail_on_false(llist_length() == 2, "After the first remove, the length is not two");

	fail_on_false(llist_remove() == &b, "The second remove returns different item then 'b'");
	fail_on_false(llist_length() == 1, "After the second remove, the length is not one");

	fail_on_false(llist_remove() == &a, "The third remove returns different item then 'a'");
	fail_on_false(llist_length() == 0, "After the third remove, the length is not zero");

	fail_on_false(llist_remove() == NULL, "The fourth remove doesn't return NULL");
	fail_on_false(llist_length() == 0, "After the fourth remove, the length is not zero");

	test_end();
}

int main(void)
{
	test_llist();
	return 0;
}
