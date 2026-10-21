#include "cunix.h"

#define BUFFER_SIZE 4096

// Print stdin to stdout.
int print_user(void)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if (ft_write(buffer, (size_t)bytes_read) != 0)
            return 1;
    }
    if (bytes_read < 0)
    {
        perror("read");
        return 1;
    }
    return 0;
}

/* Print file contents without options. */
int print_cat(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int     fd;

    if (file_path == NULL || strcmp(file_path, "-") == 0)
        return print_user();

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        perror(file_path);
        return 1;
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (ft_write(buffer, (size_t)bytes_read) != 0)
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
    if (close(fd) < 0)
    {
        perror("close");
        return 1;
    }
    return 0;
}

/* Write a nonnegative integer to stdout. */
int write_number(size_t n)
{
    char ch;

    if (n >= 10 && write_number(n / 10) != 0)
        return 1;

    ch = (char)('0' + n % 10);
    return ft_write(&ch, 1);
}

/* Print file contents with line numbers. */
int cat_n(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    size_t  capacity;
    size_t  index;
    ssize_t bytes_read;
    size_t  count_lines;
    int     fd;

    if (file_path == NULL || strcmp(file_path, "-") == 0)
        fd = STDIN_FILENO;
    else
    {
        fd = open(file_path, O_RDONLY);
        if (fd < 0)
        {
            perror(file_path);
            return 1;
        }
    }

    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        if (fd != STDIN_FILENO)
            close(fd);
        return 1;
    }

    capacity = BUFFER_SIZE;
    index = 0;
    count_lines = 1;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
            if(index + 1 >= capacity)
            {
                capacity *= 2;
                new_temp = realloc(temp, capacity);
                if(new_temp == NULL)
                {
                    perror("realloc");
                    if (fd != STDIN_FILENO)
                        close(fd);
                    free(temp);
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index + 1] = '\0';
                write(1, "\t", 1);
                write_number(count_lines);
                write(1, "  ", 2);
                if(ft_write(temp, index + 1) != 0)
                {
                    perror("write");
                    if (fd != STDIN_FILENO)
                        close(fd);
                    free(temp);
                    return 1;
                }
                count_lines++;
                index = 0;
            }
            else
                index++;
        }
    }

    if(index > 0)
    {
        temp[index] = '\0';
        write(1, "\t", 1);
        write_number(count_lines);
        write(1, "  ", 2);
        if(ft_write(temp, index) != 0)
        {
            perror("write");
            if (fd != STDIN_FILENO)
                close(fd);
            free(temp);
            return 1;
        }  
    }

    if (bytes_read < 0)
    {
        perror("read");
        free(temp);
        if (fd != STDIN_FILENO)
            close(fd);
        return 1;
    }

    if (fd != STDIN_FILENO && close(fd) < 0)
    {
        perror("close");
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}

// Display '$' after each newline.
int cat_e(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    char    *temp;
    char    *new_temp;
    size_t  capacity;
    size_t  index;
    ssize_t bytes_read;
    int     fd;

    if (file_path == NULL || strcmp(file_path, "-") == 0)
        fd = STDIN_FILENO;
    else
    {
        fd = open(file_path, O_RDONLY);
        if (fd < 0)
        {
            perror(file_path);
            return 1;
        }
    }

    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        if (fd != STDIN_FILENO)
            close(fd);
        return 1;
    }

    capacity = BUFFER_SIZE;
    index = 0;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
            if(index + 1 >= capacity)
            {
                capacity *= 2;
                new_temp = realloc(temp, capacity);
                if(new_temp == NULL)
                {
                    perror("realloc");
                    free(temp);
                    if (fd != STDIN_FILENO)
                        close(fd);
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index + 1] = '\0';
                if(ft_write(temp, index) != 0)
                {
                    perror("write");
                    free(temp);
                    if (fd != STDIN_FILENO)
                        close(fd);
                    return 1;
                }
                write(1, "$\n", 2);
                index = 0;
            }
            else
                index++;
        }
        
    }

    if(index > 0)
    {
        temp[index] = '\0';
        if(ft_write(temp, index) != 0)
        {
            perror("write");
            free(temp);
            if (fd != STDIN_FILENO)
                close(fd);
            return 1;
        }
        write(1, "$\n", 2);
    }

    if (bytes_read < 0)
    {
        perror("read");
        if (fd != STDIN_FILENO)
            close(fd);
        free(temp);
        return 1;
    }

    if (fd != STDIN_FILENO && close(fd) < 0)
    {
        perror("close");
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}

