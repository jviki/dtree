#include "dtree.h"
#include <errno.h>
#include <string.h>

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
	"Successful"
};

void dtree_error_clear(void)
{
	error  = 0;
	xerrno = 0;
}

void dtree_error_set(int e)
{
	error  = e;
	xerrno = errno;
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
		return strerror(errno);

	return "Unknown error occured";
}

