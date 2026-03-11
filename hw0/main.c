/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義 Linked List 節點結構 
typedef struct Node {
    char character;
    int count;
    struct Node* next;
} Node;

// 全域或主函式內的索引陣列，用於達成 O(1) 尋找 [cite: 57, 58]
// ASCII 共有 256 種可能的數值 (0-255)
Node* lookup[256] = {NULL};

int main() {
    // 1. 開啟程式本身 [cite: 5]
    FILE* file = fopen("main.c", "r");
    if (file == NULL) {
        return 1;
    }

    Node* head = NULL;
    Node* tail = NULL; // 紀錄尾端以維持讀取順序
    int ch;

    // 2. 逐一讀取字元並處理 [cite: 7, 20]
    while ((ch = fgetc(file)) != EOF) {
        unsigned char index = (unsigned char)ch;

        // 加分題實作：利用索引陣列達到 O(1) 尋找 
        if (lookup[index] != NULL) {
            // 若已存在，個數加一 [cite: 8, 20]
            lookup[index]->count++;
        } else {
            // 若為新字元，新增節點進 Linked List [cite: 7, 20]
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

            // 更新索引表
            lookup[index] = newNode;

            // 串連 Linked List
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

    // 3. 輸出結果 [cite: 41, 42]
    Node* current = head;
    while (current != NULL) {
        if (current->character == '\n') {
            printf("\\n:%d\n", current->count);
        } else if (current->character == ' ') {
            printf("' ':%d\n", current->count);
        } else if (current->character >= 32 && current->character <= 126) {
            // 只列印可見的 ASCII 字元
            printf("%c:%d\n", current->character, current->count);
        } else {
            // 針對不可見字元或中文字位元組，顯示十六進位避免亂碼
            printf("0x%02X:%d\n", (unsigned char)current->character, current->count);
        }
        current = current->next;
    }

    // 4. 釋放記憶體
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}