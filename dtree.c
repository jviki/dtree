#include "dtree.h"
#include "dtree_error.h"
#include "dtree_procfs.h"

#include <string.h>

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

struct dtree_dev_t *dtree_next(void)
{
	return dtree_procfs_next();
}

void dtree_dev_free(struct dtree_dev_t *dev)
{
	dtree_procfs_dev_free(dev);
}

int dtree_reset(void)
{
	return dtree_procfs_reset();
}

struct dtree_dev_t *dtree_byname(const char *name)
{
	struct dtree_dev_t *curr = NULL;

	if(name == NULL || strlen(name) == 0)
		return NULL;

	while((curr = dtree_next()) != NULL) {
		if(!strcmp(name, curr->name))
			break;

		dtree_dev_free(curr);
	}

	return curr;
}

