#include "tree.h"

// Util ===

bool tokenTypeIsOperation(TokenType type) {
    return type >= OPERATION_PLUS;
}

void tokenPrint(Token token) {
    switch (token.type) {
        case INTEGER:
            printf("%d", token.value);
            break;
        case VARIABLE:
            printf("%s", token.name);
            break;
        case OPERATION_PLUS:
            printf("+");
            break;
        case OPERATION_MINUS:
            printf("-");
            break;
        case OPERATION_MULT:
            printf("*");
            break;
        case OPERATION_DIVIDE:
            printf("/");
            break;
    }
}

// ========

// AstNodeTree interface ===

TreeNode *treeNodeCreate() {
    TreeNode *result = malloc(sizeof(TreeNode));
    result->left = NULL;
    result->right = NULL;
    result->finished = false;
    return result;
}

void treeNodeDestroy(TreeNode *node) {
    if (node != NULL) {
        free(node);
    }
}

void treeNodeDestroySubtree(TreeNode *node) {
    if (node != NULL) {
        treeNodeDestroySubtree(node->left);
        treeNodeDestroySubtree(node->right);
        treeNodeDestroy(node);
    }
}

void treeNodePrintSubtree(TreeNode *node, int shift) {
    if (node == NULL) return;
    treeNodePrintSubtree(node->left, shift + TREE_PRINT_SHIFT);
    for (int i = 0;i < shift;i++) putchar(' ');
    tokenPrint(node->token);
    putchar('\n');
    treeNodePrintSubtree(node->right, shift + TREE_PRINT_SHIFT);
}

// =========================

// Tree interface ===

Tree *treeCreate() {
    Tree *tree = malloc(sizeof(Tree));
    tree->root = NULL;
    return tree;
}

void treeDestroy(Tree *tree) {
    if (tree != NULL) {
        treeNodeDestroySubtree(tree->root);
        free(tree);
    }
}

void treePrint(Tree *tree) {
    assert(tree != NULL);
    treeNodePrintSubtree(tree->root, 0);
}

// =====================

// Stack interface ===

TNStackNode *tnStackNodeCreate() {
    TNStackNode *node = malloc(sizeof(TNStackNode));
    node->previous = NULL;
    return node;
}

void tnStackNodeDestroy(TNStackNode *node) {
    if (node != NULL) free(node);
}

TNStack *tnStackCreate() {
    TNStack *stack = malloc(sizeof(TNStack));
    stack->size = 0;
    stack->last = NULL;
    return stack;
}

void tnStackDestroy(TNStack *stack) {
    if (stack != NULL) {
        if (stack->last != NULL) {
            TNStackNode *nextNode = stack->last;
            TNStackNode *currentNode = nextNode->previous;
            while (currentNode != NULL) {
                free(nextNode);
                nextNode = currentNode;
                currentNode = nextNode->previous;
            }
            free(nextNode);
        }
        free(stack);
    }
}

void tnStackPush(TNStack *stack, TreeNode *value) {
    assert(stack != NULL);
    if (stack->last == NULL) {
        TNStackNode *node = tnStackNodeCreate();
        node->value = value;
        stack->last = node;
    } else {
        TNStackNode *node = tnStackNodeCreate();
        node->previous = stack->last;
        node->value = value;
        stack->last = node;
    }
    stack->size++;
}

size_t tnStackSize(TNStack *stack) {
    assert(stack != NULL);
    return stack->size;
}

bool tnStackEmpty(TNStack *stack) {
    assert(stack != NULL);
    return tnStackSize(stack) == 0;
}

TreeNode *tnStackTop(TNStack *stack) {
    assert(stack != NULL);
    assert(!tnStackEmpty(stack));
    return stack->last->value;
}

void tnStackSetTop(TNStack *stack, TreeNode *node) {
    assert(stack != NULL);
    stack->last->value = node;
}

TreeNode *tnStackPop(TNStack *stack) {
    assert(stack != NULL);
    assert(!tnStackEmpty(stack));
    TreeNode *result = stack->last->value;
    if (tnStackSize(stack) == 1) {
        tnStackNodeDestroy(stack->last);
        stack->last = NULL;
    } else {
        TNStackNode *previous = stack->last->previous;
        tnStackNodeDestroy(stack->last);
        stack->last = previous;
    }
    stack->size--;
    return result;
}

// ===================

// Parser ===

