#ifndef DTREE_UTIL_H
#define DTREE_UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

static inline
uint8_t hex2num(char c, int *err)
{
	*err = 0;
	if(isdigit(c))
		return c - '0';
	if(isalpha(c))
		return tolower(c) - 'a' + 10;

	*err = 1;
	return 0;
}

static inline
uint32_t parse_hex(const char *s, size_t slen)
{
	uint32_t val = 0;
	uint32_t order = 1;
	int error = 0;
	int32_t i = (int32_t) slen - 1; // possible overflow, assuming only few (8) characters

	for(; i >= 0; --i) {
		uint32_t num = hex2num(s[i], &error);
		if(error == 1)
			break;

		val += num * order;
		order *= 16;
	}

	return val;
}

#endif
