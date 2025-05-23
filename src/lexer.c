#include <ctype.h>
#include "sh.h"

/* 23/05/2025 - I need to rethink my life choice for a bit. Be right back. */

enum token_type {
    TK_WORD, TK_STRING, TK_NUMBER,

    TK_OPEN_PAREN, TK_CLOSE_PAREN, TK_OPEN_CBRACK, TK_CLOSE_CBRACK,
    TK_OPEN_SBRACK, TK_CLOSE_SBRACK, TK_COMMA, TK_SEMICOLON, TK_COLON,
    TK_DOLLAR, TK_BAR, TK_AMPER, 

    TK_PLUS, TK_MINUS, TK_STAR, TK_BANG,

    TK_ASSIGN, TK_GREATER, TK_GEQUAL, TK_LESSER, TK_LEQUAL, TK_EQUAL,
    TK_NOTEQUAL
};

struct token {
    enum token_type type;
    char *value;
    size_t len;
    size_t cap;
};

#define TOKEN_HEAP_SIZE 32

struct token *token_new()
{

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

int token_concat(struct token *token, const char *s, size_t len)
{
    /* 1 is reserved for null terminator */
    size_t avail_mem = token->cap - token->len - 1;

    while (len > avail_mem) {
        char *tmp = realloc(token->value, token->cap + TOKEN_HEAP_SIZE);

        if (!tmp)
            die("token_concat; realloc failed");

        token->value = tmp;
        token->cap += TOKEN_HEAP_SIZE;

        avail_mem = token->cap - token->len - 1;
    }

    memcpy(&token->value[token->len], s, len);

    token->len += len;
    token->value[token->len] = '\0';

    return 0;
}

/* Might need to nuke this, since we are going DFA */
char token_pop(struct token *token)
{
    if (token->len == 0)
        return NULL;

    char c = token->value[token->len - 1];
    token->value[token->len - 1] = '\0';
    --token->len;

    return c;
}

struct lexer {
    struct token **tokens;
    struct token *curr_token;
};

void lexer_flush(struct lexer* lex)
{
    size_t i = 0;
    while (lex->tokens[i])
        ++i;

    struct token **tmp = realloc(lex->tokens, (i + 1 + 1) * sizeof(struct token*));
    if (!tmp)
        die("lexer_flush; realloc failed");
    
    lex->tokens = tmp;

    /* 
     * - Move ownership of curr_token into tokens 
     * - Make a new token for curr_token
     */
    lex->tokens[i] = lex->curr_token;
    lex->tokens[i + 1] = NULL; 
    lex->curr_token = token_new();
}

struct token **tokenize(const char *buf)
{
    struct token **tokens = malloc(1 * sizeof(struct token*));
    tokens[0] = NULL;
    struct lexer lex = {
        tokens,
        NULL,
    };
    size_t i = 0;
    size_t buf_len = strlen(buf);

    while (i < buf_len) {
    }

    if(lex.curr_token)
        token_destroy(lex.curr_token);
    if (!tokens[0])
        free(tokens);

    return tokens;
}
