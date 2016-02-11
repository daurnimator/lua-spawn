#include <errno.h> /* errno */
#include <string.h> /* strerror */
#include <unistd.h> /* pipe */

#include <lua.h>
#include <lauxlib.h>

#include "lua-spawn.h"

static int l_pipe(lua_State *L) {
	int fildes[2];
	if (0 != pipe(fildes)) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushinteger(L, fildes[0]);
	lua_pushinteger(L, fildes[1]);
	return 2;
};

static const luaL_Reg lib[] = {
	{ "pipe", l_pipe },
	{ NULL, NULL }
};

int luaopen_spawn_pipe(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
