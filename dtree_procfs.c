#define _BSD_SOURCE // enable dir type constants

#include "dtree.h"
#include "dtree_procfs.h"
#include "dtree_error.h"

#include <libgen.h> // basename, dirname
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <ftw.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//
// Linked-list implementation
//

/**
 * Entry of dtree_procfs linked-list.
 */
struct dtree_entry_t {
	struct dtree_dev_t dev;
	struct dtree_entry_t *next;
};

/**
 * Linked-list that holds all the devices.
 */
static struct dtree_entry_t *top = NULL;

static
void llist_append(struct dtree_entry_t *e)
{
	e->next = top;
	top = e;	
}

static
struct dtree_entry_t *llist_remove(void)
{
	struct dtree_entry_t *e = top;

	if(top != NULL)
		top = top->next;

	if(e != NULL)
		e->next = NULL;

	return e;
}

static
struct dtree_entry_t *llist_next(struct dtree_entry_t *e)
{
	return e->next;
}

static
struct dtree_entry_t *llist_last(void)
{
	return top;
}


//
// Walking over the procfs
//

static
const char *copy_devname(void *name, const char *d_name, size_t namel)
{
	memcpy(name, d_name, namel);

	char *p = (char *) name;
	p[namel] = '\0';

	return (const char *) p;
}

static
dtree_addr_t parse_devaddr(const char *addr)
{
	long val = strtol(addr, NULL, 16);
	return (dtree_addr_t) val;
}

static
struct dtree_entry_t *build_entry(const char *name, size_t namel, const char *base)
{
	static const char *NULL_ENTRY = NULL;
	struct dtree_entry_t *entry = NULL;

	void *m = malloc(sizeof(struct dtree_entry_t) + namel + 1);
	if(m == NULL)
		return NULL;

	entry = (struct dtree_entry_t *) m;
	entry->dev.name = copy_devname((void *) (entry + 1), name, namel);
	entry->dev.base = parse_devaddr(base);
	entry->dev.compat = &NULL_ENTRY;

	return entry;
}

static
int dtree_walk_dir(const char *path)
{
	const char *bname = basename((char *) path); // XXX: be careful of "/"
	const char *at = strchr(bname, '@');

	// not found or next character is not of address
	if(at == NULL || !isalnum(at[1]))
		return 0; // skip non device directory

	size_t namel = at - bname;
	const char  *name  = bname;
	const char  *base  = at + 1;

	struct dtree_entry_t *dev = build_entry(name, namel, base);
	if(dev == NULL)
		return 1;

	llist_append(dev);
	return 0;
}

static
int dtree_walk_file(struct dtree_entry_t *e, const char *path, const struct stat *s)
{
	static const char *NULL_ENTRY = NULL;
	const char *bname = basename((char *) path); // XXX: be careful of "/"

	if(strcmp("compatible", bname))
		return 0;

	size_t fsize = s->st_size;
	// read the file contents and process
	e->dev.compat = &NULL_ENTRY;
	return 0;
}

static
int dtree_walk(const char *fpath, const struct stat *sb, int typeflag)
{
	struct dtree_entry_t *last = llist_last();

	if(last != NULL && typeflag == FTW_F)
		return dtree_walk_file(last, fpath, sb);
	
	else if(typeflag == FTW_D)
		return dtree_walk_dir(fpath);
	
	// ignoring the top level files
	return 0;
}

//
// Initialization & destruction
//

#define DTREE_PROCFS_MAX_LEVEL 4

int dtree_procfs_open(const char *rootd)
{
	if(rootd == NULL) {
		dtree_errno_set(EINVAL);
		return -1;
	}

	int err = ftw(rootd, &dtree_walk, DTREE_PROCFS_MAX_LEVEL);

	if(err != 0) {
		dtree_error_from_errno(); // XXX: does ftw use errno?
		return err;
	}

	return 0;
}

void dtree_procfs_close(void)
{
	struct dtree_entry_t *curr = NULL;

	while((curr = llist_remove()) != NULL) {
		curr->dev.name = NULL;
		curr->dev.base = 0;
		curr->dev.compat = NULL;
		free(curr);
	}
}


//
// Iteration over entries
//

struct dtree_dev_t *dtree_procfs_next(void)
{
	return NULL;
}

void dtree_procfs_dev_free(struct dtree_dev_t *dev)
{
}

int dtree_procfs_reset(void)
{
	return 0;
}

