#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Node for Linked List
typedef struct Node {
    char c;
    struct Node *next;
} Node;

int main() {
    FILE *file = fopen(__FILE__, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int ch;

    // Read character by character and append to the Linked List
    while ((ch = fgetc(file)) != EOF) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (newNode == NULL) {
            printf("Memory allocation failed!\n");
            fclose(file);
            return 1;
        }
        newNode->c = (char)ch;
        newNode->next = NULL;
        
        // Append to the tail of the linked list to maintain order
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(file);

    // Output formatting matching the provided image
    printf("Input  -> reading %s\n", __FILE__);
    printf("Output -> \n");
    
    Node *current = head;
    while (current != NULL) {
        // Handle special characters as shown in the image
        if (current->c == '\n') {
            printf("'\\n' ");
        } else if (current->c == '\t') {
            printf("'\\t' ");
        } else if (current->c == '\r') {
            // Handle carriage return for Windows environments silently or explicitly
            printf("'\\r' ");
        } else if (current->c == ' ') {
            // In the image, space is represented as '_'
            printf("'_' "); 
        } else {
            // Normal characters
            printf("'%c' ", current->c);
        }
        current = current->next;
    }
    printf("\n");

    // Free the linked list memory
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}