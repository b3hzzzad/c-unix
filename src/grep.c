#include "cunix.h"

#define BUFFER_SIZE 4096

//allocate memory for temp
char *ft_realloc(char *temp, size_t *current_size)
{
    char    *new_temp;
    size_t  new_size;

    if (*current_size == 0)
        new_size = BUFFER_SIZE;
    else
        new_size = *current_size * 2;

    new_temp = realloc(temp, new_size * sizeof(char));
    if (new_temp == NULL)
        return NULL;
    *current_size = new_size;
    return new_temp;
}

/* Returns 0 on success, 1 on failure. */
int grep(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  line_length;
    size_t  temp_capacity;
    int     fd;
    int     i;

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

    line_length = 0;
    temp_capacity = 0;
    while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        i = 0;
        while(i < bytes_read)
        {
            if(line_length + 1 >= temp_capacity)
            {
                new_temp = ft_realloc(temp, &temp_capacity);
                if(new_temp == NULL)
                {
                    perror("realloc");
                    free(temp);
                    close(fd);
                    return 1;
                }
                temp = new_temp;
            }
            temp[line_length] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[line_length] = '\0';
                if(strstr(temp, to_find))
                {
                    if (ft_write(temp, line_length) != 0 ||
                        ft_write("\n", 1) != 0)
                    {
                        free(temp);
                        close(fd);
                        return 1;
                    }
                }
                line_length = 0;
            }
            else
                line_length++;
            i++;
        }
    }
    if(bytes_read < 0)
        {
            perror("read");
            close(fd);
            free(temp);
            return 1;
        }
    // Last line in the file (if it doesn't end with a newline)
    if(line_length > 0)
    {
        temp[line_length] = '\0';  
        if(strstr(temp, to_find))
        {
            if (ft_write(temp, line_length) != 0 ||
                ft_write("\n", 1) != 0)
            {
                free(temp);
                close(fd);
                return 1;
            }
        }
    }
    if(close(fd) == -1)
    {
        perror("close");
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}