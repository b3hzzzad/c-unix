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

static int run_tail(int argc, char **argv)
{
    char            *end;
    unsigned long   lines;

    if (argc == 3)
        return tail(argv[2], 10);

    if (argc != 5 || strcmp(argv[2], "-n") != 0)
    {
        fprintf(stderr, "Usage: %s tail [-n lines] <file>\n", argv[0]);
        return 1;
    }

    if (argv[3][0] == '-')
    {
        fprintf(stderr, "tail: invalid line count\n");
        return 1;
    }

    errno = 0;
    lines = strtoul(argv[3], &end, 10);

    if (errno == ERANGE || end == argv[3] || *end != '\0'
        || lines > SIZE_MAX)
    {
        fprintf(stderr, "tail: invalid line count\n");
        return 1;
    }

    return tail(argv[4], (size_t)lines);
}

int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        print_usage(argv[0]);
        return 0;
    }

    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "cat") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s cat <file>\n", argv[0]);
            return 1;
        }
        return cat(argv[2]);
    }

    if (strcmp(argv[1], "tail") == 0)
        return run_tail(argc, argv);

    if (strcmp(argv[1], "grep") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr,
                "Usage: %s grep <pattern> <file>\n", argv[0]);
            return 1;
        }
        return grep(argv[3], argv[2]);
    }

    fprintf(stderr, "Unknown command: %s\n", argv[1]);
    print_usage(argv[0]);
    return 1;
}