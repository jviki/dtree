/**
 * lua_dtree.c
 * Copyright (C) 2012 Jan Viktorin
 */

#include "dtree.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
 * Check access to the library.
 * Can not call open nor close twice.
 */
enum l_dtree_state {
	L_DTREE_OPEN,
	L_DTREE_CLOSE
};

/**
 * Current state of the dtree library.
 */
static
enum l_dtree_state state = L_DTREE_CLOSE;


//
// Open & close for Lua
//

static
int l_dtree_open(lua_State *l)
{
	if(state == L_DTREE_OPEN)
		luaL_error(l, "The dtree.open() has already been called without a dtree.close()");

	const int argc = lua_gettop(l);
	const char *path = NULL;

	switch(argc) {
	case 0:
		path = "/proc/device-tree";
		break;

	case 1:
		path = luaL_checkstring(l, 1);
		break;
	
	default:
		luaL_error(l, "Too many arguments (%d)", argc);
		break;
	}

	if(dtree_open(path))
		luaL_error(l, dtree_errstr());

	state = L_DTREE_OPEN;
	return 0;
}

static
int l_dtree_close(lua_State *l)
{
	if(state == L_DTREE_CLOSE)
		luaL_error(l, "Invalid call to dtree.close(), call dtree.open() first");

	dtree_close();
	state = L_DTREE_CLOSE;
	return 0;
}


//
// Register in Lua
//

static const
struct luaL_reg dtreelib[] =
{
	{"open",  l_dtree_open},
	{"close", l_dtree_close},
	{NULL, NULL}
};

int luaopen_dtreelib(lua_State *l)
{
	luaL_openlib(l, "dtree", dtreelib, 0);
	return 1;
}
