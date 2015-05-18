local posix = require "spawn.posix"

local default_file_actions = posix.new_file_actions()
local default_attr = posix.new_attr()

local function start(program, ...)
	return posix.spawnp(program, default_file_actions, default_attr,
		{ program, ... }, nil)
end

local function run(program, ...)
	return posix.waitpid(start(program, ...))
end

return {
	posix = posix;
	start = start;
	run = run;
}
