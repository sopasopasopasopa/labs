#include "stack.h"

void stack_create(stack *s) {
    s->size = 0;
    s->top = NULL;
}

int size_stack(stack *s) {
    return s->size;
}

bool empty_stack(stack *s) {
    return size_stack(s) == 0;

}

void push_top(stack *s, int value) {
    stack_node *tmp = (stack_node *) malloc(sizeof(stack_node));
    stack_node *last = s->top;
    tmp->value = value;
    tmp->prev = last;
    s->top = tmp;
    s->size++;
}

int top_top(stack *s) {
    return s->top->value;
}


void pop_top(stack *s) {
    int size = size_stack(s);
    if (size == 0) {
        return;
    }
    stack_node *last = s->top;
    stack_node *new_last = last->prev;
    s->top = new_last;
    free(last);
    s->size--;
}

stack_node *top_stack(stack *s) {
    return s->top;
}

void stack_delete(stack *s) {
    int size = size_stack(s);
    if (size == 0) {
        return;
    }
    while (size > 0) {
        stack_node *tmp = top_stack(s);
        s->top = s->top->prev;
        free(tmp);
        size--;
    }
    s->size = 0;
    s->top = NULL;
}

void stack_conc(stack *s1, stack *s2) {
    stack *tmp = (stack *) malloc(sizeof(stack));
    stack_create(tmp);
    int size = size_stack(s2);
    while (size) {
        stack_node *top = top_stack(s2);
        pop_top(s2);
        push_top(tmp, top->value);
        size--;
    }
    stack_delete(s2);
    size = size_stack(tmp);
    while (size) {
        stack_node *top = top_stack(tmp);
        pop_top(tmp);
        push_top(s1, top->value);
        size--;
    }
    free(tmp);
}

void stack_print(stack *s1) {
    int size = size_stack(s1);
    if (size == 0) {
        printf("Стек пуст\n");
        return;
    }
    stack *s2 = (stack*)malloc(sizeof(stack));
    stack_create(s2);
    stack_node *top = top_stack(s1);
    while (size) {
        push_top(s2, top->value);
        top = top->prev;
        size--;
    }
    size = size_stack(s2);
    stack_node *tmp = top_stack(s2);
    printf("(");
    for (int i = 0; i < size; i++) {
        printf("%d ", tmp->value);
        tmp = tmp->prev;
    }
    printf(")\n");
    stack_delete(s2);
    free(s2);

}

void merge(stack *res, stack *s1, stack *s2) {
    stack s3;
    stack_create(&s3);

    while (!empty_stack(s1) && !empty_stack(s2)) {
        if (top_top(s1) > top_top(s2)) {
            push_top(&s3, top_top(s1));
            pop_top(s1);
        } else {
            push_top(&s3, top_top(s2));
            pop_top(s2);
        }
    }
    while (!empty_stack(s1)) {
        push_top(&s3, top_top(s1));
        pop_top(s1);
    }
    while (!empty_stack(s2)) {
        push_top(&s3, top_top(s2));
        pop_top(s2);
    }

    while (!empty_stack(&s3)) {
        push_top(res, top_top(&s3));
        pop_top(&s3);
    }
}

// Сортировка слиянием
void merge_sort(stack *s) {
    if (s->size > 1) {
        stack s2, s3;
        stack_create(&s2);
        stack_create(&s3);
        while (!empty_stack(s)) {
            if (size_stack(s) % 2 == 0) {
                push_top(&s2, top_top(s));
                pop_top(s);
            } else {
                push_top(&s3, top_top(s));
                pop_top(s);
            }
        }
        merge_sort(&s2);
        merge_sort(&s3);
        merge(s, &s2, &s3);
    }
}
