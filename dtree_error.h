/**
 * Internal error handling module.
 * Non-public API.
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 */

#ifndef DTREE_ERROR
#define DTREE_ERROR

void dtree_error_clear(void);

/**
 * Sets error state. When negative it assumes that
 * errno is set. When positive it assumes a valid
 * internal error code.
 * Passing zero is an error.
 */
void dtree_error_set(int e);

#endif

