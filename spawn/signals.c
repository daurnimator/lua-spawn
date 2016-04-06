#include <signal.h>
#include <limits.h> /* NSIG_MAX */
#include <string.h> /* strsignal */

#ifndef NSIG_MAX
#if defined NSIG
#define NSIG_MAX NSIG
#elif defined _NSIG
#define NSIG_MAX _NSIG
#else
#warning "NSIG_MAX not available"
#endif
#endif

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include "lua-spawn.h"

static int l_signals_strsignal(lua_State *L) {
	int sig = luaL_checkinteger(L, 1);
	lua_pushstring(L, strsignal(sig));
	return 1;
}

static const luaL_Reg lib[] = {
	{ "strsignal", l_signals_strsignal },
	{ NULL, NULL }
};


int luaopen_spawn_signals(lua_State *L) {
	luaL_newlib(L, lib);

#if defined NSIG_MAX
	lua_pushinteger(L, NSIG_MAX);
	lua_setfield(L, -2, "NSIG_MAX");
#endif

	lua_newtable(L);
#if defined SIGHUP
	lua_pushinteger(L, SIGHUP);
	lua_setfield(L, -2, "HUP");
#endif
#if defined SIGINT
	lua_pushinteger(L, SIGINT);
	lua_setfield(L, -2, "INT");
#endif
#if defined SIGQUIT
	lua_pushinteger(L, SIGQUIT);
	lua_setfield(L, -2, "QUIT");
#endif
#if defined SIGILL
	lua_pushinteger(L, SIGILL);
	lua_setfield(L, -2, "ILL");
#endif
#if defined SIGTRAP
	lua_pushinteger(L, SIGTRAP);
	lua_setfield(L, -2, "TRAP");
#endif
#if defined SIGABRT
	lua_pushinteger(L, SIGABRT);
	lua_setfield(L, -2, "ABRT");
#endif
#if defined SIGBUS
	lua_pushinteger(L, SIGBUS);
	lua_setfield(L, -2, "BUS");
#endif
#if defined SIGFPE
	lua_pushinteger(L, SIGFPE);
	lua_setfield(L, -2, "FPE");
#endif
#if defined SIGKILL
	lua_pushinteger(L, SIGKILL);
	lua_setfield(L, -2, "KILL");
#endif
#if defined SIGUSR1
	lua_pushinteger(L, SIGUSR1);
	lua_setfield(L, -2, "USR1");
#endif
#if defined SIGSEGV
	lua_pushinteger(L, SIGSEGV);
	lua_setfield(L, -2, "SEGV");
#endif
#if defined SIGUSR2
	lua_pushinteger(L, SIGUSR2);
	lua_setfield(L, -2, "USR2");
#endif
#if defined SIGPIPE
	lua_pushinteger(L, SIGPIPE);
	lua_setfield(L, -2, "PIPE");
#endif
#if defined SIGALRM
	lua_pushinteger(L, SIGALRM);
	lua_setfield(L, -2, "ALRM");
#endif
#if defined SIGTERM
	lua_pushinteger(L, SIGTERM);
	lua_setfield(L, -2, "TERM");
#endif
#if defined SIGSTKFLT
	lua_pushinteger(L, SIGSTKFLT);
	lua_setfield(L, -2, "STKFLT");
#endif
#if defined SIGCLD
	lua_pushinteger(L, SIGCLD);
	lua_setfield(L, -2, "CLD");
#endif
#if defined SIGCHLD
	lua_pushinteger(L, SIGCHLD);
	lua_setfield(L, -2, "CHLD");
#endif
#if defined SIGCONT
	lua_pushinteger(L, SIGCONT);
	lua_setfield(L, -2, "CONT");
#endif
#if defined SIGSTOP
	lua_pushinteger(L, SIGSTOP);
	lua_setfield(L, -2, "STOP");
#endif
#if defined SIGTSTP
	lua_pushinteger(L, SIGTSTP);
	lua_setfield(L, -2, "TSTP");
#endif
#if defined SIGTTIN
	lua_pushinteger(L, SIGTTIN);
	lua_setfield(L, -2, "TTIN");
#endif
#if defined SIGTTOU
	lua_pushinteger(L, SIGTTOU);
	lua_setfield(L, -2, "TTOU");
#endif
#if defined SIGURG
	lua_pushinteger(L, SIGURG);
	lua_setfield(L, -2, "URG");
#endif
#if defined SIGXCPU
	lua_pushinteger(L, SIGXCPU);
	lua_setfield(L, -2, "XCPU");
#endif
#if defined SIGXFSZ
	lua_pushinteger(L, SIGXFSZ);
	lua_setfield(L, -2, "XFSZ");
#endif
#if defined SIGVTALRM
	lua_pushinteger(L, SIGVTALRM);
	lua_setfield(L, -2, "VTALRM");
#endif
#if defined SIGPROF
	lua_pushinteger(L, SIGPROF);
	lua_setfield(L, -2, "PROF");
#endif
#if defined SIGWINCH
	lua_pushinteger(L, SIGWINCH);
	lua_setfield(L, -2, "WINCH");
#endif
#if defined SIGPOLL
	lua_pushinteger(L, SIGPOLL);
	lua_setfield(L, -2, "POLL");
#endif
#if defined SIGIO
	lua_pushinteger(L, SIGIO);
	lua_setfield(L, -2, "IO");
#endif
#if defined SIGPWR
	lua_pushinteger(L, SIGPWR);
	lua_setfield(L, -2, "PWR");
#endif
#if defined SIGSYS
	lua_pushinteger(L, SIGSYS);
	lua_setfield(L, -2, "SYS");
#endif
#if defined SIGRTMIN
	lua_pushinteger(L, SIGRTMIN);
	lua_setfield(L, -2, "RTMIN");
#endif
#if defined SIGRTMAX
	lua_pushinteger(L, SIGRTMAX);
	lua_setfield(L, -2, "RTMAX");
#endif
	lua_setfield(L, -2, "signals");

	return 1;
}
