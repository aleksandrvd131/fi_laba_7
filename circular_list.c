#include "circular_list.h"
#include <stdio.h>
#include <string.h>

// Инициализация пустого списка
void list_init(CircularList *list) {
    if (!list) return;

    list->head = NIL;
    list->count = 0;
    list->free_head = 0;

    // Связываем все ячейки в список свободных узлов
    for (int i = 0; i < LIST_CAPACITY - 1; i++) {
        list->nodes[i].is_used = false;
        list->nodes[i].next = i + 1;
        list->nodes[i].key[0] = '\0';
        list->nodes[i].value = '\0';
    }
    list->nodes[LIST_CAPACITY - 1].is_used = false;
    list->nodes[LIST_CAPACITY - 1].next = NIL;
    list->nodes[LIST_CAPACITY - 1].key[0] = '\0';
    list->nodes[LIST_CAPACITY - 1].value = '\0';
}

// Проверка на пустоту
bool list_is_empty(const CircularList *list) {
    return !list || list->count == 0;
}

// Проверка на заполненность
bool list_is_full(const CircularList *list) {
    return !list || list->count >= LIST_CAPACITY || list->free_head == NIL;
}

// Получение количества элементов
int list_size(const CircularList *list) {
    return list ? list->count : 0;
}

// Выделение свободного узла из статического массива
int list_alloc_node(CircularList *list) {
    if (!list || list->free_head == NIL) {
        return NIL;
    }

    int idx = list->free_head;
    list->free_head = list->nodes[idx].next;
    list->nodes[idx].is_used = true;
    list->nodes[idx].next = NIL;
    return idx;
}

// Возврат узла в список свободных ячеек
void list_free_node(CircularList *list, int idx) {
    if (!list || idx < 0 || idx >= LIST_CAPACITY || !list->nodes[idx].is_used) {
        return;
    }

    list->nodes[idx].is_used = false;
    list->nodes[idx].key[0] = '\0';
    list->nodes[idx].value = '\0';
    list->nodes[idx].next = list->free_head;
    list->free_head = idx;
}

// Добавление элемента в конец кольца
bool list_push_back(CircularList *list, const char *key, char value) {
    if (!list || !key || list_is_full(list)) {
        return false;
    }

    int new_idx = list_alloc_node(list);
    if (new_idx == NIL) {
        return false;
    }

    strncpy(list->nodes[new_idx].key, key, MAX_KEY_LEN - 1);
    list->nodes[new_idx].key[MAX_KEY_LEN - 1] = '\0';
    list->nodes[new_idx].value = value;

    if (list->head == NIL) {
        // Первый элемент кольца указывает сам на себя
        list->head = new_idx;
        list->nodes[new_idx].next = new_idx;
    } else {
        // Ищем хвост кольца (элемент, чей next указывает на head)
        int tail = list->head;
        while (list->nodes[tail].next != list->head) {
            tail = list->nodes[tail].next;
        }
        list->nodes[tail].next = new_idx;
        list->nodes[new_idx].next = list->head;
    }

    list->count++;
    return true;
}

// Добавление элемента в начало кольца
bool list_push_front(CircularList *list, const char *key, char value) {
    if (!list || !key || list_is_full(list)) {
        return false;
    }

    int new_idx = list_alloc_node(list);
    if (new_idx == NIL) {
        return false;
    }

    strncpy(list->nodes[new_idx].key, key, MAX_KEY_LEN - 1);
    list->nodes[new_idx].key[MAX_KEY_LEN - 1] = '\0';
    list->nodes[new_idx].value = value;

    if (list->head == NIL) {
        list->head = new_idx;
        list->nodes[new_idx].next = new_idx;
    } else {
        int tail = list->head;
        while (list->nodes[tail].next != list->head) {
            tail = list->nodes[tail].next;
        }
        list->nodes[new_idx].next = list->head;
        list->nodes[tail].next = new_idx;
        list->head = new_idx;
    }

    list->count++;
    return true;
}

// Вставка элемента после заданного узла по индексу
bool list_insert_after_index(CircularList *list, int prev_idx, const char *key, char value) {
    if (!list || !key || list_is_full(list)) {
        return false;
    }
    if (prev_idx < 0 || prev_idx >= LIST_CAPACITY || !list->nodes[prev_idx].is_used) {
        return false;
    }

    int new_idx = list_alloc_node(list);
    if (new_idx == NIL) {
        return false;
    }

    strncpy(list->nodes[new_idx].key, key, MAX_KEY_LEN - 1);
    list->nodes[new_idx].key[MAX_KEY_LEN - 1] = '\0';
    list->nodes[new_idx].value = value;

    list->nodes[new_idx].next = list->nodes[prev_idx].next;
    list->nodes[prev_idx].next = new_idx;
    list->count++;
    return true;
}

// Удаление элемента по ключу
bool list_remove_by_key(CircularList *list, const char *key) {
    if (!list || !key || list_is_empty(list)) {
        return false;
    }

    int curr = list->head;
    int prev = NIL;
    bool found = false;

    // Поиск узла по ключу с сохранением предыдущего
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->nodes[curr].key, key) == 0) {
            found = true;
            break;
        }
        prev = curr;
        curr = list->nodes[curr].next;
    }

    if (!found) {
        return false;
    }

    if (list->count == 1) {
        // В списке всего 1 элемент
        list_free_node(list, curr);
        list->head = NIL;
    } else if (curr == list->head) {
        // Удаляем голову списка
        int tail = list->head;
        while (list->nodes[tail].next != list->head) {
            tail = list->nodes[tail].next;
        }
        list->head = list->nodes[curr].next;
        list->nodes[tail].next = list->head;
        list_free_node(list, curr);
    } else {
        // Удаляем узел из середины или конца
        list->nodes[prev].next = list->nodes[curr].next;
        list_free_node(list, curr);
    }

    list->count--;
    return true;
}

// Очистка списка
void list_clear(CircularList *list) {
    list_init(list);
}

// Печать внутреннего представления (физический статический массив)
void list_print_internal(const CircularList *list) {
    if (!list) return;

    printf("\n=================== Внутреннее представление АТД ===================\n");
    printf("Вместимость: %d | Занято: %d | Head: %d | Free Head: %d\n",
           LIST_CAPACITY, list->count, list->head, list->free_head);
    printf("--------------------------------------------------------------------\n");
    printf("%-7s | %-8s | %-20s | %-8s | %-6s\n",
           "Индекс", "Занят", "Ключ", "Значение", "Next");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < LIST_CAPACITY; i++) {
        const Node *node = &list->nodes[i];
        if (node->is_used) {
            printf("[%4d]  | %-8s | %-20s | '%c'      | %4d",
                   i, "Да", node->key, node->value, node->next);
            if (i == list->head) {
                printf("  <-- HEAD");
            }
            printf("\n");
        } else {
            printf("[%4d]  | %-8s | %-20s | %-8s | %4d",
                   i, "Нет", "-", "-", node->next);
            if (i == list->free_head) {
                printf("  <-- FREE_HEAD");
            }
            printf("\n");
        }
    }
    printf("====================================================================\n\n");
}
