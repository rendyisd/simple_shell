// Is this lexer or tokenizer or they're basically the same thing?
#include <ctype.h>
#include "sh.h"

enum token_type {
    TOKEN_STRING,
    TOKEN_PIPE,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_IN,
};


struct token {
    enum token_type type;
    char *value;
    size_t len;
    size_t cap;
};

struct token *token_new()
{
#define TOKEN_HEAP_SIZE 32

    char *value = malloc(TOKEN_HEAP_SIZE);
    struct token *token_buf = malloc(sizeof(struct token));

    token_buf->type = (enum token_type)-1;
    token_buf->value = value;
    token_buf->len = 0;
    token_buf->cap = TOKEN_HEAP_SIZE;

    return token_buf;
}

void token_destroy(struct token *token_buf)
{
    free(token_buf->value);
    free(token_buf);
}

int token_concat(struct token *token_buf, const char *s, size_t len)
{
    size_t avail_mem = token_buf->cap - token_buf->len;

    if (len > avail_mem) {
        char *tmp = realloc(token_buf->value, token_buf->cap + TOKEN_HEAP_SIZE);

        if (!tmp) {
            free(token_buf->value);
            die("Realloc failed");
        }

        token_buf->value = tmp;
        token_buf->cap += TOKEN_HEAP_SIZE;
    }

    for (size_t i = 0; i < len; ++i)
        token_buf->value[token_buf->len + i] = s[i];

    token_buf->len += len;

    return 0;
}

/* TOKENIZER */

#define NUM_STATE 6
enum tokenizer_state {
    STATE_DEFAULT,
    STATE_IN_WORD,
    STATE_IN_SQUOTE,
    STATE_IN_DQUOTE,
    STATE_ESCAPE,
    STATE_END,
};

#define NUM_INPUT_T 5
enum input_type {
    INPUT_ALPHANUM,
    INPUT_WHITESPACE,
    INPUT_QUOTE,
    INPUT_OPERATOR,
    INPUT_ESCAPE,
};

struct tokenizer_ctx {
    struct token **tokens;
    enum tokenizer_state state;
    struct token *curr_token;
};

enum tokenizer_state tf_default_alphanum(char, struct tokenizer_ctx *);

typedef enum tokenizer_state transition_func_t(char, struct tokenizer_ctx *);
transition_func_t * const transition_table[ NUM_STATE ][ NUM_INPUT_T ] = {
    [STATE_DEFAULT][INPUT_ALPHANUM] = tf_default_alphanum,
};

enum tokenizer_state tf_default_alphanum(char c, struct tokenizer_ctx *ctx)
{
    return STATE_IN_WORD;
}

struct token **tokenize(const char *buf)
{
    // TODO: IMPLEMENT DYNAMIC ARRAY OR SOMETHING FFS
    struct token **tokens = malloc(TOKEN_HEAP_SIZE * sizeof(struct token*));
    tokens[0] = NULL;

    struct tokenizer_ctx ctx = {
        tokens,
        STATE_DEFAULT,
        NULL,
    };
    /* TODO: Make state stack(?) */
    size_t i = 0;
    size_t buf_len = strlen(buf);

    while (i < buf_len) {
        char c = buf[i];
        enum input_type type = (enum input_type)-1;

        if (isalpha(c) || '_' == c)
            type = INPUT_ALPHANUM;
        else if (isspace(c))
            type = INPUT_WHITESPACE;
        else if (strchr("\"\'`", c))
            type = INPUT_QUOTE;
        else if (strchr("><|", c)) /* for now let it only be these 3 */
            type = INPUT_OPERATOR;
        else if ('\\' == c)
            type = INPUT_ESCAPE;
        
        if (type == (enum input_type)-1)
            /* TODO: go back to accepting input instead of terminating */
            die("Invalid input encountered");

        enum tokenizer_state new_state = transition_table[ctx.state][type](c, &ctx);
        ctx.state = new_state;

        ++i;
    }

    return tokens;
}
