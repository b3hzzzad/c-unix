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

void    write_number(int n)
{
    char    ch;

    if(n >= 10)
        write_number(n / 10);
    ch = (n % 10) + '0';
    write(1, &ch, 1);
}