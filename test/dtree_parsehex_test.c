#include "dtree_util.h"
#include "test.h"
#include <string.h>

void test_hex2num_digit(void)
{
	test_start();
	uint8_t v;
	int error = 0;

	v = hex2num('0', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 0, "'0' could not be parsed");

	v = hex2num('1', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 1, "'1' could not be parsed");

	v = hex2num('2', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 2, "'2' could not be parsed");

	v = hex2num('3', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 3, "'3' could not be parsed");

	v = hex2num('4', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 4, "'4' could not be parsed");

	v = hex2num('5', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 5, "'5' could not be parsed");

	v = hex2num('6', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 6, "'6' could not be parsed");

	v = hex2num('7', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 7, "'7' could not be parsed");

	v = hex2num('8', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 8, "'8' could not be parsed");

	v = hex2num('9', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 9, "'9' could not be parsed");

	test_end();
}

void test_hex2num_alpha_lower(void)
{
	test_start();
	uint8_t v;
	int error = 0;

	v = hex2num('a', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 10, "'a' could not be parsed");

	v = hex2num('b', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 11, "'b' could not be parsed");

	v = hex2num('c', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 12, "'c' could not be parsed");

	v = hex2num('d', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 13, "'d' could not be parsed");

	v = hex2num('e', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 14, "'e' could not be parsed");

	v = hex2num('f', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 15, "'f' could not be parsed");

	test_end();
}

void test_hex2num_alpha_upper()
{
	test_start();
	uint8_t v;
	int error = 0;

	v = hex2num('A', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 10, "'A' could not be parsed");

	v = hex2num('B', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 11, "'B' could not be parsed");

	v = hex2num('C', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 12, "'C' could not be parsed");

	v = hex2num('D', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 13, "'D' could not be parsed");

	v = hex2num('E', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 14, "'E' could not be parsed");

	v = hex2num('F', &error);
	warn_on_true(error, "Error is set but should not be");
	fail_on_false(v == 15, "'F' could not be parsed");

	test_end();
}

#define assert_parsehex(value, variable)               \
	variable = parse_hex(#value, strlen(#value));  \
	fprintf(stderr, "Result: 0x%08X\n", variable); \
	fail_on_false(variable == value, "Can not parse " #value);

#define assert_parsehex_0x(value, variable)            \
	variable = parse_hex(#value, strlen(#value));  \
	fprintf(stderr, "Result: 0x%08X\n", variable); \
	fail_on_false(variable == 0x##value, "Can not parse " #value);

void test_parsehex_valid(void)
{
	test_start();
	uint32_t v;

	assert_parsehex(0x00000000, v);
	assert_parsehex(0x11111111, v);
	assert_parsehex(0x22222222, v);
	assert_parsehex(0x33333333, v);
	assert_parsehex(0x44444444, v);
	assert_parsehex(0x55555555, v);
	assert_parsehex(0x66666666, v);
	assert_parsehex(0x77777777, v);
	assert_parsehex(0x88888888, v);
	assert_parsehex(0x99999999, v);
	assert_parsehex(0xAAAAAAAA, v);
	assert_parsehex(0xBBBBBBBB, v);
	assert_parsehex(0xCCCCCCCC, v);
	assert_parsehex(0xDDDDDDDD, v);
	assert_parsehex(0xEEEEEEEE, v);
	assert_parsehex(0xFFFFFFFF, v);

	assert_parsehex_0x(00000000, v);
	assert_parsehex_0x(11111111, v);
	assert_parsehex_0x(22222222, v);
	assert_parsehex_0x(33333333, v);
	assert_parsehex_0x(44444444, v);
	assert_parsehex_0x(55555555, v);
	assert_parsehex_0x(66666666, v);
	assert_parsehex_0x(77777777, v);
	assert_parsehex_0x(88888888, v);
	assert_parsehex_0x(99999999, v);
	assert_parsehex_0x(AAAAAAAA, v);
	assert_parsehex_0x(BBBBBBBB, v);
	assert_parsehex_0x(CCCCCCCC, v);
	assert_parsehex_0x(DDDDDDDD, v);
	assert_parsehex_0x(EEEEEEEE, v);
	assert_parsehex_0x(FFFFFFFF, v);

	assert_parsehex(0x0, v);
	assert_parsehex(0x4, v);
	assert_parsehex(0x8, v);
	assert_parsehex(0xC, v);

	test_end();
}

#define assert_fail_parsehex(value, variable)   \
	variable = parse_hex(#value, strlen(#value));   \
	fprintf(stderr, "Result: 0x%08X = %u\n", variable, variable); \
	fail_on_false(variable == 0, "Parsing of " #value " leads to different result than 0");

void test_parsehex_invalid(void)
{
	test_start();
	uint32_t v;

	assert_fail_parsehex("0kFFFFFFFF", v);
	assert_fail_parsehex("xEEEEEEEE", v);
	assert_fail_parsehex("12345xABC", v);
	assert_fail_parsehex("alphabet", v);

	test_end();
}

int main(void)
{
	test_hex2num_digit();
	test_hex2num_alpha_lower();
	test_hex2num_alpha_upper();
	test_parsehex_valid();
	test_parsehex_invalid();
}