int cat_b(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    size_t  capacity;
    size_t  index;
    ssize_t bytes_read;
    size_t  count_lines;
    int     fd;

    if (file_path == NULL || strcmp(file_path, "-") == 0)
        fd = STDIN_FILENO;
    else
    {
        fd = open(file_path, O_RDONLY);
        if (fd < 0)
        {
            perror(file_path);
            return 1;
        }
    }

    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        if (fd != STDIN_FILENO)
            close(fd);
        return 1;
    }

    capacity = BUFFER_SIZE;
    index = 0;
    count_lines = 1;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
            if(index + 1 >= capacity)
            {
                capacity *= 2;
                new_temp = realloc(temp, capacity);
                if(new_temp == NULL)
                {
                    perror("realloc");
                    if (fd != STDIN_FILENO)
                        close(fd);
                    free(temp);
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index + 1] = '\0';
                if(index == 0)
                {
                    if(ft_write(temp, index + 1) != 0)
                    {
                        perror("write");
                        if (fd != STDIN_FILENO)
                            close(fd);
                        free(temp);
                        return 1;
                    }
                }
                else
                {
                    write(1, "\t", 1);
                    write_number(count_lines);
                    write(1, "  ", 2);
                    if(ft_write(temp, index + 1) != 0)
                    {
                        perror("write");
                        if (fd != STDIN_FILENO)
                            close(fd);
                        free(temp);
                        return 1;
                }
                count_lines++;
                index = 0;
            }
            }
            else
                index++;
        }
    }

    if(index > 0)
    {
        temp[index] = '\0';
        write(1, "\t", 1);
        write_number(count_lines);
        write(1, "  ", 2);
        if(ft_write(temp, index) != 0)
        {
            perror("write");
            if (fd != STDIN_FILENO)
                close(fd);
            free(temp);
            return 1;
        }  
    }

    if (bytes_read < 0)
    {
        perror("read");
        free(temp);
        if (fd != STDIN_FILENO)
            close(fd);
        return 1;
    }

    if (fd != STDIN_FILENO && close(fd) < 0)
    {
        perror("close");
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}

/* Returns 0 on success, 1 on failure. */
int cat(int argc, char **argv)
{
    int status;
    int i;

    if (argc < 1 || argv == NULL)
        return 1;

    if (argc == 1)
        return print_user();

    status = 0;
    if (strcmp(argv[1], "-n") == 0)
    {
        if (argc == 2)
            return cat_n(NULL);

        i = 2;
        while (i < argc)
        {
            if (cat_n(argv[i]) != 0)
                status = 1;
            i++;
        }
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc == 2)
            return cat_e(NULL);

        i = 2;
        while (i < argc)
        {
            if (cat_e(argv[i]) != 0)
                status = 1;
            i++;
        }
    }

    else if (strcmp(argv[1], "-b") == 0)
    {
        if (argc == 2)
            return cat_b(NULL);

        i = 2;
        while (i < argc)
        {
            if (cat_b(argv[i]) != 0)
                status = 1;
            i++;
        }
    }

    else
    {
        i = 1;
        while (i < argc)
        {
            if (print_cat(argv[i]) != 0)
                status = 1;
            i++;
        }
    }
    return status;
}