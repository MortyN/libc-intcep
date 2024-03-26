#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Function pointer types for the original functions
typedef int (*orig_open_f_type)(const char *pathname, int flags, ...);
typedef ssize_t (*orig_write_f_type)(int fd, const void *buf, size_t count);
typedef int (*orig_close_f_type)(int fd);
typedef int (*orig_fclose_f_type)(int fd);

// Function prototypes for the original functions
int open(const char *pathname, int flags, ...);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
int fclose(FILE *fd);

// Function to log the intercepted calls
void log_write(const char *pathname, ssize_t count)
{
    printf("[Intercept] Write to file: %s, Bytes written: %zd\n", pathname, count);
}

// Function to intercept open() calls
int open(const char *pathname, int flags, ...)
{
    orig_open_f_type orig_open;
    va_list args;
    mode_t mode = 0;

    va_start(args, flags);
    if (flags & O_CREAT)
    {
        mode = va_arg(args, mode_t);
    }
    va_end(args);

    orig_open = (orig_open_f_type)dlsym(RTLD_NEXT, "open");
    int fd = orig_open(pathname, flags, mode);
    if (fd != -1)
    {
        printf("[Intercept] Open file: %s\n", pathname);
    }
    return fd;
}

// Function to intercept write() calls
ssize_t write(int fd, const void *buf, size_t count)
{

    int redirected_fd = open("file2.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (redirected_fd == -1)
    {
        perror("Error opening file");
        return -1;
    }

    orig_write_f_type orig_write;
    orig_write = (orig_write_f_type)dlsym(RTLD_NEXT, "write");
    ssize_t ret = orig_write(fd, buf, count);
    if (ret != -1)
    {
        char proc_path[256];
        snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
        char fd_path[256];
        ssize_t link_len = readlink(proc_path, fd_path, sizeof(fd_path) - 1);
        if (link_len != -1)
        {
            fd_path[link_len] = '\0';
            log_write(fd_path, ret);
        }
    }

    ssize_t written = write(redirected_fd, buf, count);
    if (written == -1)
    {
        perror("Error writing to file2.txt");
        close(redirected_fd);
        return -1;
    }

    close(redirected_fd);

    return ret;
}

// Function to intercept close() calls
int close(int fd)
{
    orig_close_f_type orig_close;
    orig_close = (orig_close_f_type)dlsym(RTLD_NEXT, "close");
    int ret = orig_close(fd);
    if (ret != -1)
    {
        printf("[Intercept] Close file descriptor: %d\n", fd);
    }
    return ret;
}

// Function to intercept close() calls
int fclose(FILE *fd)
{
    orig_fclose_f_type orig_fclose;
    orig_fclose = (orig_fclose_f_type)dlsym(RTLD_NEXT, "fclose");
    int ret = orig_fclose(fd);
    if (ret != -1)
    {
        printf("[F-Intercept] Close file descriptor: %d\n", fd);
    }

    return ret;
}
