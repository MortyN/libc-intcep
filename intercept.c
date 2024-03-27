#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

// Custom implementation of open
int open(const char *pathname, int flags, ...)
{
    int (*original_open)(const char *pathname, int flags, ...);
    // Load the original open function from libc
    original_open = dlsym(RTLD_NEXT, "open");

    // Your interception logic here
    // For example, you can print the file being opened
    printf("Intercepted open operation: %s\n", pathname);

    return original_open(pathname, flags);
}

// Custom implementation of open64
// open64() function is a part of the large file extensions, and is equivalent to calling open() with the O_LARGEFILE flag.
int open64(const char *pathname, int flags, ...)
{
    int (*original_open64)(const char *pathname, int flags, ...);
    // Load the original open64 function from libc
    original_open64 = dlsym(RTLD_NEXT, "open64");

    // Your interception logic here
    // For example, you can print the file being opened
    printf("Intercepted open64 operation: %s\n", pathname);

    return original_open64(pathname, flags);
}

// Custom implementation of write
// ssize_t write(int fd, const void *buf, size_t count)
// {
//     // Load the original write function from libc
//     orig_write_f_type original_write = (orig_write_f_type)dlsym(RTLD_NEXT, "write");

//     // Your interception logic here
//     // For example, you can print the data being written
//     printf("Intercepted write operation: %.*s\n", (int)count, (const char*)buf);

//     // Call the original write function
//     return original_write(fd, buf, count);
// }