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

static
int l_dtree_next(lua_State *l) {
	if(state == L_DTREE_CLOSE)
		luaL_error(l, "Invalid call to dtree.next(), call dtree.open() first");

	struct dtree_dev_t *dev = dtree_next();
	if(dev == NULL) {
		if(dtree_iserror())
			luaL_error(l, dtree_errstr());

		lua_pushboolean(l, 0);
		return 1;
	}

	lua_createtable(l, 0, 4);

	lua_pushstring(l, "name");
	lua_pushstring(l, dtree_dev_name(dev));
	lua_rawset(l, -3);

	lua_pushstring(l, "base");
	lua_pushinteger(l, dtree_dev_base(dev));
	lua_rawset(l, -3);

	lua_pushstring(l, "high");
	lua_pushinteger(l, dtree_dev_high(dev));
	lua_rawset(l, -3);

	lua_pushstring(l, "compat");
	lua_newtable(l);
	const char **compat = dtree_dev_compat(dev);
	int i;

	for(i = 0; compat[i] != NULL; ++i) {
		lua_pushstring(l, compat[i]);
		lua_rawseti(l, -2, i);
	}

	lua_rawset(l, -3);

	dtree_dev_free(dev);
	return 1;
}

//
// Register in Lua
//

static const
struct luaL_reg dtreelib[] =
{
	{"open",  l_dtree_open},
	{"close", l_dtree_close},
	{"next",  l_dtree_next},
	{NULL, NULL}
};

int luaopen_dtreelib(lua_State *l)
{
	if(sizeof(lua_Integer) < sizeof(dtree_addr_t)) {
		luaL_error(l,
		  "Too short lua_Integer (%d B) data type, required at least %d B",
		  sizeof(lua_Integer), sizeof(dtree_addr_t));

		return 0;
	}

	luaL_openlib(l, "dtree", dtreelib, 0);
	return 1;
}

#ifdef TEST
#include <stdio.h>

int main(void)
{
	lua_State *l;
	l = luaL_newstate();
	luaL_openlibs(l);
	luaopen_dtreelib(l);
	printf("result: %d\n", luaL_dofile(l, "test.lua"));
	lua_close(l);
	return 0;
}
#endif
