#define _BSD_SOURCE // enable dir type constants
#include "dtree.h"
#include "dtree_procfs.h"
#include "dtree_error.h"
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
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

/**
 * Finds the next device and sets sep to point to the '@'
 * separator in the directory name (name@address).
 */
static
struct dirent *find_device(char **sep)
{
	struct dirent *entry = NULL;

	while((entry = readdir(procfs)) != NULL) {
		char *at = strchr(entry->d_name, '@');	

		if(!dirent_is_dir(entry))
			continue;

		// found and next character is a number (address)
		// XXX: do some PATH_MAX checking?
		if(at != NULL && isalnum(at[1])) {
			*sep = at;
			break;
		}
	}

	return entry;
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

struct dtree_dev_t *build_dev(struct dirent *devdir, const char *sep)
{
	static const char *NULL_ENTRY = NULL;

	struct dtree_dev_t *dev = NULL;
	size_t namel = sep - devdir->d_name;
	size_t addrl = strlen(sep + 1);

	void *m = malloc(sizeof(struct dtree_dev_t) + namel + 1);
	if(m == NULL) {
		dtree_error_from_errno();
		return NULL;
	}

	dev = (struct dtree_dev_t *) m;
	dev->compat = &NULL_ENTRY; // TODO: implement
	dev->name = copy_devname((void *) (dev + 1), devdir->d_name, namel);
	dev->base = parse_devaddr(sep + 1, addrl);

	return dev;
}

struct dtree_dev_t *dtree_next(void)
{
	struct dirent *devdir = NULL;
	char *sep = NULL;

	devdir = find_device(&sep);
	if(devdir == NULL) {
		dtree_error_from_errno();
		return NULL;
	}

	return build_dev(devdir, sep);
}

void dtree_dev_free(struct dtree_dev_t *dev)
{
	assert(dev != NULL);

	dev->name = NULL;
	dev->base = 0;
	dev->compat = NULL;

	free(dev);
}

int dtree_reset(void)
{
	assert(procfs != NULL);
	rewinddir(procfs);
	return 0;
}


