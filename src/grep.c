#include "cunix.h"

#define BUFFER_SIZE 4096

/* Returns 0 on success, 1 on failure. */
int grep_function(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp = NULL;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  index;
    size_t  capacity;
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
                    if (ft_write(temp, index) != 0)
                    {
                        free(temp);
                        close(fd);
                        return 1;
                    }
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
            close(fd);
            free(temp);
            return 1;
        }
    // Last line in the file (if it doesn't end with a newline)
    if(index > 0)
    {
        temp[index + 1] = '\0';  
        if(strstr(temp, to_find) == NULL)
        {
            if (ft_write(temp, index) != 0)
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

int grep_c(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp;
    char    *new_temp;
    ssize_t bytes_read;
    size_t  capacity;
    size_t  count;
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

    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        close(fd);
        return 1;
    }

    count = 0;
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
                if(strstr(temp, to_find) != NULL)
                    count++;
                index = 0;
            }
            else
                index++;
        }
    }
    if(index > 0)
    {
        temp[index + 1] = '\0';
        if(strstr(temp, to_find) != NULL)
            count++;
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

    write_num(count);
    ft_write("\n", 1);

    return 0;
}

//  Exit code 0: A match was found.
//  Exit code 1: No match was found.
//  Exit code >1: An error occurred.
int grep_q(char *file_path, char *to_find)
{
    char    buffer[BUFFER_SIZE];
    char    *temp;
    char    *new_temp;
    size_t  capacity;
    size_t  index;
    ssize_t bytes_read;
    int     fd;

    if(file_path == NULL)
    {
        fprintf(stderr, "invalid file path.\n");
        return 2;
    }
    fd = open(file_path, O_RDONLY);
    if(fd < 0)
    {
        perror(file_path);
        return 2;
    }
    temp = malloc(BUFFER_SIZE);
    if(temp == NULL)
    {
        perror("malloc");
        close(fd);
        return 2;
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
                    free(temp);
                    return 2;
                }
                temp = new_temp;
            }
            temp[index] = buffer[i];
            if(buffer[i] == '\n')
            {
                temp[index] = '\0';
                if(strstr(temp, to_find) != NULL)
                {
                    free(temp);
                    close(fd);
                    return 0; //fount match so its done.
                }
                index = 0;
            }
            else
                index++;
        }
    }
    if(index > 0)
    {
        temp[index] = '\0';
        if(strstr(temp, to_find) != NULL)
        {
            free(temp);
            close(fd);
            return 0; //fount match so its done.
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
    return 1;
}

int grep(int ac, char **av)
{
    if (ac < 3)
    {
        fprintf(stderr, "usage: cunix grep [-n|-v|-c] <pattern> <file...>\n");
        return 1;
    }

    if (strcmp(av[1], "-n") == 0)
    {
        if (ac < 4)
        {
            fprintf(stderr, "grep: missing file argument for -n\n");
            return 1;
        }
        for (int i = 3; i < ac; i++)
            grep_n(av[i], av[2]);
    }
    else if (strcmp(av[1], "-c") == 0)
    {
        if (ac < 4)
        {
            fprintf(stderr, "grep: missing file argument for -c\n");
            return 1;
        }
        for (int i = 3; i < ac; i++)
            grep_c(av[i], av[2]);
    }
    else if (strcmp(av[1], "-v") == 0)
    {
        if (ac < 4)
        {
            fprintf(stderr, "grep: missing file argument for -v\n");
            return 1;
        }
        for (int i = 3; i < ac; i++)
            grep_v(av[i], av[2]);
    }
    else
    {
        for (int i = 2; i < ac; i++)
            grep_function(av[i], av[1]);
    }

    return 0;
}