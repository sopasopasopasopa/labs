#include "table.h"

void createVector(vector* v, int size) {
    v->size = size;
    v->data = (value*)malloc(size * sizeof(value));
    for (int i = 0; i < size; i++) {
        v->data[i].key = 0;
        strcpy(v->data[i].string, "");
    }
}

bool empty(vector* v) {
    return v->size == 0;
}

int sizeVector(vector* v) {
    return v->size;
}

value loadVector(vector* v, int i) {
    value emptyValue;
    emptyValue.key = 0;
    strcpy(emptyValue.string, "");

    if (i >= 0 && i < v->size) {
        return v->data[i];
    } else {
        return emptyValue;
    }
}

void writeVector(vector* v, int i, value val) {
    if (i >= 0 && i < v->size) {
        v->data[i] = val;
    }
}

void resizeVector(vector* v, int size) {
    value* new_values = (value*)realloc(v->data, size * sizeof(value));
    v->data = new_values;
    v->size = size;

    if (v->size > size) {
        for (int i = size; i < v->size; i++) {
            v->data[i].key = 0;
            strcpy(v->data[i].string, "");
        }
    }
}

void destroyVector(vector* v) {
    v->size = 0;
    free(v->data);
    v->data = NULL;
}

void printTable(vector* v) {
    int size = sizeVector(v);
    printf("|============================================|\n");
    printf("|  Ключ        | Значение                          |\n");
    printf("|============================================|\n");
    for (int i = 0; i < size; i++) {
        value tmp = loadVector(v, i);
        printf("| %12lld | %-41s \n", tmp.key, tmp.string);
    }
    printf("|============================================|\n");
}

void swap(value* x, value* y) {
    value temp = *x;
    *x = *y;
    *y = temp;
}


value binarySearch(vector* table, long long int key) {
    long long int left = 0;
    long long int right = sizeVector(table) - 1;
    long long int middle;
    value def = { 0, "" }; //если ничено не найдем, то вернем пустой объект
    while (left <= right) {
        middle = (left + right) / 2;

        if (key < table->data[middle].key) {
            right = middle - 1;
        } else if (key > table->data[middle].key) {
            left = middle + 1;
        } else {
            return table->data[middle];
        }
    }

    return def;

}
void vectorReverse(vector* a, vector* b) { //функция записи реверс вектора
    int size = sizeVector(a);
    int pos = 0;
    for(int i = size - 1; i >= 0; i--) {
        value tmp = loadVector(a, i);
        writeVector(b, pos, tmp);
        pos++;
    }
}

bool checkSortTable(vector* table) { // функция проверки на отсортированность таблицы
    int size = sizeVector(table);
    for (int i = 0; i < size - 1; i++) {
        value a = loadVector(table, i);
        value b = loadVector(table, i + 1);

        if (a.key > b.key) { //если нарушен порядок
            return false;
        }
    }

    return true;
}

bool checkSortTableDescending(vector* table) {// функция проверки на убывание(обратные порядок)
    int size = sizeVector(table);
    for (int i = 0; i < size - 1; i++) {
        value a = loadVector(table, i);
        value b = loadVector(table, i + 1);

        if (a.key < b.key) { // если нарушен порядок
            return false;
        }
    }

    return true;
}

void quickSort(vector* v) {
    int size = sizeVector(v);
    if (size <= 1) {
        return;
    }

    // Создание массива пар индексов для хранения границ подмассивов
    int arr[size];
    int top = -1;
 
    // Инициализация начальных границ
    arr[++top] = 0;
    arr[++top] = size - 1;
 
    // Пока есть подмассивы для сортировки
    while (top >= 0) {
        // Извлечение текущих границ подмассива
        int end = arr[top--];
        int start = arr[top--];
 
        // Установка опорного элемента
        value pivot = loadVector(v, end);
        int i = start - 1;
 
        // Перестановка элементов вокруг опорного элемента
        for (int j = start; j <= end - 1; j++) {
            value current = loadVector(v, j);
            if (current.key < pivot.key) {
                i++;
                swap(&v->data[i], &v->data[j]);
            }
        }
        swap(&v->data[i + 1], &v->data[end]);
        int partitionIndex = i + 1;
 
        // Если есть подмассив слева от опорного элемента,
        // добавляем его в стек
        if (partitionIndex - 1 > start) {
            arr[++top] = start;
            arr[++top] = partitionIndex - 1;
        }
 
        // Если есть подмассив справа от опорного элемента,
        // добавляем его в стек
        if (partitionIndex + 1 < end) {
            arr[++top] = partitionIndex + 1;
            arr[++top] = end;
        }
    }
}