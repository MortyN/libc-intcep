#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <stdint.h>

#define ASSIGN_DLSYM_OR_DIE(name)			\
        libc_##name = (libc_##name##_##t)(intptr_t)dlsym(RTLD_NEXT, #name);			\
        if (!libc_##name)                       \
        {                                       \
                const char *dlerror_str = dlerror();                          \
                fprintf(stderr, "init error for %s: %s\n", #name,\
                        dlerror_str ? dlerror_str : "(null)");                \
                _exit(1);                       \
        }

typedef int (*libc_open_t)(const char*, int, ...);
typedef int (*libc_open64_t)(const char*, int, ...);

/* All the following are thread-local, to avoid initialization races between
 * threads. */
static __thread libc_open_t libc_open= NULL;
static __thread libc_open64_t libc_open64= NULL;

static void preload_init() {
    ASSIGN_DLSYM_OR_DIE(open);
	ASSIGN_DLSYM_OR_DIE(open64);
}

int *target_fd;

// Custom implementation of open
int open64(const char *pathname, int flags, ...)
{
    if (!libc_open64) preload_init();

    va_list argp;
    va_start(argp, flags);
    mode_t mode = va_arg(argp, mode_t);
    va_end(argp);

    const char str[17] = "filenamejava.txt";
    if (strstr(pathname, str) != NULL)
    {
        printf("Intercepted open64: %s\n", pathname);
    }

	return (*libc_open64)(pathname,flags,mode);
}

// Custom implementation of open
int open(const char *pathname, int flags, ...)
{
   if (!libc_open) preload_init();

    va_list argp;
    va_start(argp, flags);
    mode_t mode = va_arg(argp, mode_t);
    va_end(argp);

    const char str[17] = "filenamejava.txt";
    if (strstr(pathname, str) != NULL)
    {
        printf("Intercepted open: %s\n", pathname);
    }

	return (*libc_open64)(pathname,flags,mode);
}

// Custom implementation of open64
// open64() function is a part of the large file extensions, and is equivalent to calling open() with the O_LARGEFILE flag.
// int open64(const char *pathname, int flags, ...)
// {
//     int (*original_open64)(const char *pathname, int flags, ...);
//     // Load the original open64 function from libc
//     original_open64 = dlsym(RTLD_NEXT, "open64");

//     // Your interception logic here
//     // For example, you can print the file being opened
//     printf("Intercepted open64 operation: %s\n", pathname);

//     const char str[17] = "filenamejava.txt";

//     if (strstr(pathname, str) != NULL)
//     {
//         printf("efoiejwghew: %s\n", pathname);
//     }

//     return original_open64(pathname, flags);
// }

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