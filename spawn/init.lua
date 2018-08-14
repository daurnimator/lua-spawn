local posix = require "spawn.posix"
local wait = require "spawn.wait"
local unix = require "unix"

local function start(program, ...)
	return posix.spawnp(program, nil, nil, { program, ... }, nil)
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

local function pclose(handle, pid)
	handle:close()
	local ok, str, status = unix.waitpid(pid)
	if not ok then
		return nil, str, status
	end

	return str == "exited" or nil, str, status
end

local function popen(cmd, mode)
	local dir = "r"
	local cloexec = ""
	if mode ~= nil then
		dir = mode:match("[rw]")
		if not dir then
			return nil -- TODO: EINVAL
		end
		cloexec = mode:match("e")
	end

	local read, write do
		local errno
		read, write, errno = unix.fpipe("e")
		if not read then
			return nil, write, errno
		end
	end

	local child_fd, child_handle, child_handle_fd, keep_handle
	if dir == "r" then
		child_fd = 1
		child_handle = write
		child_handle_fd = unix.fileno(child_handle)
		keep_handle = read
	else
		child_fd = 0
		child_handle = read
		keep_handle = write
	end
	--[[ If the child's end of the pipe happens to already be on the final
	fd number to which it will be assigned (either 0 or 1), it must
	be moved to a different fd. Otherwise, there is no safe way to
	remove the close-on-exec flag in the child without also creating
	 a file descriptor leak race condition in the parent.]]
	if child_fd == child_handle_fd then
		local new_child_handle, err, errno = unix.fdup(child_handle_fd, "e")
		if not new_child_handle then
			keep_handle:close()
			child_handle:close()
			return nil, err, errno
		end
		child_handle = new_child_handle
		child_handle_fd = unix.fileno(child_handle)
	end

	local file_actions, err, errno = posix.new_file_actions()
	if file_actions then
		local ok
		ok, err, errno = file_actions:adddup2(child_handle_fd, child_fd)
		if ok then
			local pid
			pid, err, errno = posix.spawnp("/bin/sh", file_actions, nil,
				{"sh", "-c", cmd})
			if pid then
				-- TODO: set pid into keep_handle
				-- TODO: make closing keep_handle call pclose
				child_handle:close()
				if cloexec ~= "e" then
					unix.fcntl(unix.F_SETFD, unix.FD_CLOEXEC)
				end
				return keep_handle
			end
		end
	end
	keep_handle:close()
	child_handle:close()
	return nil, err, errno
end

return {
	_VERSION = nil;
	start = start;
	run = run;
	system = system;
	popen = popen;
}
