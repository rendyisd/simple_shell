#include <stdarg.h>
#include "sh.h"

void die(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

ssize_t readline(char **line_buf, FILE *fp)
{
#define READ_CHUNK 64
    char *buf = NULL;
    char tmpbuf[READ_CHUNK];
    ssize_t total_buf_len = 0;
    unsigned short int is_newline = 0;

    while (is_newline == 0) {
        char *temp = fgets(tmpbuf, READ_CHUNK, fp);
        if (!temp) {
            *line_buf = NULL;
            return -1;
        }

        size_t tmpbuf_len = strlen(tmpbuf);

        if ((tmpbuf_len > 0 && (tmpbuf[tmpbuf_len - 1] == '\n' || tmpbuf[tmpbuf_len - 1] == '\r')) || feof(fp))
            is_newline = 1;

        if (tmpbuf_len > 0) {
            char *new_buf = realloc(buf, total_buf_len + tmpbuf_len + 1); // +1 null terminator
            if (!new_buf) {
                free(buf);
                *line_buf = NULL;
                return -1;
            }
            buf = new_buf;
            memcpy(buf + total_buf_len, tmpbuf, tmpbuf_len);
            total_buf_len += tmpbuf_len;
        }
    }

    if (buf)
        buf[total_buf_len] = '\0';

    *line_buf = buf;

    return total_buf_len;
}

/* Separate null terminated string on whitespaces
 * Currently not needed, just in case
char **tokenize_cmd(const char *buf)
{ 
    char **argv = 0; // How many strings,  how many characters in each string

    size_t string_counter = 0;
    size_t char_counter = 0;

    const char *s_start = 0;
    
    size_t buf_len = strlen(buf);
    for (size_t i = 0; i < buf_len; ++i) {
        if (strchr(whitespaces, buf[i])) {
            if (char_counter != 0) {
                ++string_counter;

                char **temp_argv = wrap_realloc(argv, (string_counter) * sizeof(char *));
                if (!temp_argv)
                    return NULL;

                char *temp_args = strndup(s_start, char_counter);
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

*/
