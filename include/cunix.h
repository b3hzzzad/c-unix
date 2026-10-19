#ifndef CUNIX_H
#define CUNIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/* Commands */
int cat(int argc, char **argv);
int ft_tail(int ac, char **av);
int grep(int ac, char **av);

/* Utilities */
int ft_write(char *str, size_t length);

#endif