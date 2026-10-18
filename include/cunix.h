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
int cat(const char *file_path);
int tail(char *file_path, size_t lines_to_print);

int grep(char *file_path, char *to_find);

/* Utilities */
int ft_write(char *str, size_t length);

ssize_t read_all(char *buffer, ssize_t total_bytes, int fd);
int write_all(char *buffer, int fd);

#endif