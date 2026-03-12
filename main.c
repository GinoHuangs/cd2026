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
    
    // 【優化核心】：建立一個大小為 256 的指標陣列，全部初始化為 NULL
    // 這裡扮演類似 Hash Table 的角色，用來達成 O(1) 的尋找速度
    Node *lookup[256] = {NULL}; 

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        // 為了安全起見，確保 ch 是非負值 (0~255)
        unsigned char index = (unsigned char)ch;

        // O(1) 尋找：直接查看陣列該索引位置是否有指向 Node
        if (lookup[index] != NULL) {
            // 字元已經在 Linked List 中，直接存取並將次數加一
            lookup[index]->count++;
        } else {
            // 字元不在 Linked List 中，新增一個 Node
            Node *newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("記憶體配置失敗！\n");
                fclose(file);
                return 1;
            }
            newNode->c = (char)index;
            newNode->count = 1;
            
            // 將新 Node 插入 Linked List 的頭部 (維持題目要求)
            newNode->next = head;
            head = newNode;

            // 【關鍵步驟】：將這個新 Node 的位址記錄到陣列中
            lookup[index] = newNode;
        }
    }

    fclose(file);

    // 印出結果 (依然透過走訪 Linked List 來印出，證明串列結構存在)
    printf("===== 原始碼字元統計結果 (O(1) 尋找優化版) =====\n");
    Node *current = head;
    while (current != NULL) {
        if (current->c == '\n') {
            printf("'\\n' (換行) : %d\n", current->count);
        } else if (current->c == '\r') {
            printf("'\\r' (回車) : %d\n", current->count);
        } else if (current->c == '\t') {
            printf("'\\t' (Tab)  : %d\n", current->count);
        } else if (current->c == ' ') {
            printf("' '  (空白) : %d\n", current->count);
        } else {
            printf("'%c'         : %d\n", current->c, current->count);
        }
        current = current->next;
    }

    // 釋放記憶體
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}