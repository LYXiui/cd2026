#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    struct Node* next;
} Node;

int main() {
    FILE* file = fopen("main.c", "r");
    if (file == NULL) {
        printf("無法開啟檔案\n");
        return 1;
    }

    Node* head = NULL;
    Node* tail = NULL;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) break;
        
        newNode->character = (char)ch;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);

    Node* current = head;
    while (current != NULL) {
        if (current->character == '\n') {
            printf("'\\n' ");
        } else if (current->character == '\t') {
            printf("'\\t' ");
        } else if (current->character == '\r') {
            printf("'\\r' ");
        } else if (current->character == ' ') {
            printf("' ' ");
        } else {
            printf("'%c' ", current->character);
        }
        current = current->next;
    }
    printf("\n");

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
