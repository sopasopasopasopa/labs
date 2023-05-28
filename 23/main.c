#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void print_menu() {
    printf("1. Добавить вершину\n");
    printf("2. Удалить вершину\n");
    printf("3. Распечатать дерево\n");
    printf("4. Определить уровень дерева с наибольшим количеством вершин\n");
    printf("5. Закончить работу программы\n");
}

int get_max_node_count(Tree* root, int level) {
    if (root == NULL)
        return 0;

    static int max_level = 0;      // Уровень с максимальным количеством вершин
    static int max_node_count = 0;  // Максимальное количество вершин на уровне

    if (level > max_level) {
        max_level = level;
        max_node_count = 1;
    } else if (level == max_level) {
        max_node_count++;
    }

    get_max_node_count(root->left, level + 1);
    get_max_node_count(root->right, level + 1);

    return max_node_count;
}

int max_level_with_max_nodes(Tree* root) {
    if (root == NULL)
        return 0;

    int max_node_count = get_max_node_count(root, 1);

    return max_node_count;
}

int main() {
    Tree* root = NULL;
    int value;
    int choice = 0;

    while (1) {
        print_menu();

        printf("Введите номер команды: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите значение элемента: ");
        		scanf("%d", &value);
       	 		root = insert(root, value);
                break;
            case 2:
                printf("Введите значение элемента для удаления: ");
        		scanf("%d", &value);
        		root = remove_Tree(root, value);
                break;
            case 3:
                printf("\nБинарное дерево: ");
    			printf("\n----------------------------------------\n");
    			print_tree(root, 1);
    			printf("\n----------------------------------------\n");
                break;
            case 4:
                printf("Уровень: %d\n", max_level_with_max_nodes(root));
                break;
            case 5:
                free_tree(root);
    			return 0;
                break;
            default:
                printf("Ошибка! Не существует такого действия\n");
                break;
        }
    }
    free_tree(root);
    
    return 0;
}