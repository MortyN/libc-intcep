# Intercepting programming languages using libc

This is a proof of concept repo for intercepting and debugging libc interactions between programming language like Java and the elementary libc library.

## Contents of repo
 - vscode devcontainer for local development inside Linux docker container with all needed extensions
 - vscode tasks. and launch.json to build interceptor shared object, and attach gdb debugger to whatever programming language is to debug (java, csharp etc.)
 - various simple programming language tests
 - intercept.c file, the libc hooks are stored here, which are common for all programming languages utilizing libc.


## Findings for hooking into golang
Golang does not utilize libc anymore, and hooking is a bit more difficult. 
however, hooking into the kernel syscalls directly is possible, here i have made a go binary writing a file named filenamejava.txt:
```bash
$ sudo strace -e trace=write ./writer

--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=8382, si_uid=0} ---
...
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