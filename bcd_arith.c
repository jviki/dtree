#include "bcd_arith.h"
#include <string.h>
#include <assert.h>

#define BCD_ZERO "00"

void bcd_init(bcd_t n)
{
	// copy all the zstring including the '\0'
	memcpy((void *) n, (void *) BCD_ZERO, strlen(BCD_ZERO) + 1);
	assert(n[strlen(BCD_ZERO)] == '\0');
}

/**
 * Uses decadic digits.
 */
static
int bcd_dec_inc(bcd_t n)
{
	size_t len = strlen(BCD_ZERO);

	for(size_t i = len; i > 0; --i) {
		if(n[i - 1] == '9') {
			n[i - 1] = '0';
		}
		else {
			n[i - 1] += 1;
			break;
		}
	}

	return bcd_iszero(n);
}

int bcd_inc(bcd_t n)
{
	return bcd_dec_inc(n);
}

int bcd_iszero(bcd_t n)
{
	return !memcmp(n, BCD_ZERO, strlen(BCD_ZERO));
}

const char *bcd_tostr(bcd_t n)
{
	return (const char *) n;
}
