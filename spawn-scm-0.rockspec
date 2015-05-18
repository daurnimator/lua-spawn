package = "spawn"
version = "scm-0"
source = {
	url = "git://github.com/daurnimator/lua-spawn.git"
}
description = {
	summary = "A lua library to spawn programs";
	homepage = "https://github.com/daurnimator/lua-spawn";
	license = "MIT/X11";
}
supported_platforms = {
	"unix";
}
dependencies = {}
build = {
	type = "builtin";
	modules = {
		["spawn.init"] = "spawn/init.lua";
		["spawn.posix"] = {
			sources = "spawn/posix.c";
		};
	}
}
