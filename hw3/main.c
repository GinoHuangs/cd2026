#include <stdio.h>
#include <string.h>

#define TOKEN_EOF 0
#define TOKEN_NUM 1
#define TOKEN_PLUS 2
#define TOKEN_LPAREN 3
#define TOKEN_RPAREN 4
#define TOKEN_INVALID 5

typedef struct {
    int type;
    char lexeme[256];
} Token;

static char output_buffer[65536] = {0};
static int output_length = 0;

int is_space(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

static Token current_token;
static int parse_error = 0;

void append_text(const char *text) {
    int text_len;

    if (parse_error) {
        return;
    }

    text_len = (int)strlen(text);
    if (output_length + text_len >= 65535) {
        parse_error = 1;
        return;
    }

    strcpy(output_buffer + output_length, text);
    output_length += text_len;
}

void append_indent(int depth) {
    int i;

    for (i = 0; i < depth; i++) {
        append_text("  ");
    }
}

void append_node(int depth, const char *text) {
    append_indent(depth);
    append_text(text);
    append_text("\n");
}

void next_token(void) {
    int c;
    int idx;

    current_token.lexeme[0] = '\0';

    do {
        c = getchar();
    } while (c != EOF && is_space(c));

    if (c == EOF) {
        current_token.type = TOKEN_EOF;
        strcpy(current_token.lexeme, "EOF");
        return;
    }

    if (is_digit(c)) {
        idx = 0;
        current_token.lexeme[idx++] = (char)c;

        while ((c = getchar()) != EOF && is_digit(c)) {
            if (idx < 255) {
                current_token.lexeme[idx++] = (char)c;
            }
        }

        current_token.lexeme[idx] = '\0';
        current_token.type = TOKEN_NUM;

        if (c != EOF) {
            ungetc(c, stdin);
        }
        return;
    }

    if (c == '+') {
        current_token.type = TOKEN_PLUS;
        strcpy(current_token.lexeme, "+");
        return;
    }

    if (c == '(') {
        current_token.type = TOKEN_LPAREN;
        strcpy(current_token.lexeme, "(");
        return;
    }

    if (c == ')') {
        current_token.type = TOKEN_RPAREN;
        strcpy(current_token.lexeme, ")");
        return;
    }

    current_token.type = TOKEN_INVALID;
    current_token.lexeme[0] = (char)c;
    current_token.lexeme[1] = '\0';
}

void syntax_error(void) {
    parse_error = 1;
}

void parse_S(int depth);
void parse_S_prime(int depth);
void parse_E(int depth);

void parse_S(int depth) {
    if (parse_error) {
        return;
    }

    if (current_token.type == TOKEN_NUM || current_token.type == TOKEN_LPAREN) {
        append_node(depth, "S -> E S'");
        parse_E(depth + 1);
        parse_S_prime(depth + 1);
    } else {
        syntax_error();
    }
}

void parse_S_prime(int depth) {
    if (parse_error) {
        return;
    }

    if (current_token.type == TOKEN_PLUS) {
        append_node(depth, "S' -> + S");
        append_node(depth + 1, "+");
        next_token();
        parse_S(depth + 1);
    } else if (current_token.type == TOKEN_RPAREN || current_token.type == TOKEN_EOF) {
        append_node(depth, "S' -> epsilon");
    } else {
        syntax_error();
    }
}

void parse_E(int depth) {
    char number_line[300];

    if (parse_error) {
        return;
    }

    if (current_token.type == TOKEN_NUM) {
        append_node(depth, "E -> num");
        append_indent(depth + 1);
        strcpy(number_line, current_token.lexeme);
        append_text(number_line);
        append_text("\n");
        next_token();
    } else if (current_token.type == TOKEN_LPAREN) {
        append_node(depth, "E -> ( S )");
        append_node(depth + 1, "(");
        next_token();
        parse_S(depth + 1);
        if (parse_error) {
            return;
        }
        if (current_token.type != TOKEN_RPAREN) {
            syntax_error();
            return;
        }
        append_node(depth + 1, ")");
        next_token();
    } else {
        syntax_error();
    }
}

int main(void) {
    next_token();
    parse_S(0);

    if (!parse_error && current_token.type != TOKEN_EOF) {
        syntax_error();
    }

    if (parse_error) {
        printf("Parse Error\n");
    } else {
        printf("%s", output_buffer);
    }

    return 0;
}
