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

	dtree_procfs_close();
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

static
int is_compatible(const struct dtree_dev_t *dev, const char *compat)
{
	const char **dev_compat = dtree_dev_compat(dev);

	for(int i = 0; dev_compat[i] != NULL; ++i) {
		if(!strcmp(dev_compat[i], compat))
			return 1;
	}

	return 0;
}

struct dtree_dev_t *dtree_bycompat(const char *compat)
{
	struct dtree_dev_t *curr = NULL;

	if(compat == NULL || strlen(compat) == 0)
		return NULL;

	while((curr = dtree_next()) != NULL) {
		if(is_compatible(curr, compat))
			break;

		dtree_dev_free(curr);
	}

	return curr;
}
