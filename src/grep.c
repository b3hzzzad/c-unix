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
int grep_function(char *file_path, char *to_find)
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

//helper function to print numbers
void    write_num(int num)
{
    char    ch;

    if(num >= 10)
        write_num(num / 10);
    ch = (num % 10) + '0';
    write(1, &ch, 1);
}

int grep_n(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    size_t  capacity;
    ssize_t bytes_read;
    size_t  count_lines;
    size_t  index;
    int     fd;

    if(file_path == NULL)
    {
        fprintf(stderr, "invalid file path.\n");
        return 1;
    }
    fd = open(file_path, O_RDONLY);
    if(fd < 0)
    {
        perror(file_path);
        return 1;
    }

    temp = malloc(BUFFER_SIZE * sizeof(char));
    if(temp == NULL)
    {
        perror("malloc");
        close(fd);
        return 1;
    }
    capacity = BUFFER_SIZE;
    count_lines = 0;
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
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                count_lines++;
                temp[index + 1] = '\0';
                if(strstr(temp, to_find) != NULL)
                {
                    write_num(count_lines);
                    ft_write(":", 1);
                    ft_write(temp, index + 1);
                }
                index = 0;
            }
            else
                index++;
        }
    }
    if(bytes_read < 0)
    {
        perror("read");
        free(temp);
        close(fd);
        return 1;
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

int grep_v(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  index;
    size_t  capacity;
    int     fd;

    if(file_path == NULL)
    {
        fprintf(stderr, "invalid file path.\n");
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
    index = 0;
    capacity = BUFFER_SIZE;
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
                    return 1;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index + 1] = '\0';
                if(strstr(temp, to_find) == NULL)
                {
                    ft_write(temp, index + 1);
                }
                index = 0;
            }
            else
                index++;
        }
    }
    // Handle the last line if the file doesn't end with '\n'
    if (index > 0 || (bytes_read == 0 && index == 0 && temp[0] != '\0'))
    {
        temp[index] = '\0'; // Null-terminate at the actual end
        if (strstr(temp, to_find) == NULL)
        {
            ft_write(temp, index);
            ft_write("\n", 1); // Ensure standard newline termination
        }
    }
    if(bytes_read < 0)
    {
        perror("read");
        free(temp);
        close(fd);
        return 1;
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

int grep(int ac, char **av)
{
    if(ac < 0)
        return 0;
    if(ac == 2)
        grep_function(av[2], av[1]);
    else if(ac > 2 && strcmp(av[1], "-n") == 0)
    {
        grep_n(av[3], av[2]);
    }
    else if(ac > 2 && strcmp(av[1], "-v") == 0)
    {
        grep_v(av[3], av[2]);
    }
    else if(ac > 2 && strcmp(av[1], "-n") != 0)
    {
        for(int i = 2; i < ac; i++)
            grep_function(av[i], av[1]);
    }
    
    return 0;
}