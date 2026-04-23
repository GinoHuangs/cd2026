#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 自行實作字元判斷，避免違規 include <ctype.h> */
int is_space(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int is_alpha(int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(int c) {
    return (is_alpha(c) || is_digit(c));
}

int main() {
    int c;
    char buffer[256];
    int buf_idx = 0;

    // 不斷讀取字元直到 EOF
    while ((c = getchar()) != EOF) {
        // 1. 略過空白字元 (Whitespace)
        if (is_space(c)) {
            continue;
        }
        // 2. 判斷是否為字母開頭 (Keywords 或 Identifiers)
        else if (is_alpha(c)) {
            buf_idx = 0;
            buffer[buf_idx++] = c;
            int next_c;
            
            // ID 可以包含字母、數字或底線 (_)
            while ((next_c = getchar()) != EOF && (is_alnum(next_c) || next_c == '_')) {
                if (buf_idx < 255) {
                    buffer[buf_idx++] = next_c;
                }
            }
            buffer[buf_idx] = '\0'; // 字串結尾
            
            // 退回多讀的一個字元
            if (next_c != EOF) {
                ungetc(next_c, stdin);
            }

            // 判斷是否為保留字 (Keywords)
            if (strcmp(buffer, "int") == 0) {
                printf("int: TYPE_TOKEN\n");
            } else if (strcmp(buffer, "main") == 0) {
                printf("main: MAIN_TOKEN\n");
            } else if (strcmp(buffer, "if") == 0) {
                printf("if: IF_TOKEN\n");
            } else if (strcmp(buffer, "else") == 0) {
                printf("else: ELSE_TOKEN\n");
            } else if (strcmp(buffer, "while") == 0) {
                printf("while: WHILE_TOKEN\n");
            } else {
                // 否則就是一般 ID
                printf("%s: ID_TOKEN\n", buffer);
            }
        }
        // 3. 判斷是否為數字 (Number / Int Literal)
        else if (is_digit(c)) {
            buf_idx = 0;
            buffer[buf_idx++] = c;
            int next_c;
            
            while ((next_c = getchar()) != EOF && is_digit(next_c)) {
                if (buf_idx < 255) {
                    buffer[buf_idx++] = next_c;
                }
            }
            buffer[buf_idx] = '\0';
            
            if (next_c != EOF) {
                ungetc(next_c, stdin);
            }

            printf("%s: LITERAL_TOKEN\n", buffer);
        }
        // 4. 判斷關係運算子 (Relop) 與 賦值 (=)
        else if (c == '=') {
            int next_c = getchar();
            if (next_c == '=') {
                printf("==: EQUAL_TOKEN\n");
            } else {
                if (next_c != EOF) ungetc(next_c, stdin);
                printf("=: ASSIGN_TOKEN\n");
            }
        }
        else if (c == '>') {
            int next_c = getchar();
            if (next_c == '=') {
                printf(">=: GREATEREQUAL_TOKEN\n");
            } else {
                if (next_c != EOF) ungetc(next_c, stdin);
                printf(">: GREATER_TOKEN\n");
            }
        }
        else if (c == '<') {
            int next_c = getchar();
            if (next_c == '=') {
                printf("<=: LESSEQUAL_TOKEN\n");
            } else {
                if (next_c != EOF) ungetc(next_c, stdin);
                printf("<: LESS_TOKEN\n");
            }
        }
        // 5. 其他符號 (+, -, (, ), {, }, ;)
        else if (c == '+') printf("+: PLUS_TOKEN\n");
        else if (c == '-') printf("-: MINUS_TOKEN\n");
        else if (c == '(') printf("(: LEFTPAREN_TOKEN\n");
        else if (c == ')') printf("): RIGHTPAREN_TOKEN\n"); // 註：投影片誤寫為 REFTPAREN_TOKEN
        else if (c == '{') printf("{: LEFTBRACE_TOKEN\n");
        else if (c == '}') printf("}: RIGHTBRACE_TOKEN\n"); // 註：投影片誤寫為 REFTBRACE_TOKEN
        else if (c == ';') printf(";: SEMICOLON_TOKEN\n");
        else {
            // 遇到無法辨識的字元略過或不做處理，符合題目「只需辨識這個檔案裡的Token」
        }
    }

    return 0;
}