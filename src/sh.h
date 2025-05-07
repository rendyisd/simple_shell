#ifndef SH_H
#define SH_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// Reimplemented these cause why not
size_t my_strlen(const char *);
char *my_strchr(const char *, char);
char *my_strdup(const char *);
char *my_strndup(const char *s, size_t n);
ssize_t readline(char **, FILE *);

// Error logging wrappers
void *wrap_malloc(size_t size);
void *wrap_realloc(void *ptr, size_t size);

#endif
