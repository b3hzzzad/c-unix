#include "cunix.h"

#define BUFFER_SIZE 4096

// Returns 0 on success, 1 on failure.

typedef struct  s_obj
{
    char    *temp;
    char    *flag;
    size_t  line_count;
    size_t  line_count_empty;
    size_t  length;
    int     has_newline;
}   t_obj;

//digit counter helper for cat -n allignment
size_t  get_digit_count(size_t n)
{
    size_t count = 1;
    while (n >= 10)
    {
        n /= 10;
        count++;
    }
    return count;
}

int apply_flag(t_obj *obj)
{
    if (obj->flag == NULL)
    {
        if(ft_write(obj->temp, obj->length + 1) != 0)
            return 1;
        return 0;
    }

    else if (strcmp(obj->flag, "-n") == 0)
    {
        size_t digits = get_digit_count(obj->line_count);

        // Print leading spaces for a 6-wide right-aligned field
        for (size_t i = digits; i < 6; i++)
            write(1, " ", 1);

        write_number(obj->line_count);
        write(1, "\t", 1);

        if(ft_write(obj->temp, obj->length + 1) != 0)
            return 1;
        return 0;
    }
    else if(strcmp(obj->flag, "-e") == 0)
    {
        obj->temp[obj->length] = '$';
        if(ft_write(obj->temp, obj->length + 1) != 0)
            return 1;
        write(1, "\n", 1);
        return 0;
    }
    else if (strcmp(obj->flag, "-b") == 0)
    {
        // Check if the line is non-empty (length > 0 before the newline)
        if (obj->length > 0)
        {
            size_t digits = get_digit_count(obj->line_count_empty);

            for (size_t i = digits; i < 6; i++)
                write(1, " ", 1);

            write_number(obj->line_count_empty);
            write(1, "\t", 1);
        }
        
        if (obj->has_newline)
        {
            if(ft_write(obj->temp, obj->length + 1) != 0)
                return 1;
        }
        else
        {
            if(ft_write(obj->temp, obj->length) != 0)
                return 1;
        }
        return 0;
    }
    return 0;
}

int run_cat(char *flag, char *file_path)
{
    t_obj   *obj;
    char    buffer[BUFFER_SIZE];
    char    *temp;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  capacity;
    size_t  line_count;
    size_t  line_count_empty;
    size_t  index;
    int     fd;

    if (file_path == NULL || strcmp(file_path, "-") == 0)
    {
        fd = STDIN_FILENO;
    }
    else
    {
        fd = open(file_path, O_RDONLY);
        if(fd < 0)
        {
            perror(file_path);
            return 1;
        }
    }

    capacity = BUFFER_SIZE;
    temp = malloc(capacity);
    if(temp == NULL)
    {
        perror("malloc");
        close(fd);
        return 1;
    }

    obj = malloc(sizeof(t_obj));
    if(obj == NULL)
    {
        perror("malloc struct");
        close(fd);
        free(temp);
        return 1;
    }

    line_count = 1;
    line_count_empty = 1;
    index = 0;
    while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
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
                    close(fd);
                    free(temp);
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                if(index > 0)
                {
                    line_count_empty++;
                    obj->has_newline = 1;
                }
                else
                    obj->has_newline = 0;

                temp[index + 1] = '\0';

                obj->temp = temp;
                obj->line_count = line_count;
                obj->line_count_empty = line_count_empty;
                obj->length = index;
                obj->flag = flag;

                apply_flag(obj);

                index = 0;
                line_count++;

            }
            else
                index++;
        }
    }
    if(index > 0)
    {
        //temp[index] = '\0';
        obj->temp = temp;
        obj->line_count = line_count;
        obj->length = index;
        apply_flag(obj);
    }
    if(close(fd) == -1)
    {
        perror("close");
        free(temp);
        free(obj);
        return 1;
    }
    free(temp);
    free(obj);
    return 0;
}

int cat(int ac, char **av)
{
    char *flag = NULL;
    int  start_index = 1;

    if (ac < 1)
        return 1;

    // If no arguments, read from stdin with no flag
    if (ac == 1)
        return run_cat(NULL, NULL);

    // Check if av[1] is a flag (starts with '-' and isn't just '-')
    if (av[1][0] == '-' && av[1][1] != '\0')
    {
        flag = av[1];
        start_index = 2; // Files start from av[2]
    }

    // If there are files to process
    if (start_index < ac)
    {
        for (int i = start_index; i < ac; i++)
            run_cat(flag, av[i]);
    }
    else
    {
        // e.g. "cat -n" with no files provided -> read from stdin
        run_cat(flag, NULL);
    }

    return 0;
}