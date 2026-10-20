#include "cunix.h"

#define BUFFER_SIZE 4096

//printf the STDIN_FILENO input
int print_user(void)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while(1)
    {
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes_read < 0)
        {
            perror("read");
            return 1;
        }
        if (bytes_read == 0)
            break;
        if (ft_write(buffer, (size_t)bytes_read) != 0)
            return 1;
    }
    return 0;
}

//cat function without options
int print_cat(char *file_path)
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;

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

//helper function to write() integers
void    write_number(int n)
{
    char    ch;

    if(n >= 10)
        write_number(n / 10);
    ch = (n % 10) + '0';
    write(1, &ch, 1);
}

int print_cat_n(char *file_path)
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    size_t  line_length;
    size_t  temp_length;
    size_t  new_capacity;
    ssize_t bytes_read;
    size_t  line_count;

    if(file_path == NULL || strcmp(file_path, "-") == 0)
        fd = STDIN_FILENO;
    else
    {
        fd = open(file_path, O_RDONLY);
        if(fd < 0)
        {
            perror(file_path);
            return 1;
        }
    }
    line_length = 0;
    temp_length = 0;
    line_count = 1;
    while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
            if(line_length + 1 >= temp_length)
            {
                new_capacity = (temp_length == 0) ? BUFFER_SIZE : temp_length * 2;
                new_temp = realloc(temp, new_capacity);

                if (new_temp == NULL)
                {
                    perror("realloc");
                    free(temp);
                    return 1;
                }
                temp = new_temp;
                temp_length = new_capacity;
            }
            temp[line_length] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[line_length] = '\0';
                write_number(line_count);
                write(1, "\t", 1);
                if(ft_write(temp, line_length) != 0)
                {
                    perror("write");
                    free(temp);
                    close(fd);
                    return 1;
                }
                write(1, "\n", 1);
                line_count++;
                line_length = 0;
            }
            else
                line_length++;
        }
    }
    if(bytes_read < 0)
    {
        perror("read");
        free(temp);
        return 1;
    }
    //handle the last line in file
    if(line_length > 0)
    {
        temp[line_length] = '\0';
        write_number(line_count);
        write(1, "\t", 1);
        if(ft_write(temp, line_length) != 0)
        {
            perror("write");
            close(fd);
            return 1;
        }
        write(1, "\n", 1);
    }
    //never close SDTIN
    if(fd != STDIN_FILENO && close(fd) == -1)
    {
        perror("close");
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}

int print_cat_e (char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t i;
    ssize_t start;
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

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        i = 0;
        start = 0;

        while (i < bytes_read)
            {
                if (buffer[i] == '\n')
                {
                    if (i > start)
                        write(1,buffer + start, (size_t)(i - start));
                    write(1, "$\n", 2);
                    start = i + 1;
                }
                i++;
            }
    }
    if(bytes_read < 0)
    {
        perror("read");
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

/* Returns 0 on success, 1 on failure. */
int cat(int argc, char **argv)
{
    if(argc < 0)
        return 0;

    if(argc == 1)
        return print_user();

    if(strcmp(argv[1], "-n") == 0)
    {
        if(argc == 2)
            return print_cat_n(NULL);
        for(int i = 2; i < argc; i++)
            print_cat_n(argv[i]);
    }
    else if(strcmp(argv[1], "-e") == 0)
    {
        for(int i = 2; i < argc; i++)
            print_cat_e(argv[i]);
    }
    else
    {
        for(int i = 1; i < argc; i++)
            print_cat(argv[i]);
    }
    return 0;
}