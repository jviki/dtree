/**
 * Library that provides simple BCD arithmetic
 * Non-public API.
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 */

#ifndef BCD_ARITH
#define BCD_ARITH

/**
 * Represents the BCD number.
 */
typedef char bcd_t[3];

/**
 * Initializes the value to zero.
 */
void bcd_init(bcd_t n);

/**
 * Increments the value by one.
 * Returns non-zero when an overflow occured.
 */
int bcd_inc(bcd_t n);

/**
 * Determines whether the given value
 * is zero.
 */
int bcd_iszero(bcd_t n);

/**
 * Converts the BCD value to zstring.
 */
const char *bcd_tostr(bcd_t n);

#endif
