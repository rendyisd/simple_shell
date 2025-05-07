#include "sh.h"

int get_cmd(char **buf)
{
    write(2, "> ", 2);

    ssize_t length = readline(buf, stdin);
    printf("%zd\n", length);

    if (length == -1) {
        perror("readline");
        return -1;
    }

    return 0;
}

char *whitespaces = " \t\r\n\v";
char **tokenize_cmd(const char *buf)
{ 
    char **argv = 0; // How many strings,  how many characters in each string

    size_t string_counter = 0;
    size_t char_counter = 0;

    const char *s_start = 0;
    
    size_t buf_len = my_strlen(buf);
    for (size_t i = 0; i < buf_len; ++i) {
        if (my_strchr(whitespaces, buf[i])) {
            if (char_counter != 0) {
                ++string_counter;

                char **temp_argv = wrap_realloc(argv, (string_counter) * sizeof(char *));
                if (!temp_argv)
                    return NULL;

                char *temp_args = my_strndup(s_start, char_counter);
                if (!temp_args) {
                    free(temp_argv);
                    return NULL;
                }
                
                argv = temp_argv;
                argv[string_counter - 1] = temp_args;

                char_counter = 0;
            }
        } else {
            if (char_counter == 0)
                s_start = &buf[i];

            ++char_counter;
        }
    }

    char **temp_argv = wrap_realloc(argv, (string_counter + 1) * sizeof(char *));
    
    argv = temp_argv;
    argv[string_counter] = 0; // Null terminate argv

    return argv;
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

    get_cmd(&buf);

    printf("%s\n", buf);

    char **tokens = parse_cmd(buf);

    for (char **token = tokens; *token; ++token) {
        printf("| %s |", *token);
    }
    //if (exec_cmd(parse_cmd(buf)) == 1) {
    //    printf("Something went wrong!\n");
    //}
}
