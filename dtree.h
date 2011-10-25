/**
 * Access to device-tree in embedded Linux.
 * Public API.
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 *
 * The main principle if to be able to iterate
 * over all devices or search among them.
 *
 * The library is not reentrant nor thread safe.
 */

#ifndef DTREE_H
#define DTREE_H

#include <stdint.h>

//
// Module initialization & destruction
//

/**
 * Opens device tree using the given root directory
 * (typically /proc/device-tree). Setups internal
 * structures. Clears error state.
 *
 * It is safe to call dtree_reset() after dtree_open()
 * but it has no effect.
 *
 * Returns 0 on success. On error sets error state.
 */
int dtree_open(const char *rootd);

/**
 * Free's resources of the module.
 * It is an error to call it when dtree_open()
 * has failed or to call it twice.
 */
void dtree_close(void);


//
// Data types
//

/**
 * Representation of address.
 */
typedef uint32_t dtree_addr_t;

/**
 * Device info representation.
 *
 * Consists of the name, address and array of
 * compatible devices. Last pointer in compat is NULL.
 */
struct dtree_dev_t {
	const char  *name;
	dtree_addr_t base;
	const char  *compat[];
};

#define DTREE_GETTER static inline

/**
 * Get name of the device.
 */
DTREE_GETTER
const char *dtree_dev_name(struct dtree_dev_t *d)
{
	return d->name;
}

/**
 * Get base address of the device.
 */
DTREE_GETTER
dtree_addr_t dtree_dev_base(struct dtree_dev_t *d)
{
	return d->base;
}

/**
 * Get the list of compatible devices. Last entry
 * points to NULL.
 */
DTREE_GETTER
const char **dtree_dev_compat(struct dtree_dev_t *d)
{
	return d->compat;
}


//
// Iteration routines
//

/**
 * Returns next available device entry.
 * The entry should be free'd by dtree_dev_free().
 *
 * Uses shared internal iterator.
 * To search from beginning call dtree_reset().
 *
 * When no more entries are available or and error occoures
 * returns NULL. On error sets error state.
 */
struct dtree_dev_t *dtree_next(void);

/**
 * Look up for device by name. Returns the first occurence
 * of device with the given name.
 * The entry should be free'd by dtree_dev_free().
 *
 * Uses shared internal iterator.
 * To search from beginning call dtree_reset().
 *
 * Returns NULL when not found or on error.
 * On error sets error state.
 */
struct dtree_dev_t *dtree_byname(const char *name);

/**
 * Looks up for device compatible with the given type.
 * The entry should be free'd by dtree_dev_free().
 *
 * Uses shared internal iterator.
 * To search from beginning call dtree_reset().
 *
 * Returns NULL when not found or on error.
 * On error sets error state.
 */
struct dtree_dev_t *dtree_bycompat(const char *compat);

/**
 * Resets the iteration over devices.
 * Eg. after this call dtree_next() will return the first
 * device again. Affects the internal shared iterator.
 * 
 * Returns 0 on success. On error sets error state.
 */
int dtree_reset(void);


//
// Common functions
//

/**
 * Frees the given device entry (returned mostly by iterators).
 * It is recommended to free every dev instance before next
 * iterator call (or as soon as possible).
 */
void dtree_dev_free(struct dtree_dev_t *dev);

/**
 * Tests whether the module is in an error state.
 * When it is in an error state the behaviour of all
 * operations except dtree_close() and dtree_errstr()
 * is undefined.
 */
int dtree_iserror(void);

/**
 * When an error occures this function should
 * return a description of what happend.
 *
 * The pointer points to static memory.
 */
const char *dtree_errstr(void);

#endif
