#include <errno.h> /* errno */
#include <signal.h> /* kill, SIGTERM */
#include <string.h> /* strerror */
#include <sys/types.h> /* pid_t */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include "lua-spawn.h"

static int l_kill(lua_State *L) {
	pid_t pid = luaL_optinteger(L, 1, 0);
	int sig = luaL_optinteger(L, 2, SIGTERM);
	if (0 != kill(pid, sig)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
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
