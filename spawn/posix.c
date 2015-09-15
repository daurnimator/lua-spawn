#include <spawn.h>
#include <sys/wait.h>
#include <errno.h> /* errno, EINTR */
#include <string.h> /* strerror */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

int luaopen_spawn_sigset(lua_State *L);

extern char** environ;

static const char** luaL_checkarraystrings(lua_State *L, int arg) {
	lua_Integer n;
	const char **ret;
	arg = lua_absindex(L, arg);
	luaL_checktype(L, arg, LUA_TTABLE);
	n = luaL_len(L, arg);
	ret = lua_newuserdata(L, (n+1)*sizeof(char*));
	ret[n] = NULL;
	for (; n>0; n--) {
		luaL_argcheck(L, lua_geti(L, arg, n) == LUA_TSTRING, arg, "expected array of strings");
		ret[n-1] = lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	return ret;
}

static const char** luaL_optarraystrings(lua_State *L, int n, const char** def) {
	return lua_isnoneornil(L, n) ? def : luaL_checkarraystrings(L, n);
}

static int l_posix_spawn(lua_State *L) {
	int r;
	pid_t pid;
	const char *path = luaL_checkstring(L, 1);
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 2, "posix_spawn_file_actions_t");
	posix_spawnattr_t *attr = luaL_checkudata(L, 3, "posix_spawnattr_t");
	const char **argv = luaL_checkarraystrings(L, 4);
	const char **envp = luaL_optarraystrings(L, 5, (const char**)environ);
	if (0 != (r = posix_spawn(&pid, path, file_actions, attr, (char*const*)argv, (char*const*)envp))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushinteger(L, pid);
	return 1;
}

/* Almost same API as posix_spawn */
static int l_posix_spawnp(lua_State *L) {
	int r;
	pid_t pid;
	const char *file = luaL_checkstring(L, 1);
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 2, "posix_spawn_file_actions_t");
	posix_spawnattr_t *attr = luaL_checkudata(L, 3, "posix_spawnattr_t");
	const char **argv = luaL_checkarraystrings(L, 4);
	const char **envp = luaL_optarraystrings(L, 5, (const char**)environ);
	if (0 != (r = posix_spawnp(&pid, file, file_actions, attr, (char*const*)argv, (char*const*)envp))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushinteger(L, pid);
	return 1;
}

static int l_posix_spawnattr_init(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = lua_newuserdata(L, sizeof(posix_spawnattr_t));
	luaL_setmetatable(L, "posix_spawnattr_t");
	if (0 != (r = posix_spawnattr_init(attr))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	return 1;
}

static int l_posix_spawnattr_destroy(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	if (0 != (r = posix_spawnattr_destroy(attr))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawnattr_getsigdefault(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	sigset_t *set = luaL_testudata(L, 2, "sigset_t");
	if (set) {
		lua_settop(L, 2);
	} else {
		lua_settop(L, 1);
		set = lua_newuserdata(L, sizeof(sigset_t));
		luaL_setmetatable(L, "sigset_t");
	}
	if (0 != (r = posix_spawnattr_getsigdefault(attr, set))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	return 1;
}

static int l_posix_spawnattr_setsigdefault(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	sigset_t *set = luaL_checkudata(L, 2, "sigset_t");
	if (0 != (r = posix_spawnattr_setsigdefault(attr, set))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawnattr_getsigmask(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	sigset_t *set = luaL_testudata(L, 2, "sigset_t");
	if (set) {
		lua_settop(L, 2);
	} else {
		lua_settop(L, 1);
		set = lua_newuserdata(L, sizeof(sigset_t));
		luaL_setmetatable(L, "sigset_t");
	}
	if (0 != (r = posix_spawnattr_getsigmask(attr, set))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	return 1;
}

static int l_posix_spawnattr_setsigmask(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	sigset_t *set = luaL_checkudata(L, 2, "sigset_t");
	if (0 != (r = posix_spawnattr_setsigmask(attr, set))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawnattr_getflags(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	short int flags;
	if (0 != (r = posix_spawnattr_getflags(attr, &flags))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_createtable(L, 0, 7);
	lua_pushboolean(L, flags & POSIX_SPAWN_RESETIDS);
	lua_setfield(L, -2, "resetids");
	lua_pushboolean(L, flags & POSIX_SPAWN_SETPGROUP);
	lua_setfield(L, -2, "setpgroup");
	lua_pushboolean(L, flags & POSIX_SPAWN_SETSIGDEF);
	lua_setfield(L, -2, "setsigdef");
	lua_pushboolean(L, flags & POSIX_SPAWN_SETSIGMASK);
	lua_setfield(L, -2, "setsigmask");
	lua_pushboolean(L, flags & POSIX_SPAWN_SETSCHEDPARAM);
	lua_setfield(L, -2, "setschedparam");
	lua_pushboolean(L, flags & POSIX_SPAWN_SETSCHEDULER);
	lua_setfield(L, -2, "setscheduler");
#ifdef POSIX_SPAWN_USEVFORK
	lua_pushboolean(L, flags & POSIX_SPAWN_USEVFORK);
	lua_setfield(L, -2, "usevfork");
#endif
	return 1;
}

static int l_posix_spawnattr_setflags(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	short int flags = 0;
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "resetids");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_RESETIDS;
	lua_getfield(L, 2, "setpgroup");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_SETPGROUP;
	lua_getfield(L, 2, "setsigdef");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_SETSIGDEF;
	lua_getfield(L, 2, "setsigmask");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_SETSIGMASK;
	lua_getfield(L, 2, "setschedparam");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_SETSCHEDPARAM;
	lua_getfield(L, 2, "setscheduler");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_SETSCHEDULER;
	lua_pop(L, 6);
#ifdef POSIX_SPAWN_USEVFORK
	lua_getfield(L, 2, "usevfork");
	if (lua_toboolean(L, -1)) flags |= POSIX_SPAWN_USEVFORK;
	lua_pop(L, 1);
#endif
	if (0 != (r = posix_spawnattr_setflags(attr, flags))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawnattr_getpgroup(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	pid_t pgroup;
	if (0 != (r = posix_spawnattr_getpgroup(attr, &pgroup))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushinteger(L, pgroup);
	return 1;
}

static int l_posix_spawnattr_setpgroup(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	pid_t pgroup = luaL_checkinteger(L, 2);
	if (0 != (r = posix_spawnattr_setpgroup(attr, pgroup))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawnattr_getschedpolicy(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	int schedpolicy;
	if (0 != (r = posix_spawnattr_getschedpolicy(attr, &schedpolicy))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushinteger(L, schedpolicy);
	return 1;
}

static int l_posix_spawnattr_setschedpolicy(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	int schedpolicy = luaL_checkinteger(L, 2);
	if (0 != (r = posix_spawnattr_setschedpolicy(attr, schedpolicy))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

/*
static int l_posix_spawnattr_getschedparam(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	struct schedparam;
	if (0 != (r = posix_spawnattr_getschedparam(attr, &schedparam))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}

	return 1;
}

static int l_posix_spawnattr_setschedparam(lua_State *L) {
	int r;
	posix_spawnattr_t *attr = luaL_checkudata(L, 1, "posix_spawnattr_t");
	struct schedparam = (L, 2);
	if (0 != (r = posix_spawnattr_setschedparam(attr, schedparam))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
}
*/


static int l_posix_spawn_file_actions_init(lua_State *L) {
	int r;
	posix_spawn_file_actions_t *file_actions = lua_newuserdata(L, sizeof(posix_spawn_file_actions_t));
	luaL_setmetatable(L, "posix_spawn_file_actions_t");
	if (0 != (r = posix_spawn_file_actions_init(file_actions))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	return 1;
}

static int l_posix_spawn_file_actions_destroy(lua_State *L) {
	int r;
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 1, "posix_spawn_file_actions_t");
	if (0 != (r = posix_spawn_file_actions_destroy(file_actions))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawn_file_actions_addopen(lua_State *L) {
	int r;
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 1, "posix_spawn_file_actions_t");
	int fd = luaL_checkinteger(L, 2);
	const char *path = luaL_checkstring(L, 3);
	int oflag = luaL_checkinteger(L, 4);
	mode_t mode = luaL_checkinteger(L, 4);
	if (0 != (r = posix_spawn_file_actions_addopen(file_actions, fd, path, oflag, mode))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawn_file_actions_addclose(lua_State *L) {
	int r;
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 1, "posix_spawn_file_actions_t");
	int fd = luaL_checkinteger(L, 2);
	if (0 != (r = posix_spawn_file_actions_addclose(file_actions, fd))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int l_posix_spawn_file_actions_adddup2(lua_State *L) {
	int r;
	posix_spawn_file_actions_t *file_actions = luaL_checkudata(L, 1, "posix_spawn_file_actions_t");
	int fd = luaL_checkinteger(L, 2);
	int newfd = luaL_checkinteger(L, 3);
	if (0 != (r = posix_spawn_file_actions_adddup2(file_actions, fd, newfd))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(r));
		lua_pushinteger(L, r);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

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
		return luaL_error(L, "waitpid: %s", strerror(r));
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

static const luaL_Reg spawnattr_methods[] = {
	{ "getsigdefault", l_posix_spawnattr_getsigdefault },
	{ "setsigdefault", l_posix_spawnattr_setsigdefault },
	{ "getsigmask", l_posix_spawnattr_getsigmask },
	{ "setsigmask", l_posix_spawnattr_setsigmask },
	{ "getflags", l_posix_spawnattr_getflags },
	{ "setflags", l_posix_spawnattr_setflags },
	{ "getpgroup", l_posix_spawnattr_getpgroup },
	{ "setpgroup", l_posix_spawnattr_setpgroup },
	{ "getschedpolicy", l_posix_spawnattr_getschedpolicy },
	{ "setschedpolicy", l_posix_spawnattr_setschedpolicy },
	// { "getschedparam", l_posix_spawnattr_getschedparam },
	// { "setschedparam", l_posix_spawnattr_setschedparam },
	{ NULL, NULL }
};

static const luaL_Reg spawn_file_actions_methods[] = {
	{ "addopen", l_posix_spawn_file_actions_addopen },
	{ "addclose", l_posix_spawn_file_actions_addclose },
	{ "adddup2", l_posix_spawn_file_actions_adddup2 },
	{ NULL, NULL }
};

static const luaL_Reg lib[] = {
	{ "spawn", l_posix_spawn },
	{ "spawnp", l_posix_spawnp },
	{ "new_attr", l_posix_spawnattr_init },
	{ "new_file_actions", l_posix_spawn_file_actions_init },
	{ "waitpid", l_waitpid },
	{ NULL, NULL }
};

int luaopen_spawn_posix(lua_State *L) {
	luaL_newmetatable(L, "posix_spawnattr_t");
	lua_pushcfunction(L, l_posix_spawnattr_destroy);
	lua_setfield(L, -2, "__gc");
	luaL_newlib(L, spawnattr_methods);
	lua_setfield(L, -2, "__index");
	luaL_newmetatable(L, "posix_spawn_file_actions_t");
	lua_pushcfunction(L, l_posix_spawn_file_actions_destroy);
	lua_setfield(L, -2, "__gc");
	luaL_newlib(L, spawn_file_actions_methods);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 2);
	/* make sure sigset module is loaded */
	luaL_requiref(L, "spawn.sigset", luaopen_spawn_sigset, 0);

	luaL_newlib(L, lib);
	return 1;
}
