#include "cunix.h"

#define BUFFER_SIZE 4096

/* Print stdin to stdout. */
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
int print_cat_n(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t  line_count;
    int     fd;
    int     status;
    int     at_line_start;

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

    line_count = 1;
    at_line_start = 1;
    status = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t i = 0;
        ssize_t start = 0;

        while (i < bytes_read)
        {
            if (at_line_start)
            {
                if (write_number(line_count) != 0
                    || ft_write("\t", 1) != 0)
                {
                    status = 1;
                    break;
                }
                line_count++;
                at_line_start = 0;
            }

            if (buffer[i] == '\n')
            {
                if (ft_write(buffer + start,
                        (size_t)(i - start + 1)) != 0)
                {
                    status = 1;
                    break;
                }
                start = i + 1;
                at_line_start = 1;
            }
            i++;
        }

        if (status != 0)
            break;

        if (start < bytes_read)
        {
            if (ft_write(buffer + start,
                    (size_t)(bytes_read - start)) != 0)
            {
                status = 1;
                break;
            }
        }
    }

    if (bytes_read < 0)
    {
        perror("read");
        status = 1;
    }

    if (fd != STDIN_FILENO && close(fd) < 0)
    {
        perror("close");
        status = 1;
    }

    return status;
}

/* Display '$' before each newline. */
int print_cat_e(char *file_path)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t i;
    ssize_t start;
    int     fd;
    int     status;

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

    status = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        start = 0;
        i = 0;

        while (i < bytes_read)
        {
            if (buffer[i] == '\n')
            {
                if (ft_write(buffer + start,
                        (size_t)(i - start)) != 0
                    || ft_write("$\n", 2) != 0)
                {
                    status = 1;
                    break;
                }
                start = i + 1;
            }
            i++;
        }

        if (status != 0)
            break;

        if (start < bytes_read)
        {
            if (ft_write(buffer + start,
                    (size_t)(bytes_read - start)) != 0)
            {
                status = 1;
                break;
            }
        }
    }

    if (bytes_read < 0)
    {
        perror("read");
        status = 1;
    }

    if (fd != STDIN_FILENO && close(fd) < 0)
    {
        perror("close");
        status = 1;
    }

    return status;
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
            return print_cat_n(NULL);

        i = 2;
        while (i < argc)
        {
            if (print_cat_n(argv[i]) != 0)
                status = 1;
            i++;
        }
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc == 2)
            return print_cat_e(NULL);

        i = 2;
        while (i < argc)
        {
            if (print_cat_e(argv[i]) != 0)
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