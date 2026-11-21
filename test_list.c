#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "circular_list.h"
#include "iterator.h"

// Тест 1: Инициализация и проверка на пустоту
static void test_init_and_empty(void) {
    CircularList list;
    list_init(&list);

    assert(list_is_empty(&list));
    assert(!list_is_full(&list));
    assert(list_size(&list) == 0);
    assert(list.head == NIL);
    assert(list.free_head == 0);

    // Итератор для пустого списка сразу невалиден
    ListIterator it = list_iterator_begin(&list);
    assert(!iterator_is_valid(&it));

    printf("[OK] Тест 1 пройден: Инициализация и проверка пустого списка.\n");
}

// Тест 2: Добавление в конец и кольцевая связь
static void test_push_back_and_circular(void) {
    CircularList list;
    list_init(&list);

    assert(list_push_back(&list, "first", '1'));
    assert(!list_is_empty(&list));
    assert(list_size(&list) == 1);
    // Для 1 элемента next указывает на самого себя
    assert(list.nodes[list.head].next == list.head);

    assert(list_push_back(&list, "second", '2'));
    assert(list_push_back(&list, "third", '3'));
    assert(list_size(&list) == 3);

    // Проверяем кольцевую структуру физически
    int h = list.head;
    int n1 = list.nodes[h].next;
    int n2 = list.nodes[n1].next;
    int n3 = list.nodes[n2].next;
    assert(n3 == h); // Хвост замыкается на голову!

    // Проверяем логический обход через итератор
    ListIterator it = list_iterator_begin(&list);
    assert(iterator_is_valid(&it));
    assert(strcmp(iterator_get_key(&it), "first") == 0);
    assert(iterator_get_value(&it) == '1');

    iterator_next(&it);
    assert(iterator_is_valid(&it));
    assert(strcmp(iterator_get_key(&it), "second") == 0);

    iterator_next(&it);
    assert(iterator_is_valid(&it));
    assert(strcmp(iterator_get_key(&it), "third") == 0);

    iterator_next(&it);
    // После 3 элементов итератор должен завершиться и стать невалидным
    assert(!iterator_is_valid(&it));

    printf("[OK] Тест 2 пройден: Добавление в конец и проверка кольцевой структуры.\n");
}

// Тест 3: Добавление в начало
static void test_push_front(void) {
    CircularList list;
    list_init(&list);

    list_push_front(&list, "c", 'C');
    list_push_front(&list, "b", 'B');
    list_push_front(&list, "a", 'A');

    assert(list_size(&list) == 3);

    ListIterator it = list_iterator_begin(&list);
    assert(strcmp(iterator_get_key(&it), "a") == 0);
    iterator_next(&it);
    assert(strcmp(iterator_get_key(&it), "b") == 0);
    iterator_next(&it);
    assert(strcmp(iterator_get_key(&it), "c") == 0);
    iterator_next(&it);
    assert(!iterator_is_valid(&it));

    printf("[OK] Тест 3 пройден: Добавление элементов в начало списка.\n");
}

// Тест 4: Поиск по ключу через итератор
static void test_find_by_key(void) {
    CircularList list;
    list_init(&list);

    list_push_back(&list, "cat", 'K');
    list_push_back(&list, "dog", 'D');
    list_push_back(&list, "fox", 'F');

    char val = '\0';
    assert(list_find_by_key(&list, "dog", &val));
    assert(val == 'D');

    assert(list_find_by_key(&list, "cat", &val));
    assert(val == 'K');

    assert(list_find_by_key(&list, "fox", &val));
    assert(val == 'F');

    assert(!list_find_by_key(&list, "wolf", &val));

    printf("[OK] Тест 4 пройден: Поиск значения по ключу через итератор.\n");
}

// Тест 5: Сортировка пузырьком через итераторы
static void test_sort(void) {
    CircularList list;
    list_init(&list);

    list_push_back(&list, "orange", 'O');
    list_push_back(&list, "apple", 'A');
    list_push_back(&list, "pear", 'P');
    list_push_back(&list, "banana", 'B');

    list_sort(&list);

    // Ожидаемый порядок: apple, banana, orange, pear
    ListIterator it = list_iterator_begin(&list);
    assert(strcmp(iterator_get_key(&it), "apple") == 0);
    iterator_next(&it);
    assert(strcmp(iterator_get_key(&it), "banana") == 0);
    iterator_next(&it);
    assert(strcmp(iterator_get_key(&it), "orange") == 0);
    iterator_next(&it);
    assert(strcmp(iterator_get_key(&it), "pear") == 0);
    iterator_next(&it);
    assert(!iterator_is_valid(&it));

    printf("[OK] Тест 5 пройден: Пузырьковая сортировка через итераторы.\n");
}

