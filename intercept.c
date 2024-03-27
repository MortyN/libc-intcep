#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Function pointer types for the original functions
typedef int (*orig_open_f_type)(const char *pathname, int flags, ...);
typedef ssize_t (*orig_write_f_type)(int fd, const void *buf, size_t count);
typedef int (*orig_close_f_type)(int fd);
typedef int (*orig_fclose_f_type)(int fd);
typedef int (*fclose_func_t)(FILE *);

// Function prototypes for the original functions
int open(const char *pathname, int flags, ...);
ssize_t write(int fd, const void *buf, size_t count);

int *currentfd;

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
    if(fd != -1 && strstr(pathname, "filenamejava")) {
        *currentfd = fd;
    }
    return fd;
}

// Function to intercept write() calls
ssize_t write(int fd, const void *buf, size_t count)
{
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

    if(fd != -1 && currentfd != NULL && fd == *currentfd) {
        printf("[Intercept] HORRY SHIT");
    }

    return ret;
}
// Function to intercept write() calls
// ssize_t write(int fd, const void *buf, size_t count) {
//     // Load the original write function using dlsym
//     orig_write_f_type original_write = dlsym(RTLD_NEXT, "write");

//     // Get the filename associated with the file descriptor
//     char filename[256];
//     if (fcntl(fd, 50, filename) == -1) {
//         fprintf(stderr, "Error getting filename associated with file descriptor.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Check if the filename is "filenamejava.txt"
//     if (strstr(filename, "filenamejava.txt") != NULL) {
//         // Writing to a file with filename "filenamejava.txt"
//         fwrite(buf, sizeof(char), count, stdout);
//     }

//     // Call the original write function to perform the actual write
//     ssize_t result = original_write(fd, buf, count);

//     return result;
// }

// Function to intercept close() calls
// int close(int fd)
// {
//     orig_close_f_type orig_close;
//     orig_close = (orig_close_f_type)dlsym(RTLD_NEXT, "close");
//     int ret = orig_close(fd);
//     if (ret != -1)
//     {
//         printf("[Intercept] Close file descriptor: %d\n", fd);
//     }
//     return ret;
// }

// Define the hook function for fclose
// int fclose(FILE *stream) {
//     // Load the original fclose function using dlsym
//     fclose_func_t original_fclose = dlsym(RTLD_NEXT, "fclose");

//     // Backup current position of the stream
//     long original_position = ftell(stream);

//     // Set the file position indicator to the beginning of the file
//     rewind(stream);

//     // Print contents of the stream
//     int c;
//     while ((c = fgetc(stream)) != EOF) {
//         putchar(c);
//     }

//     // Restore original file position
//     fseek(stream, original_position, SEEK_SET);

//     // Call the original fclose function
//     return original_fclose(stream);
// }