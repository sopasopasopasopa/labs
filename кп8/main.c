#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void printMenu() { // функция меню
    printf("\n");
    printf("0)Выход\n");
    printf("1)Распечатать список\n");
    printf("2)Добавить элемент в начало\n");
    printf("3)Добавить элемент в конец\n");
    printf("4)Добавить элемент по индексу\n");
    printf("5)Удалить элемент в начале\n");
    printf("6)Удалить элемент в конце\n");
    printf("7)Удалить элемент по индексу\n");
    printf("8)Узнать размер списка\n");
    printf("9)Добавить k экземпляров последнего в начало\n");
    printf("\n");
    printf("Введите номер желаемого действия\n");
}

void addLastKToFront(node** head, int k) {
    if (*head == NULL) {
        printf("Список пуст!\n");
        return;
    }

    // Находим последний элемент списка
    node* current = *head;
    while (current->next != *head) {
        current = current->next;
    }

    // Добавляем k экземпляров последнего элемента в начало списка
    for (int i = 0; i < k; i++) {
        node* newNode = (node*)malloc(sizeof(node));
        newNode->data = current->data;
        newNode->next = (*head)->next;
        (*head)->next = newNode;
    }
}
int main(int argc, const char *argv[]) {
    node* head = NULL; //изначально "голова" = нулю
    int choose = 1;
    char elem; // элемент списка
    int k = 1;
    int size = sizeList(head); //размер списка
    int index = 1;
    while(choose) {
        printMenu(); 
        scanf("%d", &choose);
        switch (choose) {
            case 0:
                printf("\nЗавершение работы...\n");
                destroy(head);
                return 0;
            case 1:
                printList(head);
                break;
            case 2:
                printf("Введите литеру: ");
                getchar();
                scanf("%c", &elem); 
                if(head == NULL) {
                    pushFront(&head, elem);
                }
                pushFront(&head, elem);
                break;
            case 3:
                printf("Введите литеру: ");
                getchar();
                scanf("%c", &elem);
                if(head == NULL) {
                    pushBack(&head, elem);
                }
                pushBack(&head, elem);
                break;
            case 4:
                printf("Введите номер индекса: ");
                scanf("%d", &index);
                printf("Введите литеру: ");
                getchar();
                scanf("%c", &elem); 
                pushIndex(&head, elem, index);
                break;
            case 5:
                deleteFront(&head);
                printf("Удаление завершено!\n");
                printList(head);
                break;
            case 6:
                deleteBack(&head);
                printf("Удаление завершено!\n");
                printList(head);
                break;
            case 7:
                printf("Введите номер индекса: ");
                scanf("%d", &index);
                deleteIndex(&head, index);
                if (size != sizeList(head)) {
                    printf("Удаление завершено!\n");
                }
                printList(head);
                break;
            case 8:
                size = sizeList(head);
                printf("Размер списка: %d\n", size);
                break;
            case 9:
                printf("Введите значение k:");
                scanf("%d", &k);
                addLastKToFront(&head, k);
                printList(head);
                break;
            default:
                printf("Такого действия не существует\n");
                break;
        }
    }
    return 0;
}