// Тест 6: Вставка с сохранением порядка
static void test_insert_sorted(void) {
    CircularList list;
    list_init(&list);

    assert(list_insert_sorted(&list, "cherry", 'C'));
    assert(list_insert_sorted(&list, "apple", 'A'));   // Новая голова
    assert(list_insert_sorted(&list, "date", 'D'));    // В конец
    assert(list_insert_sorted(&list, "banana", 'B'));  // В середину

    // Ожидаемый порядок: apple, banana, cherry, date
    const char *expected[] = {"apple", "banana", "cherry", "date"};
    char expected_vals[] = {'A', 'B', 'C', 'D'};

    ListIterator it = list_iterator_begin(&list);
    for (int i = 0; i < 4; i++) {
        assert(iterator_is_valid(&it));
        assert(strcmp(iterator_get_key(&it), expected[i]) == 0);
        assert(iterator_get_value(&it) == expected_vals[i]);
        iterator_next(&it);
    }
    assert(!iterator_is_valid(&it));

    printf("[OK] Тест 6 пройден: Вставка с сохранением порядка.\n");
}

// Тест 7: Удаление элементов по ключу (голова, середина, хвост, единственный)
static void test_remove(void) {
    CircularList list;
    list_init(&list);

    list_push_back(&list, "node1", '1');
    list_push_back(&list, "node2", '2');
    list_push_back(&list, "node3", '3');
    list_push_back(&list, "node4", '4');

    // Удаляем из середины ("node2")
    assert(list_remove_by_key(&list, "node2"));
    assert(list_size(&list) == 3);
    assert(!list_find_by_key(&list, "node2", NULL));

    // Удаляем голову ("node1")
    assert(list_remove_by_key(&list, "node1"));
    assert(list_size(&list) == 2);
    ListIterator it = list_iterator_begin(&list);
    assert(strcmp(iterator_get_key(&it), "node3") == 0);

    // Удаляем хвост ("node4")
    assert(list_remove_by_key(&list, "node4"));
    assert(list_size(&list) == 1);
    it = list_iterator_begin(&list);
    assert(strcmp(iterator_get_key(&it), "node3") == 0);

    // Удаляем последний оставшийся элемент
    assert(list_remove_by_key(&list, "node3"));
    assert(list_is_empty(&list));
    assert(list.head == NIL);

    // Удаление из пустого списка
    assert(!list_remove_by_key(&list, "non_existent"));

    printf("[OK] Тест 7 пройден: Удаление элементов (голова, середина, хвост, единственный).\n");
}

// Тест 8: Переполнение статического массива
static void test_capacity(void) {
    CircularList list;
    list_init(&list);

    char key[16];
    for (int i = 0; i < LIST_CAPACITY; i++) {
        snprintf(key, sizeof(key), "k%02d", i);
        assert(list_push_back(&list, key, 'X'));
    }

    assert(list_is_full(&list));
    assert(list_size(&list) == LIST_CAPACITY);

    // Попытка добавить сверх вместимости должна вернуть false
    assert(!list_push_back(&list, "overflow", '!'));
    assert(!list_push_front(&list, "overflow", '!'));
    assert(!list_insert_sorted(&list, "overflow", '!'));

    // Удалим один элемент и проверим, что освободившаяся ячейка переиспользуется
    assert(list_remove_by_key(&list, "k05"));
    assert(!list_is_full(&list));
    assert(list_size(&list) == LIST_CAPACITY - 1);

    assert(list_push_back(&list, "reused", 'R'));
    assert(list_is_full(&list));

    printf("[OK] Тест 8 пройден: Корректная обработка переполнения и переиспользование ячеек.\n");
}

int main(void) {
    printf("================ ЗАПУСК АВТОМАТИЧЕСКИХ ТЕСТОВ ================\n");
    test_init_and_empty();
    test_push_back_and_circular();
    test_push_front();
    test_find_by_key();
    test_sort();
    test_insert_sorted();
    test_remove();
    test_capacity();
    printf("================ ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ! ================\n");
    return 0;
}
