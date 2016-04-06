#ifndef LUASPAWN_H
#define LUASPAWN_H

#include <lua.h>

int luaopen_spawn_kill(lua_State *L);
int luaopen_spawn_posix(lua_State *L);
int luaopen_spawn_signals(lua_State *L);
int luaopen_spawn_sigset(lua_State *L);
int luaopen_spawn_wait(lua_State *L);

#endif /* LUASPAWN_H */
