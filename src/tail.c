#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "utils.h"

#define BUFFER_SIZE 4096

/* Returns 0 on success, 1 on failure. */
int tail(char *file_path, size_t lines_to_print)
{
    char    buffer[BUFFER_SIZE];
    off_t   total_bytes;
    off_t   remaining_bytes;
    size_t  count_lines;
    size_t  chunks;
    ssize_t bytes_read;
    off_t   bytes_to_print;
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

    if(lines_to_print == 0)
    {
        close(fd);
        return 0;
    }

    total_bytes = lseek(fd, 0, SEEK_END);
    if(total_bytes == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    remaining_bytes = total_bytes;
    count_lines = 0;
    bytes_to_print = 0;

    while(count_lines < lines_to_print && remaining_bytes > 0)
    {
        chunks = (remaining_bytes < BUFFER_SIZE 
            ? remaining_bytes : BUFFER_SIZE);
        remaining_bytes -= chunks;
        if(lseek(fd, remaining_bytes, SEEK_SET) == -1)
        {
            perror("lseek");
            close(fd);
            return 1;
        }

        bytes_read = read_all(buffer, chunks, fd);
        if (bytes_read != (ssize_t)chunks)
        {
            fprintf(stderr, "tail: incomplete read\n");
            close(fd);
            return 1;
        }

        for(ssize_t i = bytes_read - 1; i >= 0; i--)
        {
            if(buffer[i] == '\n'
                && remaining_bytes + i != total_bytes - 1)//handle the last line of the file
            {    
                count_lines++;
                if(count_lines == lines_to_print)
                {
                    bytes_to_print = remaining_bytes + i + 1;
                    break;
                }
            }
        }
    }
    if(lseek(fd, bytes_to_print, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write_all(buffer, fd) != 0)
    {
        close(fd);
        return 1;
    }
    if(close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return 0;
}