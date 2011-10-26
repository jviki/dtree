#include "dtree.h"
#include "dtree_error.h"
#include "dtree_procfs.h"

int dtree_open(const char *rootd)
{
	int err = dtree_procfs_open(rootd);

	if(err == 0) {
		dtree_error_clear();
		return 0;
	}

	return err;
}

void dtree_close(void)
{
	dtree_procfs_close();
}

