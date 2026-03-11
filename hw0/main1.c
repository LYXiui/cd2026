/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義 Linked List 節點結構 [cite: 6]
typedef struct Node {
    char character;
    int count;
    struct Node* next;
} Node;

// 加分題關鍵：建立直接索引表 (Direct Addressing Table) 
// 初始化為 NULL，代表該字元尚未出現過
Node* lookup[256] = {NULL};

int main() {
    // 開啟程式本身 [cite: 5]
    FILE* file = fopen("main.c", "r");
    if (file == NULL) {
        return 1;
    }

    Node* head = NULL;
    Node* tail = NULL;
    int ch;

    // 逐一讀取字元 [cite: 5, 7]
    while ((ch = fgetc(file)) != EOF) {
        unsigned char index = (unsigned char)ch;

        // --- 加分題邏輯：O(1) 尋找 ---
        if (lookup[index] != NULL) {
            // 已存在的字元，直接透過索引找到節點並加一 [cite: 8, 20]
            lookup[index]->count++;
        } else {
            // 新字元，新增 node 進 link-list [cite: 7, 20]
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

            // 更新索引表，下次讀到同字元即可 O(1) 存取
            lookup[index] = newNode;

            // 維持 Linked List 串連 [cite: 6]
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    fclose(file);

    // 輸出結果 [cite: 41]
    Node* current = head;
    while (current != NULL) {
        if (current->character == '\n') printf("\\n:%d\n", current->count);
        else if (current->character == ' ') printf("' ':%d\n", current->count);
        else if (current->character >= 32 && current->character <= 126) {
            printf("%c:%d\n", current->character, current->count);
        } else {
            // 處理非 ASCII 或中文字位元組
            printf("0x%02X:%d\n", (unsigned char)current->character, current->count);
        }
        current = current->next;
    }

    // 釋放記憶體
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}