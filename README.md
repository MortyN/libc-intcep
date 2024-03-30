# Intercepting libc based programming languages

This is a proof of concept repo for intercepting and debugging interactions between programming languages like Java, CSharp etc. and the elementary libc (C Standard Library).

## How it works
This proof of concept utilizes linux's `LD_PRELOAD` to link shared libraries and most importantly override symbols in existing binary at runtime. This essentially enables adding functionality to existing code with ease, if we know what symbols to interact with, which in the case of libc we do as it is open source.
For example, in the `intercept.c` code written in the root folder, there are symbol overrides for `open()` and `open64()` (open64 is part of large file extensions), which means whenever the `LD_PRELOAD` hooked binary (written in Java, CSharp etc.) executes a `File.open()` internally, it will instead use symbols from our custom runtime injected `intercept.so` library instead of libc's `libc.so` (libc's shared object) `open()` or `open64()`.

## Contents of repo
 - vscode devcontainer for local development inside Linux docker container with all needed extensions
 - vscode `tasks.json` and `launch.json` to build interceptor shared object, and attach gdb debugger to whatever programming language is to debug (Java, CSharp etc.)
 - various simple programming language tests
 - intercept.c file, the libc hooks are stored here, which are common for all programming languages utilizing libc.


## Findings for hooking into golang
Golang does not utilize libc anymore, and hooking is a bit more difficult. 
However, hooking into the kernel syscalls directly is possible, here i have made a go binary writing a file named `filenamejava.txt` with the content `hello`, 
and traces of both write and openat is present, however only by using ptrace (which strace uses internally):
```bash
$ sudo strace -e trace=write,openat ./writer 
openat(AT_FDCWD, "/sys/kernel/mm/transparent_hugepage/hpage_pmd_size", O_RDONLY) = 3
--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=10142, si_uid=0} ---
...
openat(AT_FDCWD, "filenamejava.txt", O_RDWR|O_CREAT|O_TRUNC|O_CLOEXEC, 0666) = 3
write(3, "hello", 5)                    = 5 
write(1, "Successfully wrote to filenameja"..., 39Successfully wrote to filenamejava.txt
) = 39
+++ exited with 0 +++

```

## Useful docs and Q&A
 - https://catonmat.net/simple-ld-preload-tutorial-part-two
 - [libeatmydata](https://github.com/stewartsmith/libeatmydata)
 - [ebpf write syscall hook](https://github.com/vishen/bpf-writesnoop/tree/master)
 - [go syscall "detour" (mirrord)](https://github.com/metalbear-co/mirrord/blob/main/mirrord/layer/src/go/linux_x64.rs)
 - [c syscall intercept](https://github.com/pmem/syscall_intercept)
 - [go intercept with c](https://notes.eatonphil.com/2023-10-01-intercepting-and-modifying-linux-system-calls-with-ptrace.html)
 - [dynamic hooking of go program](https://blog.quarkslab.com/lets-go-into-the-rabbit-hole-part-1-the-challenges-of-dynamically-hooking-golang-program.html)
