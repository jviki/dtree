#include "dtree.h"
#include "dtree_error.h"

#include <errno.h>
#include <string.h>
#include <assert.h>

/**
 * Error state.
 */
static int error = 0;

/**
 * Holds errno. It is valid when error
 * is less then zero.
 */
static int xerrno = 0;

#define ERRSTR_COUNT ((int) (sizeof(errstr)/sizeof(char *)))
static const char *errstr[] = {
	[0]                       = "Successful",
	[DTREE_EINVALID_ROOT_DIR] = "Root dir is invalid"
};

void dtree_error_clear(void)
{
	error  = 0;
	xerrno = 0;
}

void dtree_error_set(int e)
{
	assert(e != 0);

	error  = e;
	xerrno = errno;
}

void dtree_errno_set(int e)
{
	error  = -1;
	xerrno = e;
}

void dtree_error_from_errno(void)
{
	if(errno != 0)
		dtree_errno_set(errno);
}

int dtree_iserror(void)
{
	return error != 0;
}

const char *dtree_errstr(void)
{
	if(error >= 0 && error < ERRSTR_COUNT)
		return errstr[error];

	if(error < 0)
		return strerror(xerrno);

	return "Unknown error occured";
}

