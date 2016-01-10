package = "spawn"
version = "scm-0"
source = {
	url = "git://github.com/daurnimator/lua-spawn.git";
}
description = {
	summary = "A lua library to spawn programs";
	homepage = "https://github.com/daurnimator/lua-spawn";
	license = "MIT/X11";
}
supported_platforms = {
	"unix";
}
dependencies = {
	"lua >= 5.1, < 5.4";
}
build = {
	type = "builtin";
	modules = {
		["spawn.init"] = "spawn/init.lua";
		["spawn"] = {
			defines = { "_POSIX_C_SOURCE=200809L" };
			incdirs = { "vendor/compat-5.3/c-api/" };
			sources = {
				"spawn/kill.c";
				"spawn/posix.c";
				"spawn/sigset.c";
				"spawn/wait.c";
			};
		};
	}
}
