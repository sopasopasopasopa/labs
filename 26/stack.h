#ifndef stack_h
#define stack_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct stack_node {
    int value;
    struct stack_node* prev;
} stack_node;

typedef struct stack {
    int size;
    stack_node* top;
} stack;

bool empty_stack (stack* s);
int size_stack (stack* s);
void push_top (stack* s, int value);
void pop_top (stack* s);
void stack_create (stack* s);
stack_node* top_stack(stack* s);
void stack_delete(stack* s);
void stack_print(stack* s1);
void stack_conc (stack* s1, stack* s2);
void merge(stack* res, stack* s1, stack* s2); 
void merge_sort(stack* s);

#endif