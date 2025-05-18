#ifndef SH_H
#define SH_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/*
 * utils.c
 */

void die(const char *, ...);
ssize_t readline(char **, FILE *);

/*
 * tokenizer.c
 */
char **tokenize_cmd(const char *);

#endif
