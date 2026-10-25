#include "cunix.h"

#define BUFFER_SIZE 4096

typedef struct  s_obj
{
    char    *temp;
    char    *flag;
    size_t  length;
    size_t  line_count;
    size_t  match_count;

}   t_obj;

int apply_flag_grep(t_obj *obj, char *to_find)
{
    if(obj->flag == NULL)
    {
        if(strstr(obj->temp, to_find) != NULL)
        {
            if(ft_write(obj->temp, obj->length) != 0)
                return 1;
            write(1, "\n", 1);
        }
        return 0;
    }
    else if(strcmp(obj->flag, "-n") == 0)
    {
        if(strstr(obj->temp, to_find) != NULL)
        {
            write_number(obj->line_count);
            write(1, ":", 1);
            if(ft_write(obj->temp, obj->length) != 0)
                return 1;
            write(1, "\n", 1);
        }
        return 0;
    }
    else if(strcmp(obj->flag, "-v") == 0)
    {
        //if it is not match
        if(strstr(obj->temp, to_find) == NULL)
        {
            if(ft_write(obj->temp, obj->length) != 0)
                return 1;
            write(1, "\n", 1);
        }
        return 0;
    }
    return 0;
}

int run_grep(char *file_path, char *to_find, char *flag)
{
    t_obj   *obj;
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  index;
    size_t  capacity;
    size_t  line_count;
    size_t  match_count;
    int     fd;

    if(file_path == NULL || to_find == NULL)
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

    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        close(fd);
        return 1;
    }

    obj = malloc(sizeof(t_obj));
    if(obj == NULL)
    {
        perror("malloc");
        close(fd);
        free(temp);
        return 1;
    }
    obj->temp = NULL;
    obj->flag = NULL;
    obj->length = 0;
    obj->line_count = 0;
    obj->match_count = 0;

    index = 0;
    capacity = BUFFER_SIZE;
    line_count = 1;
    match_count = 0;
    while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
            if(index + 2 >= capacity)
            {
                capacity *= 2;
                new_temp = realloc(temp, capacity);
                if(new_temp == NULL)
                {
                    perror("realloc");
                    close(fd);
                    free(obj);
                    free(temp);
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index + 1] = '\0';

            //handle -q flag
            if(flag != NULL && strcmp(flag, "-q") == 0)
            {
                if(strstr(temp, to_find) != 0)
                {
                    close(fd);
                    free(temp);
                    free(obj);
                    return 0;
                }
            }

                if(strstr(temp, to_find) != NULL)
                    match_count++;

                obj->temp = temp;
                obj->length = index;
                obj->flag = flag;
                obj->line_count = line_count;
                obj->match_count = match_count;

                if(apply_flag_grep(obj, to_find) != 0)
                {
                    perror("apply_grep");
                    close(fd);
                    free(obj);
                    free(temp);
                    return 1;  
                }

                index = 0;
                line_count++;
            }
            else
                index++;
        }
    }
    if(bytes_read < 0)
        {
            perror("read");
            close(fd);
            free(temp);
            free(obj);
            return 1;
        }

    // Last line in the file (if it doesn't end with a newline)
    if(index > 0)
    {
        //handle -q flag
        if(flag != NULL && strcmp(flag, "-q") == 0)
        {
            if(strstr(temp, to_find) != 0)
            {
                close(fd);
                free(temp);
                free(obj);
                return 0;
            }
        }

        if(strstr(temp, to_find) != NULL)
            match_count++;
        temp[index] = '\0';  
        obj->temp = temp;
        obj->length = index;
        obj->flag = flag;
        obj->line_count = line_count;
        obj->match_count = match_count;
        apply_flag_grep(obj, to_find);
    }

    //handling grep -c flag
    if (flag != NULL && strcmp(flag, "-c") == 0)
    {
        write_number(obj->match_count);
        write(1, "\n", 1);
    }

    // If -q finishes the whole file without returning 0, it means no match was found
    if (flag != NULL && strcmp(flag, "-q") == 0)
    {
        close(fd);
        free(temp);
        free(obj);
        return 1;
    }

    if(close(fd) == -1)
    {
        perror("close");
        free(obj->temp);
        free(obj);
        return 1;
    }

    free(obj->temp);
    free(obj);
    return 0;
}


int grep(int ac, char **av)
{
    char    *flag = NULL;
    char    *to_find;
    int     start_index;
    int     status = 0;

    if (ac < 3)
    {
        fprintf(stderr, "usage: grep [flag] pattern [file ...]\n");
        return 1;
    }

    // Check if av[1] is a flag
    if (av[1][0] == '-' && av[1][1] != '\0')
    {
        flag = av[1];
        to_find = av[2];
        start_index = 3;
    }
    else
    {
        to_find = av[1];
        start_index = 2;
    }

    // If files are provided, iterate through them
    if (start_index < ac)
    {
        for (int i = start_index; i < ac; i++)
        {
            if(run_grep(av[i], to_find, flag) != 0)
                status = 1;
        }
        return status;
    }

    return 0;
}