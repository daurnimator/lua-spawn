local posix = require "spawn.posix"
local wait = require "spawn.wait"

local default_file_actions = assert(posix.new_file_actions())

local function start(program, ...)
	return posix.spawnp(program, default_file_actions, nil,
		{ program, ... }, nil)
end

local function run(...)
	local pid, err, errno = start(...)
	if pid == nil then
		return nil, err, errno
	end
	return wait.waitpid(pid)
end

local function system(arg)
	return run("/bin/sh", "-c", arg)
end

return {
	_VERSION = nil;
	start = start;
	run = run;
	system = system;
}
