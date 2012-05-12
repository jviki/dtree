/**
 * dtree_bcd_test.c
 * Copyright (C) 2012 Jan Viktorin
 */

#include "bcd_arith.h"
#include "test.h"
#include <stdlib.h>
#include <string.h>

size_t pow10(size_t c)
{
	size_t r = 10;

	for(size_t i = 1; i < c; ++i)
		r *= 10;

	return r;
}

void print_bcd(size_t i, size_t count, size_t mod, bcd_t number)
{
		if((i > 0 && i % mod == 0))
			printf("\n");
		printf("%s ", bcd_tostr(number));
		if(i + 1 >= count)
			printf("\n");
}

void test_bcd_inc(void)
{
	test_start();
	
	bcd_t number;
	bcd_init(number);
	const size_t count = pow10(sizeof(bcd_t) - 1);
	
	for(size_t i = 0; i < count; ++i) {
		char expect[sizeof(bcd_t)];
		snprintf(expect, sizeof(bcd_t), "%02zu", i);

		print_bcd(i, count, 20, number);

		fail_on_true(strcmp(expect, bcd_tostr(number)), "Doesn't match the expected result");
		bcd_inc(number);
	}

	fflush(stdout);
	test_end();
}

int main(void)
{
	test_bcd_inc();
	return 0;
}
