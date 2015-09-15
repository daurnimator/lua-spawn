#include <sys/types.h> /* pid_t */
#include <signal.h> /* kill, SIGTERM */
#include <string.h> /* strerror */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

static int l_kill(lua_State *L) {
	int r;
	pid_t pid = luaL_optinteger(L, 1, 0);
	int sig = luaL_optinteger(L, 2, SIGTERM);
	if (0 != (r = kill(pid, sig))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static const luaL_Reg lib[] = {
	{ "kill", l_kill },
	{ NULL, NULL }
};

int luaopen_spawn_kill(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
