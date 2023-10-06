#include "list.h"

void destroy(node* head) {
    if (head == NULL) {
        return;
    }

    node* current = head->next;
    while (current != head) {
        node* temp = current;
        current = current->next;
        free(temp);
    }

    free(head);
}

void printList(node* head) {
    if (head == NULL) {
        return;
    }

    node* current = head->next;
    while (current != head) {
        printf("%c ", current->data);
        current = current->next;
    }
    printf("\n");
}

void deleteBack(node** head) {
    if (*head == NULL || (*head)->next == *head) {
        return;
    }

    node* current = *head;
    while (current->next->next != *head) {
        current = current->next;
    }

    node* temp = current->next;
    current->next = *head;
    free(temp);
}

void deleteFront(node** head) {
    if (*head == NULL || (*head)->next == *head) {
        return;
    }

    node* temp = (*head)->next;
    (*head)->next = temp->next;
    free(temp);
}

void pushFront(node** head, char data) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->data = data;

    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode; // Устанавливаем указатель next на себя
    } else {
        newNode->next = (*head)->next;
        (*head)->next = newNode;
    }
}

void pushBack(node** head, char data) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->data = data;

    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode; // Устанавливаем указатель next на себя
    } else {
        node* current = *head;
        while (current->next != *head) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

int sizeList(node* head) {
    if (head == NULL) {
        return 0;
    }

    int count = 0;
    node* current = head->next;
    while (current != head) {
        count++;
        current = current->next;
    }

    return count;
}

void deleteIndex(node** head, int index) {
    int size = sizeList(*head);
    if (*head == NULL || index < 0 || index > size) {
        return;
    }

    if (index == 0) {
        deleteFront(head);
        return;
    }

    node* current = *head;
    int count = 0;
    while (count < index - 1) {
        current = current->next;
        count++;
    }

    if (current->next == *head) {
        return;
    }

    node* temp = current->next;
    current->next = temp->next;
    free(temp);
}

void pushIndex(node** head, char data, int index) {
    int size = sizeList(*head);
    if (index < 0 || index > size) {
        return;
    }

    if (index == 0) {
        pushFront(head, data);
        return;
    }

    node* newNode = (node*)malloc(sizeof(node));
    newNode->data = data;

    node* current = *head;
    int count = 0;
    while (count < index - 1) {
        current = current->next;
        count++;
    }

    newNode->next = current->next;
    current->next = newNode;
}
