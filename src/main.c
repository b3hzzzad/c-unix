#include "cunix.h"

static void print_usage(const char *program)
{
    printf("Usage:\n");
    printf("  %s cat <file>\n", program);
    printf("  %s tail <file>\n", program);
    printf("  %s tail -n <lines> <file>\n", program);
    printf("  %s grep <pattern> <file>\n", program);
    printf("  %s --help\n", program);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        print_usage(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "cat") == 0)
        return cat(argc - 1, argv + 1);
    else if(strcmp(argv[1], "grep") == 0)
        return grep(argc - 1, argv + 1);
    else if(strcmp(argv[1], "tail") == 0)
        return tail(argc - 1, argv + 1);
    
    return 0;
}