typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef unsigned long size_t; // 64-bit arch

#define NULL ((void *)0)

int write(int, const void *, int);
int read(int, const void *, int);
void *malloc(size_t); // TODO: Try reimplementing malloc?
void *realloc(void *, size_t); 
void free(void *);

size_t strlen(const char *s)
{
    size_t c = 0;
    while (s[c])
        ++c;

    return c;
}

char *strchr(const char *s, char c)
{
    size_t i = 0;
    while (s[i]) {
        if (s[i] == c)
            return (char *) s+i;

        ++i;
    }
    return NULL;
}

char *strdup(const char *s)
{
    size_t len_s = strlen(s);
    if (len_s == 0) 
        return NULL;

    char *s_dup = malloc(len_s * sizeof(char) + 1);
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

char *strndup(const char *s, size_t n)
{
    // Copy at most n byte of s
    size_t i = 0;
    while (s[i] && i < n) 
        ++i;

    char *s_dup = malloc(i * sizeof(char) + 1);
    if (!s_dup)
        return NULL;

    for (int j = 0; j < i; ++j)
        s_dup[j] = s[j];

    s_dup[i] = '\0';

    return s_dup;
}

int get_cmd(char *buf, size_t count)
{
    write(2, "> ", 2);

    int c = read(0, buf, count);

    buf[c] = '\0';

    return 0;
}

char whitespaces[] = " \t\r\n\v";
char **tokenize_cmd(char *buf)
{ 
    char **argv = 0; // How many strings,  how many characters in each string

    size_t string_counter = 0;
    size_t char_counter = 0;

    char *s_start = 0;
    
    size_t buf_len = strlen(buf);
    for (size_t i = 0; i < buf_len; ++i) {
        if (strchr(whitespaces, buf[i])) {
            if (char_counter != 0) {
                ++string_counter;

                char **temp_argv = realloc(argv, (string_counter) * sizeof(char *));
                char *temp_args = strndup(s_start, char_counter);
                if (!temp_argv || !temp_args) {
                    free(temp_argv);
                    free(temp_args);

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

    char **temp_argv = realloc(argv, (string_counter + 1) * sizeof(char *));
    if (!temp_argv)
        free(temp_argv);
    
    argv = temp_argv;
    argv[string_counter] = 0; // Null terminate argv

    return argv;
}

int parse_cmd(char *buf)
{
    char **tokens = tokenize_cmd(buf);

    while (*tokens) {
        size_t len_s = strlen(*tokens);
        write(1, *tokens, len_s);
        write(1, "\n", 1);
        ++tokens;
    }

    return 0;
}

int main()
{
#define BUF_SIZE 100
    char buf[BUF_SIZE];

    get_cmd(buf, (size_t)BUF_SIZE);
    parse_cmd(buf);
    
    write(1, buf, strlen(buf));
}
