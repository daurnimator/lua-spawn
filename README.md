A lua library to spawn programs

## Rationale

Spawning a new child program from a lua script is suprisingly hard.

Existing solutions:

  - [`os.execute`](http://www.lua.org/manual/5.3/manual.html#pdf-os.execute) doesn't allow interaction with any file descriptors
  - [`io.popen`](http://www.lua.org/manual/5.3/manual.html#pdf-io.popen) only allows interaction with *one of* `stdin` or `stdout`
  - `fork` + `exec` isn't portable to non-MMU or non-unix architectures

This project uses the [`posix_spawn`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/posix_spawn.html) family of functions.
