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
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
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

/**
 * Iterator over the linked-list.
 */
static struct dtree_entry_t *iter = NULL;

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
	if(e == NULL)
		return NULL;

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

#define SYSERR_OCCURED -2

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

/**
 * Reads file on the given path to the given buffer of length len.
 * Returns 0 when successful. On error sets dtree error state.
 */
static
int read_file(const char *path, char *buff, const size_t len)
{
	int fd = open(path, O_RDONLY);
	if(fd == -1) { // permissions!! or a race condition??
		dtree_error_from_errno();
		return -1;
	}

	assert(len <= SSIZE_MAX); // not supporting big files for now

	ssize_t rlen = 0;
	size_t total = 0;
	int err = 0;

	do {
		rlen = read(fd, buff + total, len - total);
		
		if(rlen == -1) {// not handling EINTR
			err = -1;
			dtree_error_from_errno();
			break;
		}

		total += (size_t) rlen;
	} while(total < len);

	close(fd);
	return err;
}

/**
 * Counts number of zero-terminated strings in the buffer.
 * If the last string doesn't ends with zero, it is not counted.
 */
static
size_t strings_count(char *buff, size_t len)
{
	char *p = NULL;
	size_t entries = 0;

	for(p = buff; (size_t) (p - buff) < len; ++p) {
		if(*p == '\0')
			entries += 1;
	}
	
	return entries;
}

/**
 * Assigns pointers of every single zero-terminated string
 * into the sarray. The last item points to NULL.
 * Assumes that the sarray has enought space.
 */
static
void strings_parse(char *buff, size_t len, char **sarray)
{
	char *p = buff;
	size_t i = 0;

	sarray[0] = p;
	for(i = 1; (size_t) (p - buff) < len; ++p) {
		if(*p == '\0')
			sarray[i] = p + 1;
	}

	sarray[i] = NULL;
}

/**
 * Parses the buffer buff with compat file contents.
 * Assigns the strings pointers to the given dtree entry last.
 */
static
int parse_compat(struct dtree_entry_t *e, char *buff, size_t fsize)
{
	// Don't mind when str_count is 0 here.
	// When trying to handle it, there must be
	// one free(buff) in this function or
	// some unreadable way to inform the caller
	// to call free on better place.
	size_t str_count = strings_count(buff, fsize);

	// alloc pointers to strings and last item for NULL
	void *m = malloc((1 + str_count) * sizeof(char *));
	if(m == NULL) {
		dtree_error_from_errno();
		return SYSERR_OCCURED;
	}

	char **sarray = (char **) m;
	strings_parse(buff, fsize, sarray);
	assert(sarray[str_count] == NULL);

	e->dev.compat = (const char **) sarray; // needs 2 free's! see read_compat_file()
	return 0;
}

/**
 * Reads the compat file to memory.
 * Finds all strings in its contents and
 * allocates pointers to point to each of them.
 *
 * Careful when deallocating. There two blocks
 * to be free'd.
 *
 * compat: | s0 | s1 | s2 |
 * sarray: | ^  | ^  | ^  | NULL |
 *
 * Deallocation sequence (pointer to compat is not held):
 *  free(sarray[0]);
 *  free(sarray);
 */
static
int read_compat_file(struct dtree_entry_t *e, const char *path, size_t fsize)
{
	void *m = malloc(fsize + 1); // 1 byte for missing ZERO (if necessary)
	if(m == NULL) {
		dtree_error_from_errno();
		return SYSERR_OCCURED;
	}

	char *buff = (char *) m;
	int read_err = read_file(path, buff, fsize);
	if(read_err != 0) {
		free(m);
		return read_err;
	}

	int compat_err = 0;
	if(buff[fsize - 1] == '\0') {
		compat_err = parse_compat(e, buff, fsize);
	}
	else {
		buff[fsize] = '\0'; // use the reserved byte
		compat_err = parse_compat(e, buff, fsize + 1);
	}

	if(compat_err != 0) // error is already handled in parse_compat()
		free(m);

	return compat_err;
}

static
int dtree_walk_file(struct dtree_entry_t *e, const char *path, const struct stat *s)
{
	static const char *NULL_ENTRY = NULL;
	const char *bname = basename((char *) path); // XXX: be careful of "/"

	if(strcmp("compatible", bname))
		return 0;

	size_t fsize = s->st_size;
	if(fsize == 0)
		return 0;

	return read_compat_file(e, path, fsize);
}

static
int dtree_walk(const char *fpath, const struct stat *sb, int typeflag)
{
	struct dtree_entry_t *last = llist_last();

	if(last != NULL && typeflag == FTW_F)
		return dtree_walk_file(last, fpath, sb);
	
	else if(typeflag == FTW_D)
		return dtree_walk_dir(fpath);

	else // root is a file!
		return DTREE_EINVALID_ROOT_DIR;

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

	if(err == -1) {
		dtree_error_from_errno(); // XXX: does ftw use errno?
		return err;
	}
	else if(err != 0) { // error generated by module
		dtree_error_set(err);
		return err;
	}

	dtree_reset();
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
	struct dtree_entry_t *entry = iter;
	iter = llist_next(entry);

	return &entry->dev;
}

void dtree_procfs_dev_free(struct dtree_dev_t *dev)
{
	// empty, nothing to free in this implementation
}

int dtree_procfs_reset(void)
{
	iter = llist_last();
	return 0;
}

