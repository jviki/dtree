/**
 * Internal error handling module.
 * Non-public API.
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 */

#ifndef DTREE_ERROR
#define DTREE_ERROR

/**
 * Clears current error state.
 */
void dtree_error_clear(void);

/**
 * Sets error state. When negative it assumes that
 * errno is set. When positive it assumes a valid
 * internal error code.
 * Passing zero is an error.
 */
void dtree_error_set(int e);

/**
 * Sets error state to negative and internal errno
 * to the given value.
 */
void dtree_errno_set(int e);

#endif

