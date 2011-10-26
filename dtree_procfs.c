#define _BSD_SOURCE // enable dir type constants
#include "dtree_procfs.h"
#include "dtree_error.h"
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>

/**
 * Handle device-tree in /proc filesystem.
 */
static DIR *procfs = NULL;

int dtree_procfs_open(const char *rootd)
{
	assert(procfs == NULL);

	if(rootd == NULL) {
		dtree_errno_set(EINVAL);
		return -1;
	}
	
	procfs = opendir(rootd);
	if(procfs == NULL) {
		dtree_error_from_errno();
		return -1;
	}

	return 0;
}

void dtree_procfs_close(void)
{
	assert(procfs != NULL);
	if(closedir(procfs) == -1)
		dtree_error_from_errno();

	procfs = NULL;
}

static
int dirent_is_dir(struct dirent *ent)
{
	if(ent->d_type == DT_DIR)
		return 1;

	if(ent->d_type != DT_UNKNOWN) // see man readdir
		return 0;

	// when unsupported by filesystem, implement it here
	return 0;
}
const char *copy_devname(void *name, char *d_name, size_t namel)
{
	memcpy(name, d_name, namel);

	char *p = (char *) name;
	p[namel] = '\0';

	return (const char *) p;
}

dtree_addr_t parse_devaddr(const char *addr, size_t addrl)
{
	assert(addr[addrl] == '\0');
	long val = strtol(addr, NULL, 16);
	return (dtree_addr_t) val;
}

