#pragma once

#include "circular_list.h"
#include <stdbool.h>

// Абстракция Итератора для однонаправленного кольцевого списка
typedef struct {
    CircularList *list;    // Указатель на структуру списка
    int current_idx;       // Индекс текущего узла в массиве (NIL по завершении обхода)
    int visited_count;     // Количество посещенных элементов
    int total_elements;    // Количество элементов в списке на старте обхода
} ListIterator;

// --- Базовые операции итератора ---

// Создание итератора, указывающего на первый элемент списка
ListIterator list_iterator_begin(CircularList *list);

// Проверка валидности текущей позиции итератора
bool iterator_is_valid(const ListIterator *it);

// Перемещение итератора к следующему элементу
void iterator_next(ListIterator *it);

// Получение ключа текущего элемента
const char *iterator_get_key(const ListIterator *it);

// Получение значения текущего элемента
char iterator_get_value(const ListIterator *it);

// Изменение значения текущего элемента
void iterator_set_value(ListIterator *it, char new_val);

// Обмен данными между двумя позициями итераторов (для сортировки)
void iterator_swap_data(ListIterator *a, ListIterator *b);

// Получение текущего индекса в массиве (для операций вставки)
int iterator_get_current_index(const ListIterator *it);

// --- Операции АТД, реализованные СТРОГО ЧЕРЕЗ ИТЕРАТОР ---

// 1. Печать всех элементов АТД (логический обход по порядку)
void list_print_all(CircularList *list);

// 2. Поиск значения элемента АТД по ключу
bool list_find_by_key(CircularList *list, const char *key, char *out_val);

// 3. Сортировка элементов АТД пузырьком
void list_sort(CircularList *list);

// 4. Добавление элемента в упорядоченную структуру с сохранением порядка
bool list_insert_sorted(CircularList *list, const char *key, char value);
