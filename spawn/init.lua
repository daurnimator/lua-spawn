local kill = require "spawn.kill"
local posix = require "spawn.posix"
local sigset = require "spawn.sigset"
local signals = require "spawn.signals"
local wait = require "spawn.wait"

local default_file_actions = posix.new_file_actions()
local default_attr = posix.new_attr()

local function start(program, ...)
	return posix.spawnp(program, default_file_actions, default_attr,
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
	kill = kill;
	posix = posix;
	sigset = sigset;
	signals = signals;
	wait = wait;

	start = start;
	run = run;
	system = system;
}
