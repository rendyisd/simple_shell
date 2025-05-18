#include "sh.h"

char *get_cmd()
{
    char *buf = NULL;

    write(2, "> ", 2);

    ssize_t length = readline(&buf, stdin);

    if (length == -1) {
        free(buf); // Just in case
        perror("readline");
        return NULL;
    }

    return buf;
}

// TODO: For now, return a vector of string
char **parse_cmd(const char *buf)
{
    char **tokens = tokenize_cmd(buf);

    // Parsing should be done here

    return tokens;
}

// TODO: For now, simplest command execution
int exec_cmd(char **tokens)
{
    char *prog_name = tokens[0];
    char **argv = &tokens[1];

    int prog_pid = fork();

    if (prog_pid < 0) {
        return 1;
    } else if (prog_pid == 0) {
        execvp(prog_name, argv);
    } else {
        wait(NULL);
    }

    return 0;
}

int main()
{
    char *buf = NULL;
    char **tokens = NULL;

    // TODO: Consider handling exit in a different way, for now let it be
    while ((buf = get_cmd()) && strcmp(buf, "exit\n") != 0) {
        tokens = parse_cmd(buf);
        for (char **token = tokens; *token; ++token) {
            printf("|%s|", *token);
        }
        printf("\n");
    }

    //if (exec_cmd(parse_cmd(buf)) == 1) {
    //    printf("Something went wrong!\n");
    //}
}
