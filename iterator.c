#include "iterator.h"
#include <stdio.h>
#include <string.h>

// Создание итератора на начало списка
ListIterator list_iterator_begin(CircularList *list) {
    ListIterator it;
    it.list = list;
    it.current_idx = (list && list->head != NIL) ? list->head : NIL;
    it.visited_count = 0;
    it.total_elements = list ? list->count : 0;
    return it;
}

// Проверка валидности позиции итератора
bool iterator_is_valid(const ListIterator *it) {
    return (it != NULL && 
            it->list != NULL && 
            it->current_idx != NIL && 
            it->visited_count < it->total_elements);
}

// Переход к следующему элементу кольцевого списка
void iterator_next(ListIterator *it) {
    if (!iterator_is_valid(it)) {
        return;
    }

    it->visited_count++;
    if (it->visited_count >= it->total_elements) {
        it->current_idx = NIL; // Полный круг завершен
    } else {
        it->current_idx = it->list->nodes[it->current_idx].next;
    }
}

// Получение ключа текущего элемента
const char *iterator_get_key(const ListIterator *it) {
    if (!iterator_is_valid(it)) {
        return NULL;
    }
    return it->list->nodes[it->current_idx].key;
}

// Получение значения текущего элемента
char iterator_get_value(const ListIterator *it) {
    if (!iterator_is_valid(it)) {
        return '\0';
    }
    return it->list->nodes[it->current_idx].value;
}

// Изменение значения текущего элемента
void iterator_set_value(ListIterator *it, char new_val) {
    if (!iterator_is_valid(it)) {
        return;
    }
    it->list->nodes[it->current_idx].value = new_val;
}

// Обмен данными между позициями двух итераторов
void iterator_swap_data(ListIterator *a, ListIterator *b) {
    if (!iterator_is_valid(a) || !iterator_is_valid(b)) {
        return;
    }

    Node *node_a = &a->list->nodes[a->current_idx];
    Node *node_b = &b->list->nodes[b->current_idx];

    // Обмен строковыми ключами
    char temp_key[MAX_KEY_LEN];
    memcpy(temp_key, node_a->key, sizeof(temp_key));
    memcpy(node_a->key, node_b->key, sizeof(node_a->key));
    memcpy(node_b->key, temp_key, sizeof(node_b->key));

    // Обмен значениями
    char temp_val = node_a->value;
    node_a->value = node_b->value;
    node_b->value = temp_val;
}

// Получение физического индекса текущего узла
int iterator_get_current_index(const ListIterator *it) {
    if (!iterator_is_valid(it)) {
        return NIL;
    }
    return it->current_idx;
}

// --- Высокоуровневые алгоритмы через итератор ---

// 1. Печать всех элементов АТД (обход строго через итератор)
void list_print_all(CircularList *list) {
    if (list_is_empty(list)) {
        printf("\n[Инфо] Список пуст.\n\n");
        return;
    }

    printf("\n=== Логический порядок элементов АТД (через итератор) ===\n");
    ListIterator it = list_iterator_begin(list);
    int order = 1;

    while (iterator_is_valid(&it)) {
        printf("  [%2d] Ключ: \"%-15s\" | Значение: '%c'\n",
               order++, iterator_get_key(&it), iterator_get_value(&it));
        iterator_next(&it);
    }
    printf("Всего элементов: %d\n", list_size(list));
    printf("========================================================\n\n");
}

// 2. Поиск значения элемента АТД по ключу (через итератор)
bool list_find_by_key(CircularList *list, const char *key, char *out_val) {
    if (!key || list_is_empty(list)) {
        return false;
    }

    ListIterator it = list_iterator_begin(list);
    while (iterator_is_valid(&it)) {
        if (strcmp(iterator_get_key(&it), key) == 0) {
            if (out_val) {
                *out_val = iterator_get_value(&it);
            }
            return true;
        }
        iterator_next(&it);
    }
    return false;
}

// 3. Сортировка элементов АТД пузырьком (через итераторы)
void list_sort(CircularList *list) {
    int n = list_size(list);
    if (n <= 1) {
        return;
    }

    // Алгоритм пузырьковой сортировки, работающий исключительно через абстракцию итератора
    for (int i = 0; i < n - 1; i++) {
        ListIterator it1 = list_iterator_begin(list);
        ListIterator it2 = it1;
        iterator_next(&it2);

        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(iterator_get_key(&it1), iterator_get_key(&it2)) > 0) {
                iterator_swap_data(&it1, &it2);
            }
            iterator_next(&it1);
            iterator_next(&it2);
        }
    }
}

// 4. Добавление элемента в упорядоченную структуру с сохранением порядка (через итератор)
bool list_insert_sorted(CircularList *list, const char *key, char value) {
    if (!list || !key || list_is_full(list)) {
        return false;
    }

    // Если список пуст — добавляем первый элемент
    if (list_is_empty(list)) {
        return list_push_back(list, key, value);
    }

    // Проверяем, должен ли новый элемент стоять перед текущей головой
    ListIterator first = list_iterator_begin(list);
    if (strcmp(key, iterator_get_key(&first)) < 0) {
        return list_push_front(list, key, value);
    }

    // Ищем позицию для вставки с помощью итераторов prev и curr
    ListIterator prev = list_iterator_begin(list);
    ListIterator curr = prev;
    iterator_next(&curr);

    while (iterator_is_valid(&curr)) {
        if (strcmp(key, iterator_get_key(&curr)) < 0) {
            // Нашли подходящую позицию: между prev и curr
            break;
        }
        iterator_next(&prev);
        iterator_next(&curr);
    }

    // Вставляем новый узел сразу после prev
    return list_insert_after_index(list, iterator_get_current_index(&prev), key, value);
}
