#!/usr/bin/env lua
--[[
This example shows off using lunix to create pipes for the child process
]]
local posix_spawn = require "spawn.posix"
local unix = require "unix"

local read, write, pid do
	-- file_actions is an ordered list of operations to do before the child process starts
	-- you can add operations to it such as `open()`, `dup2()` and `close()`
	local file_actions = posix_spawn.new_file_actions()
	local attr = posix_spawn.new_attr()
	-- Create two pipes, one for stdin, one shared by stdout and stderr
	-- the first fd returned by pipe() can only be read from
	-- the second fd returned by pipe() can only be written to
	-- "e" means to enable O_CLOEXEC (this way the other ends of the pipes are closed in the child)
	local child_stdin, child_stdout
	child_stdin, write = unix.fpipe("e")
	read, child_stdout = unix.fpipe("e")
	-- Tell posix_spawn to dup `child_stdin` to fd 0 in the child
	file_actions:adddup2(unix.fileno(child_stdin), 0)
	-- Tell posix_spawn to dup `child_stdout` to fd 1 in the child
	file_actions:adddup2(unix.fileno(child_stdout), 1)
	-- Tell posix_spawn to dup fd 1 (which is child_stdout already) to fd 2 (stderr)
	file_actions:adddup2(1, 2)
	-- Start the child program: cat
	pid = assert(posix_spawn.spawnp("cat", file_actions, attr, {"cat"}, nil))
	-- Close files now owned by the child
	child_stdin:close()
	child_stdout:close()
end
-- Lets write a string to the child
assert(write:write("foo"))
assert(write:flush())
-- The child is 'cat': we should be able to read "foo" back
print(read:read(3))
-- Close the child's stdin. For 'cat' this should result in the program exiting
write:close()
-- Wait for child to exit and print the exit status
print(unix.waitpid(pid))
