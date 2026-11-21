#pragma once

#include <stdbool.h>
#include <stddef.h>

// Константы структуры данных
#define LIST_CAPACITY 32      // Максимальная вместимость статического массива
#define MAX_KEY_LEN 64        // Максимальная длина строкового ключа
#define NIL -1                // Индекс, обозначающий отсутствие связи (аналог NULL)

// Узел списка в статическом массиве
typedef struct {
    char key[MAX_KEY_LEN]; // Ключ элемента (строковый)
    char value;            // Значение элемента (символ)
    int next;              // Индекс следующего элемента в массиве
    bool is_used;          // Флаг занятости ячейки
} Node;

// Абстрактный тип данных: Однонаправленный кольцевой список на статическом массиве
typedef struct {
    Node nodes[LIST_CAPACITY]; // Статический пул ячеек памяти
    int head;                  // Индекс первого элемента списка (NIL если пуст)
    int free_head;             // Индекс головы списка свободных ячеек
    int count;                 // Текущее количество элементов в списке
} CircularList;

// --- Базовые операции АТД ---

// Создание (инициализация) пустого объекта
void list_init(CircularList *list);

// Проверка на пустоту
bool list_is_empty(const CircularList *list);

// Проверка на заполненность статического массива
bool list_is_full(const CircularList *list);

// Подсчет числа элементов
int list_size(const CircularList *list);

// Добавление элемента в конец списка
bool list_push_back(CircularList *list, const char *key, char value);

// Добавление элемента в начало списка
bool list_push_front(CircularList *list, const char *key, char value);

// Вставка элемента после узла с заданным индексом
bool list_insert_after_index(CircularList *list, int prev_idx, const char *key, char value);

// Удаление элемента по ключу
bool list_remove_by_key(CircularList *list, const char *key);

// Очистка списка
void list_clear(CircularList *list);

// Печать внутреннего представления (статический массив, индексы, связи, списки)
void list_print_internal(const CircularList *list);

// Управление свободными ячейками статического массива
int list_alloc_node(CircularList *list);
void list_free_node(CircularList *list, int idx);
