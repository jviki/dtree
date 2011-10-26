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
int dtree_walk_file(struct dtree_entry_t *e, const char *path, const struct stat *s)
{
}

static
int dtree_walk_dir(const char *path, const struct stat *s)
{
}

static
int dtree_walk(const char *fpath, const struct stat *sb, int typeflag)
{
	struct dtree_entry_t *last = llist_last();

	if(last != NULL && typeflag == FTW_F) {
		return dtree_walk_file(last, fpath, sb);
	}
	else if(typeflag == FTW_D) {
		return dtree_walk_dir(path, sb);
	}
	
	// ignoring the top level files

	return 0;
}

//
// Initialization & destruction
//

int dtree_procfs_open(const char *rootd)
{
}



