A lua library to spawn programs

## Status

[![Build Status](https://travis-ci.org/daurnimator/lua-spawn.svg)](https://travis-ci.org/daurnimator/lua-spawn)


## Features

  - High level functions for common cases
  - Bindings to the full `posix_spawn` family of functions
  - Compatible with Lua 5.1, 5.2, 5.3 and [LuaJIT](http://luajit.org/)

Currently only works on POSIXy systems, but the high level module should be portable to Windows; someone just needs to put the work in.


## Rationale

Spawning a new child program from a lua script is surprisingly hard.

Existing solutions:

  - [`os.execute`](http://www.lua.org/manual/5.3/manual.html#pdf-os.execute) doesn't allow interaction with any file descriptors
  - [`io.popen`](http://www.lua.org/manual/5.3/manual.html#pdf-io.popen) only allows interaction with *one of* `stdin` or `stdout`
  - `fork` + `exec` isn't portable to non-MMU or non-unix architectures

This project uses the [`posix_spawn`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/posix_spawn.html) family of functions.
