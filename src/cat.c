#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

/* Returns 0 on success, 1 on failure. */
int mini_cat(const char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t  bytes_written;
    size_t  offset;
    int     fd;

    if(file_path == NULL)
    {
        fprintf(stderr, "invalid input.\n");
        return 1;
    }
    fd = open(file_path, O_RDONLY);
    if(fd < 0)
    {
        perror(file_path);
        return 1;   
    }
    while(1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if(bytes_read == -1)
        {
            perror("read");
            close(fd);
            return 1;
        }
        if(bytes_read == 0)
            break;
        offset = 0;
        while(offset < (size_t)bytes_read)
        {
            bytes_written = write(STDOUT_FILENO,
                buffer + offset, bytes_read - offset);
            if(bytes_written < 0)
            {
                perror("write");
                close(fd);
                return 1;
            }
            if (bytes_written == 0)
            {
                fprintf(stderr, "write: no progress.\n");
                close(fd);
                return 1;
            }
            offset += bytes_written;
        }
    }
    if(close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return 0;
}