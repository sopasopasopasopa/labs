#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void print_menu() {
    printf("\nВыберите действие\n");
    printf("0)Выход\n");
    printf("1)Вывести содержимое стека\n");
    printf("2)Узнать размер стека\n");
    printf("3)Добавить элемент\n");
    printf("4)Удалить элемент\n");
    printf("5)Сортировка слияением\n");
    printf("6)Опустошить стек\n");
    printf("\n");
}


int main() {
    stack* s1 = (stack*)malloc(sizeof(stack));
    stack* s2 = (stack*)malloc(sizeof(stack));
    stack_create(s1);
    stack_create(s2);
    int choose = 1;
    int value;
    int size;
    while(choose) {
        print_menu();
        scanf("%d", &choose);
        switch(choose) {
        case 0:
            stack_delete(s1);
            stack_delete(s2);
            free(s1);
            free(s2);
            return 0;
        case 1:
            stack_print(s1);
            stack_delete(s2);
            //size = size_stack(s1);
            //for (int i = 0; i < size; i++) {
            //    printf("%d\t", top_stack(s1)->value);
            //}
            break;
        case 2:
            size = size_stack(s1);
            printf("Размер стека: %d\n", size);
            break;
        case 3:
            printf("Введите значение элемента: ");
            scanf("%d", &value);
            push_top(s1, value);
            break;
        case 4:
            pop_top(s1);
            printf("GDFLDFLG\n");
            break;
        case 5:
            merge_sort(s1);
            stack_print(s1);
            break;
        case 6:
            stack_delete(s1);
            break;
        default:
            printf("Указанного действия не существует");
            break;
        }
    }
    return 0;
}