int parseUnit(char *unit, Token *resultToken) {
    if (unit[0] == '\0') {
        Token token;
        token.type = BLANK;
        *resultToken = token;
    }
    Token token;
    // May be operation
    // One character token
    if (unit[1] == '\0') {
        switch (unit[0]) {
            case '+':
                token.type = OPERATION_PLUS;
                break;
            case '-':
                token.type = OPERATION_MINUS;
                break;
            case '*':
                token.type = OPERATION_MULT;
                break;
            case '/':
                token.type = OPERATION_DIVIDE;
                break;
            case '(':
                token.type = BRACKETS_OPEN;
                break;
            case ')':
                token.type = BRACKETS_CLOSE;
                break;
            default:
                if (isdigit(unit[0])) {
                    token.type = INTEGER;
                    token.value = atoi(unit);
                    break;
                }
                token.type = VARIABLE;
                strcpy(token.name, unit);
                break;
        }
        *resultToken = token;
        return STATUS_OK;
    }

    // Not one character token
    size_t size = strlen(unit);
    if (size > VARIABLE_MAX_LENGTH) return STATUS_ERROR;

    // May be an integer
    if (isdigit(unit[0]) || unit[0] == '-') {
        for (int i = 1;i < size;i++) {
            if (!isdigit(unit[i])) return STATUS_ERROR;
        }
        token.type = INTEGER;
        token.value = atoi(unit);
        *resultToken = token;
        return STATUS_OK;
    }

    // Then it's a variable
    token.type = VARIABLE;
    strcpy(token.name, unit);
    *resultToken = token;
    return STATUS_OK;
}

// Util
void parseUtilPlaceMostRightNull(TNStackNode *stackNode, TreeNode *placingNode) {
    TreeNode *node = stackNode->value;
    if (node == NULL) {
        stackNode->value = placingNode;
        return ;
    }
    while (node->right != NULL) node = node->right;
    node->right = placingNode;
    placingNode->finished = true;
}

void parseUtilPriorityEmplacement(TNStackNode *stackNode, TreeNode *operationNode) {
    TreeNode *node = stackNode->value;
    if (node->finished) {
        operationNode->left = node;
        operationNode->finished = true;
        stackNode->value = operationNode;
    } else {
        while (!node->right->finished) node = node->right;
        TreeNode *rChild = node->right;
        node->right = operationNode;
        operationNode->left = rChild;
        operationNode->finished = true;
    }
}

void parseUtilUnpriorityEmplacement(TNStackNode *stackNode, TreeNode *operationNode) {
    TreeNode *node = stackNode->value;
    operationNode->left = node;
    stackNode->value = operationNode;
}

int parseFromStdin(Tree **resultTree) {
    assert(resultTree != NULL);
    Tree *tree = treeCreate();
    char str[100];
    Token token;
    TNStack *stack = tnStackCreate();

    tnStackPush(stack, NULL);

    int cnt = 0;
    do {
        scanf("%[^ \n]", str);
        //
        if (parseUnit(str, &token) == STATUS_ERROR) return STATUS_ERROR;

        if (token.type == BRACKETS_OPEN) {
            tnStackPush(stack, NULL);
        } else if (token.type == INTEGER || token.type == VARIABLE) {
            TreeNode *node = tnStackTop(stack);
            if (node == NULL) {
                node = treeNodeCreate();
                node->token = token;
                node->finished = true;
                tnStackPop(stack);
                tnStackPush(stack, node);
            } else {
                TreeNode *newNode = treeNodeCreate();
                newNode->token = token;
                parseUtilPlaceMostRightNull(stack->last, newNode);
            }
        } else if (token.type == OPERATION_PLUS || token.type == OPERATION_MINUS) {
            TreeNode *operationNode = treeNodeCreate();
            operationNode->token = token;
            // operationNode->left = tnStackTop(stack);
            parseUtilUnpriorityEmplacement(stack->last, operationNode);
        } else if (token.type == OPERATION_MULT || token.type == OPERATION_DIVIDE) {
            TreeNode *operationNode = treeNodeCreate();
            operationNode->token = token;
            // operationNode->left = tnStackPop(stack);
            parseUtilPriorityEmplacement(stack->last, operationNode);
        } else if (token.type == BRACKETS_CLOSE) {
            TNStackNode *prev = stack->last->previous;
            parseUtilPlaceMostRightNull(prev, tnStackPop(stack));
            tnStackTop(stack)->finished = true;
        }

    } while (getchar() != '\n');

    tree->root = tnStackPop(stack);
    tnStackDestroy(stack);
    *resultTree = tree;

    return STATUS_OK;
}

