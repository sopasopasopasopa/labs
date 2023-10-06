#ifndef list_h
#define list_h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node {
    char data;
    struct node* next;
} node;

void destroy(node* head);
void printList(node* head);
void deleteBack(node** head);
void deleteFront(node** head);
void pushBack(node** head, char data);
void pushFront(node** head, char data);
void deleteIndex(node** head, int index);
void pushIndex(node** head, char data, int index);
int sizeList(node* head);
#endif