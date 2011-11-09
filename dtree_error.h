/**
 * Internal error handling module.
 * Non-public API.
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 */

#ifndef DTREE_ERROR
#define DTREE_ERROR

#define DTREE_EINVALID_ROOT_DIR 1
#define DTREE_ECANT_READ_ROOT   2

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

/**
 * Tests errno and if it describes and error
 * it sets the error state.
 */
void dtree_error_from_errno(void);

#endif

