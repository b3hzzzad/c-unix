#include "cunix.h"

#define BUFFER_SIZE 4096

//read and handle partail read
ssize_t read_all(int fd, char *buffer, size_t size)
{
    size_t  offset = 0;
    ssize_t bytes_read;

    if (fd < 0 || buffer == NULL)
        return -1;

    while (offset < size)
    {
        bytes_read = read(fd, buffer + offset, size - offset);

        if (bytes_read < 0)
        {
            perror("read");
            return -1;
        }
        if (bytes_read == 0)
            break;//reached end of the file
        offset += bytes_read;
    }

    return (ssize_t)offset;
}

//write and handle partial write
ssize_t write_all(int fd, const char *buffer, size_t size)
{
    size_t  offset = 0;
    ssize_t bytes_written;

    if (fd < 0 || buffer == NULL)
        return -1;

    while (offset < size)
    {
        bytes_written = write(fd, buffer + offset, size - offset);

        if (bytes_written < 0)
        {
            perror("write");
            return -1;
        }
        if (bytes_written == 0)
            return -1;

        offset += bytes_written;
    }

    return (ssize_t)offset;
}

int tail_lseek(char *file_path, int lines)
{
    /*
    1. Open file
    2. Get file size using lseek()
    3. Start from the end
    4. Read backward in chunks
    5. Count newline characters
    6. Find the starting position of the last N lines
    7. Move to that position using lseek()
    8. Read forward and print
    9. Close file
    */

    char    buffer[BUFFER_SIZE];
    ssize_t total_bytes;
    ssize_t bytes_read;
    ssize_t bytes_to_print;
    size_t  chunks;
    size_t  count_lines;
    ssize_t  remaining_bytes;
    int     fd;

    if(file_path == NULL || lines < 0)
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
    if (lines == 0)
    {
        close(fd);
        return 0;
    }
    
    total_bytes = lseek(fd, 0, SEEK_END);
    if(total_bytes == - 1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    remaining_bytes = total_bytes;
    count_lines = 0;
    bytes_to_print = 0;
    while(count_lines < (size_t)lines && remaining_bytes > 0)
    {
        chunks = (remaining_bytes >= BUFFER_SIZE) ? BUFFER_SIZE : remaining_bytes;
        remaining_bytes -= chunks;

        if(lseek(fd, remaining_bytes, SEEK_SET) == -1)
        {
            perror("lseek");
            close(fd);
            return 1;
        };

        //read forward to the end of the chunk
        bytes_read = read_all(fd, buffer, chunks);//use custom read function to handle partial read
        if(bytes_read == -1)
        {
            perror("read");
            close(fd);
            return 1;
        }

        //read the chunks backward
        for(ssize_t i = bytes_read - 1; i >= 0; i--)
        {
            if(buffer[i] == '\n' 
                && remaining_bytes + i != total_bytes - 1)//handle the last line in the file
            {    
                count_lines++;
                //found the starting point to print
                if(count_lines == (size_t)lines)
                {
                    bytes_to_print = remaining_bytes + i + 1;
                    break;
                }
            }
        }
    }

    //skip to the starting point of the line
    if(lseek(fd, bytes_to_print, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    };

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write_all(STDOUT_FILENO, buffer, (size_t)bytes_read) == -1)
        {
            close(fd);
            return 1;
        }
    }

    if (bytes_read < 0)
    {
        perror("read");
        close(fd);
        return 1;
    }

    if(close(fd) == -1)
    {
        perror("close");
        return 1;
    };
    return 0;
}

/* Returns 0 on success, 1 on failure. */
int ft_tail(int ac, char **av)
{
    int i = 1;
    int start;
    int lines = 10;
    int status = 0;

    if (ac < 2)
        return 1;

    if (strcmp(av[i], "-n") == 0)
    {
        if (ac < 4)
            return 1;

        lines = atoi(av[2]);
        i = 3;
    }

    start = i;

    while (i < ac)
    {
        if (ac - start > 1)
        {
            if (i > start)
                write(1, "\n", 1);

            write(1, "==> ", 4);
            write(1, av[i], strlen(av[i]));
            write(1, " <==\n", 5);
        }

        if (tail_lseek(av[i], lines) != 0)
            status = 1;

        i++;
    }

    return status;
}