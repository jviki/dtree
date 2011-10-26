#include "dtree_procfs.h"
#include "dtree_error.h"
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>

/**
 * Handle device-tree in /proc filesystem.
 */
static DIR *procfs = NULL;

int dtree_procfs_open(const char *rootd)
{
	assert(procfs == NULL);
	
	procfs = opendir(rootd);
	if(procfs == NULL) {
		dtree_error_set(-1);
		return -1;
	}

	return 0;
}

void dtree_procfs_close(void)
{
	assert(procfs != NULL);
	if(closedir(procfs) == -1)
		dtree_error_set(-1);

	procfs = NULL;
}

