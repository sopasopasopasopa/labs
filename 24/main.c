#include <stdio.h>

#include "tree.h"

void solve() {
    Tree *tree;
    printf("Введите выражение в инфиксной записи:\n");

    parseFromStdin(&tree);

    printf("Дерево выражения:\n");
    treePrint(tree);
    printf("\n");
    applyMinus(tree);
    printf("Преобразованное выражение:\n");
    treeConvertToStdout(tree);
    printf("\nДерево преобразованного выражения:\n");
    treePrint(tree);
    treeDestroy(tree);
}

int main() {
    solve();
    return 0;
}