package = "spawn"
version = "0.1-0"
source = {
	url = "https://github.com/daurnimator/lua-spawn/archive/v0.1.zip";
	dir = "lua-spawn-0.1";
}
description = {
	summary = "A lua library to spawn programs";
	homepage = "https://github.com/daurnimator/lua-spawn";
	license = "MIT";
}
supported_platforms = {
	"unix";
}
dependencies = {
	"lua >= 5.1, < 5.4";
	"lunix";
}
build = {
	type = "builtin";
	modules = {
		["spawn.init"] = "spawn/init.lua";
		["spawn.posix"] = {
			defines = {
				"_POSIX_C_SOURCE=200809L";
				"_GNU_SOURCE";
			};
			incdirs = { "vendor/compat-5.3/c-api/" };
			sources = {
				"spawn/posix.c";
			};
		};
		["spawn.wait"] = "spawn/wait.lua";
	}
}
