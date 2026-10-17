#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

int write_all(char *buffer, int fd);
ssize_t read_all(char *buffer, ssize_t total_bytes, int fd);

#endif