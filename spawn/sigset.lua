local unix = require "unix"

return {
	new = function()
		return unix.sigemptyset();
	end;
	empty = unix.sigemptyset;
	fill = unix.sigfillset;
	add = unix.sigaddset;
	del = unix.sigdelset;
	ismember = unix.sigismember;
}
