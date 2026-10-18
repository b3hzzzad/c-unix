#include "cunix.h"

#define BUFFER_SIZE 4096

//write function to handle partial writes also
int ft_write(char *str, size_t length)
{
    ssize_t bytes_written;
    size_t   offset;

    offset = 0;
    while(offset < length)
    {
        bytes_written = write(STDOUT_FILENO, str + offset, length - offset);
        if(bytes_written < 0)
        {
            perror("write");
            return 1;
        }
        if(bytes_written == 0)
            return 1;
        offset += bytes_written;
    }
    return 0;
}

int write_all(char *buffer, int fd)
{
    ssize_t  offset;
    ssize_t  bytes_read;
    ssize_t  bytes_written;

    if(buffer == NULL || fd < 0)
        return 1;

    while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        offset = 0;
        while(offset < bytes_read)
        {
            bytes_written = write(1, buffer + offset,
                bytes_read - offset);

            if(bytes_written < 0)
            {
                perror("write");
                return 1;
            }

            if(bytes_written == 0)
            {
                fprintf(stderr, "write: no progress\n");
                return 1;
            }

            offset += bytes_written;
        }
    }
    if(bytes_read < 0)
    {
        perror("read");
        return 1;
    }
    return 0;
}

//reads up to total_bytes not all the file
ssize_t read_all(char *buffer, ssize_t total_bytes, int fd)
{
    ssize_t bytes_read;
    off_t   offset;

    if(buffer == NULL || fd < 0)
        return -1;
    offset = 0;
    while(offset < total_bytes)
    {
        bytes_read = read(fd, buffer + offset, total_bytes - offset);
        if(bytes_read < 0)
        {
            perror("read");
            return -1;
        }
        if(bytes_read == 0)
            break; // end of the file
        offset += bytes_read;
    }
    return (ssize_t)offset;
}
