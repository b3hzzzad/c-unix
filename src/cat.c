#include "cunix.h"

#define BUFFER_SIZE 4096

/* Returns 0 on success, 1 on failure. */
int cat(const char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
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
        if (ft_write(buffer, bytes_read) != 0)// write and also check for errors
        {
            close(fd);
            return 1;
        }
    }
    if(close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return 0;
}