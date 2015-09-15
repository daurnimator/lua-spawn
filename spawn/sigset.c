/* bind sigset_t manipulation functions */

#include <signal.h> /* sigset_t, sigemptyset, sigfillset, sigaddset, sigdelset, sigismember */
#include <errno.h> /* errno */
#include <string.h> /* strerror */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

static int l_sigset_empty(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	if (0 != sigemptyset(set)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_sigset_fill(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	if (0 != sigfillset(set)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_sigset_add(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	int signum = luaL_checkinteger(L, 2);
	if (0 != sigaddset(set, signum)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_sigset_del(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	int signum = luaL_checkinteger(L, 2);
	if (0 != sigdelset(set, signum)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_sigset_ismember(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	int signum = luaL_checkinteger(L, 2);
	int r = sigismember(set, signum);
	if (-1 == r) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, r);
	return 1;
}

static int l_sigset_new(lua_State *L) {
	sigset_t *set = lua_newuserdata(L, sizeof(sigset_t));
	luaL_setmetatable(L, "sigset_t");
	/* initialise to empty */
	if (0 != sigemptyset(set)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	return 1;
}

static int l_sigset_tostring(lua_State *L) {
	sigset_t *set = luaL_checkudata(L, 1, "sigset_t");
	lua_pushfstring(L, "sigset_t: %p", set);
	return 1;
}

static const luaL_Reg methods[] = {
	{ "empty", l_sigset_empty },
	{ "fill", l_sigset_fill },
	{ "add", l_sigset_add },
	{ "del", l_sigset_del },
	{ "ismember", l_sigset_ismember },
	{ NULL, NULL }
};

static const luaL_Reg lib[] = {
	{ "new", l_sigset_new },
	{ NULL, NULL }
};

int luaopen_spawn_sigset(lua_State *L) {
	luaL_newmetatable(L, "sigset_t");
	luaL_newlib(L, methods);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, l_sigset_tostring);
	lua_setfield(L, -2, "__tostring");
	/* key for pluto and eris to say this userdata can be safely serialised */
	lua_pushboolean(L, 1);
	lua_setfield(L, -2, "__persist");
	lua_pop(L, 2);

	luaL_newlib(L, lib);
	return 1;
}
