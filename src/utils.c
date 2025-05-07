#include "sh.h"

size_t my_strlen(const char *s)
{
    size_t c = 0;
    while (s && s[c])
        ++c;

    return c;
}

char *my_strchr(const char *s, char c)
{
    size_t i = 0;
    while (s && s[i]) {
        if (s[i] == c)
            return (char *) s+i;

        ++i;
    }
    return NULL;
}

char *my_strdup(const char *s)
{
    size_t len_s = my_strlen(s);
    if (len_s == 0) 
        return NULL;

    char *s_dup = wrap_malloc(len_s * sizeof(char) + 1);
    if (!s_dup)
        return NULL;

    size_t i = 0;
    while (s[i]) {
        s_dup[i] = s[i];
        ++i;
    }
    s_dup[i] = '\0';

    return s_dup;
}

char *my_strndup(const char *s, size_t n)
{
    // Copy at most n byte of s
    size_t i = 0;
    while (s[i] && i < n) 
        ++i;

    char *s_dup = wrap_malloc(i * sizeof(char) + 1);
    if (!s_dup)
        return NULL;

    for (size_t j = 0; j < i; ++j)
        s_dup[j] = s[j];

    s_dup[i] = '\0';

    return s_dup;
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

        size_t tmpbuf_len = my_strlen(tmpbuf);

        if ((tmpbuf_len > 0 && (tmpbuf[tmpbuf_len - 1] == '\n' || tmpbuf[tmpbuf_len - 1] == '\r')) || feof(fp))
            is_newline = 1;

        if (tmpbuf_len > 0) {
            char *new_buf = wrap_realloc(buf, total_buf_len + tmpbuf_len + 1); // +1 null terminator
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

void *wrap_malloc(size_t size)
{
    void *ret = malloc(size);

    if (!ret) {
        perror("wrap_malloc");
        return NULL;
    }

    return ret;
}

void *wrap_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if (!ret) {
        perror("wrap_realloc");
        return NULL;
    }

    return ret;
}
