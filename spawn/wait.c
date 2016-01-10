#include <sys/types.h> /* pid_t */
#include <sys/wait.h>
#include <errno.h> /* errno, EINTR */
#include <string.h> /* strerror */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include "lua-spawn.h"

static int l_waitpid(lua_State *L) {
	int r;
	pid_t which_pid;
	pid_t pid = luaL_optinteger(L, 1, -1);
	int options = 0;
	int stat;
	const char *what = "exit";  /* type of termination */
	switch(lua_type(L, 2)) {
		case LUA_TNIL:
		case LUA_TNONE:
			break;
		case LUA_TNUMBER:
			options = lua_tointeger(L, 2);
			break;
		case LUA_TTABLE:
			lua_getfield(L, 2, "nohang");
			if (lua_toboolean(L, -1)) options |= WNOHANG;
			lua_getfield(L, 2, "untraced");
			if (lua_toboolean(L, -1)) options |= WUNTRACED;
			lua_pop(L, 2);
#ifdef WIFCONTINUED
			lua_getfield(L, 2, "continued");
			if (lua_toboolean(L, -1)) options |= WCONTINUED;
			lua_pop(L, 1);
#endif
			break;
		default:
			return luaL_argerror(L, 2, "expected nil, an integer or a table of options ");
	}
waitpid:
	if (-1 == (which_pid = waitpid(pid, &stat, options))) {
		r = errno;
		if (r == EINTR) goto waitpid;
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	/* similar to lua 5.2+'s luaL_execresult */
	if ((options & WNOHANG) && which_pid == 0) {
		lua_pushboolean(L, 0);
		return 1;
	} else if (WIFEXITED(stat)) {
		stat = WEXITSTATUS(stat);
	} else if (WIFSIGNALED(stat)) {
		stat = WTERMSIG(stat);
		what = "signal";
	} else if (WIFSTOPPED(stat)) {
		stat = WSTOPSIG(stat);
		what = "stop";
#ifdef WIFCONTINUED
	} else if (WIFCONTINUED(stat)) {
		stat = SIGCONT;
		what = "continue";
#endif
	}
	if (*what == 'e' && stat == 0)  /* successful termination? */
		lua_pushboolean(L, 1);
	else
		lua_pushnil(L);
	lua_pushstring(L, what);
	lua_pushinteger(L, stat);
	return 3;  /* return true/nil,what,code */
}

static const luaL_Reg lib[] = {
	{ "waitpid", l_waitpid },
	{ NULL, NULL }
};

int luaopen_spawn_wait(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