// ==========

// Tree to string expression ===

void tokenConvertToStdout(Token token) {
    if (token.type == OPERATION_PLUS) printf("+");
    else if (token.type == OPERATION_MINUS) printf("-");
    else if (token.type == OPERATION_MULT) printf("*");
    else if (token.type == OPERATION_DIVIDE) printf("/");
    else if (token.type == INTEGER) printf("%d", token.value);
    else if (token.type == VARIABLE) printf("%s", token.name);
}

void treeNodeSubtreeConvertToStdout(TreeNode *node) {
    if (node->finished && (node->token.type == OPERATION_PLUS || node->token.type == OPERATION_MINUS)) {
        printf("( ");
        treeNodeSubtreeConvertToStdout(node->left);
        printf(" "); tokenConvertToStdout(node->token); printf(" ");
        treeNodeSubtreeConvertToStdout(node->right);
        printf(" )");
    } else if (tokenTypeIsOperation(node->token.type)) {
        treeNodeSubtreeConvertToStdout(node->left);
        printf(" "); tokenConvertToStdout(node->token); printf(" ");
        treeNodeSubtreeConvertToStdout(node->right);
    } else {
        tokenConvertToStdout(node->token);
    }
}

void treeConvertToStdout(Tree *tree) {
    treeNodeSubtreeConvertToStdout(tree->root);
}

// =============================

// ||======||
// ||      ||
// ||      ||
// ||======||

// Special function:

TreeNode* findUnfinishedScalarSubtree(TreeNode *node) {
    if (node->token.type == INTEGER) return node;
    TreeNode *l = NULL, *r = NULL;
    if (node->right != NULL && (node->right->token.type == INTEGER || !node->right->finished)) l = findUnfinishedScalarSubtree(node->right);
    if (node->left != NULL && (node->left ->token.type == INTEGER || !node->left->finished)) r = findUnfinishedScalarSubtree(node->left);
    if (l != NULL) return l;
    if (r != NULL) return r;
    return NULL;
}

void applyMinusIfCanRightDefined(TreeNode *node, TreeNode *parent, Tree *tree) {
    TreeNode *leftOperand = findUnfinishedScalarSubtree(node->left);
    if (leftOperand == NULL) return;
    bool wasFinished = node->finished;
    leftOperand->token.value -= node->right->token.value;
    if (parent == NULL) {
        tree->root = node->left;
    } else if (parent->left == node) {
        parent->left = node->left;
    } else {
        parent->right = node->left;
    }
    if (wasFinished) node->left->finished = true;
    treeNodeDestroy(node->right);
    treeNodeDestroy(node);
}

void applyMinusIfCanLeftDefined(TreeNode *node, TreeNode *parent, Tree *tree) {
    TreeNode *rightOperand = findUnfinishedScalarSubtree(node->right);
    if (rightOperand != NULL) return;
    bool wasFinished = node->finished;
    rightOperand->token.value = node->left->token.value - rightOperand->token.value;
    if (parent == NULL) {
        tree->root = node->right;
    } else if (parent->right == node) {
        parent->right = node->right;
    } else {
        parent->left = node->right;
    }
    if (wasFinished) node->left->finished = true;
    treeNodeDestroy(node->left);
    treeNodeDestroy(node);
}

void applyMinusSubtree(TreeNode *node, TreeNode *parent, Tree *tree) {
    if (node->left != NULL) applyMinusSubtree(node->left, node, tree);
    if (node->right != NULL) applyMinusSubtree(node->right, node, tree);
    if (node->token.type == OPERATION_MINUS && node->right->token.type == INTEGER) {
        applyMinusIfCanRightDefined(node, parent, tree);
        // printf("{right}");
    }
    else if (node->token.type == OPERATION_MINUS && node->left->token.type == INTEGER) {
        applyMinusIfCanLeftDefined(node, parent, tree);
        // printf("{left}");
    }
    // printf("==PROTO==\n");
    // treeConvertToStdout(tree);
    // printf("==PROTO==\n");
}

void applyMinus(Tree *tree) {
    applyMinusSubtree(tree->root, NULL, tree);
}
