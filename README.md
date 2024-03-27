# Intercepting programming languages using libc

This is a proof of concept repo for intercepting and debugging libc interactions between programming language like Java and the elementary libc library.

## Contents of repo
 - vscode devcontainer for local development inside Linux docker container with all needed extensions
 - vscode tasks. and launch.json to build interceptor shared object, and attach gdb debugger to whatever programming language is to debug (java, csharp etc.)
 - various simple programming language tests
 - intercept.c file, the libc hooks are stored here, which are common for all programming languages utilizing libc.