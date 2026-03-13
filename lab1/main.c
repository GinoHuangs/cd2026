#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char c;
    int count;
    struct Node *next;
} Node;

int main() {
    FILE *file = fopen(__FILE__, "r");
    if (file == NULL) {
        printf("無法開啟檔案: %s\n", __FILE__);
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL; // 新增 tail 指標，為了達成 O(1) 的尾端插入
    
    // O(1) 尋找用的指標陣列
    Node *lookup[256] = {NULL}; 

    int ch;
    // Scanner 的第一步：Character Stream Reading
    while ((ch = fgetc(file)) != EOF) {
        unsigned char index = (unsigned char)ch;

        if (lookup[index] != NULL) {
            // 字元已存在，次數加一
            lookup[index]->count++;
        } else {
            // 字元未存在，建立新 Node
            Node *newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("記憶體配置失敗！\n");
                fclose(file);
                return 1;
            }
            newNode->c = (char)index;
            newNode->count = 1;
            newNode->next = NULL;
            
            // 【 Scanner 順序保留】：插入到 Linked List 的尾端
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }

            // 記錄到 lookup table
            lookup[index] = newNode;
        }
    }

    fclose(file);

    // 模擬 Scanner 輸出：從 link-list 把每一個 char 依序輸出
    printf("Input  -> reading %s\n", __FILE__);
    printf("Output -> ");
    
    Node *current = head;
    while (current != NULL) {
        // 為了讓輸出漂亮地呈現 "#, i, n, c..."，針對特殊字元做字串化處理
        if (current->c == '\n') {
            printf("\\n");
        } else if (current->c == '\r') {
            printf("\\r");
        } else if (current->c == '\t') {
            printf("\\t");
        } else if (current->c == ' ') {
            printf("' '"); // 空白用單引號包起來比較清楚
        } else {
            printf("%c", current->c);
        }

        // 如果不是最後一個節點，印出逗號
        if (current->next != NULL) {
            printf(", ");
        }
        
        current = current->next;
    }
    printf("\n");

    // 釋放記憶體
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}