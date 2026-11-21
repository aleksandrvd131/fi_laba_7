#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_list.h"
#include "iterator.h"

// Очистка буфера ввода
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Заполнение демонстрационными данными
static void load_demo_data(CircularList *list) {
    list_clear(list);
    list_push_back(list, "delta", 'D');
    list_push_back(list, "alpha", 'A');
    list_push_back(list, "charlie", 'C');
    list_push_back(list, "bravo", 'B');
    list_push_back(list, "echo", 'E');
    printf("\n[Успех] Демо-данные загружены (5 элементов: delta, alpha, charlie, bravo, echo).\n\n");
}

static void print_menu(void) {
    printf("==================== МЕНЮ АТД ====================\n");
    printf(" 1. Проверить список на пустоту\n");
    printf(" 2. Подсчитать число элементов\n");
    printf(" 3. Добавить элемент в конец\n");
    printf(" 4. Добавить элемент в начало\n");
    printf(" 5. Добавить элемент с сохранением порядка (в упорядоченный список)\n");
    printf(" 6. Удалить элемент по ключу\n");
    printf(" 7. Найти значение по ключу\n");
    printf(" 8. Отсортировать список (пузырьком через итератор)\n");
    printf(" 9. Печать всех элементов АТД (через итератор)\n");
    printf("10. Печать внутреннего представления (статический массив)\n");
    printf("11. Заполнить демонстрационными данными\n");
    printf("12. Очистить список\n");
    printf(" 0. Выход\n");
    printf("==================================================\n");
    printf("Выберите пункт меню: ");
}

int main(void) {
    CircularList list;
    list_init(&list);

    printf("Лабораторная работа: Структуры данных\n");
    printf("Вариант 6: Однонаправленный кольцевой список на статическом массиве\n");
    printf("Тип ключа: строковый, Тип данных: символ\n\n");

    int choice = -1;
    char key_buf[MAX_KEY_LEN];
    char val_buf;

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("\n[Ошибка] Некорректный ввод! Пожалуйста, введите число.\n\n");
            clear_input_buffer();
            continue;
        }

        if (choice == 0) {
            printf("\nЗавершение работы программы. До свидания!\n");
            break;
        }

        switch (choice) {
            case 1: {
                if (list_is_empty(&list)) {
                    printf("\n-> Список ПУСТ.\n\n");
                } else {
                    printf("\n-> Список НЕ пуст (содержит элементов: %d).\n\n", list_size(&list));
                }
                break;
            }

            case 2: {
                printf("\n-> Число элементов в списке: %d (вместимость: %d)\n\n",
                       list_size(&list), LIST_CAPACITY);
                break;
            }

            case 3: {
                printf("Введите строковый ключ (без пробелов, до 63 символов): ");
                if (scanf("%63s", key_buf) != 1) {
                    clear_input_buffer();
                    break;
                }
                printf("Введите символ данных: ");
                clear_input_buffer();
                val_buf = getchar();

                if (list_push_back(&list, key_buf, val_buf)) {
                    printf("\n[Успех] Элемент (\"%s\", '%c') добавлен в конец.\n\n", key_buf, val_buf);
                } else {
                    printf("\n[Ошибка] Не удалось добавить элемент (список полон).\n\n");
                }
                break;
            }

            case 4: {
                printf("Введите строковый ключ (без пробелов, до 63 символов): ");
                if (scanf("%63s", key_buf) != 1) {
                    clear_input_buffer();
                    break;
                }
                printf("Введите символ данных: ");
                clear_input_buffer();
                val_buf = getchar();

                if (list_push_front(&list, key_buf, val_buf)) {
                    printf("\n[Успех] Элемент (\"%s\", '%c') добавлен в начало.\n\n", key_buf, val_buf);
                } else {
                    printf("\n[Ошибка] Не удалось добавить элемент (список полон).\n\n");
                }
                break;
            }

            case 5: {
                printf("Введите строковый ключ (без пробелов, до 63 символов): ");
                if (scanf("%63s", key_buf) != 1) {
                    clear_input_buffer();
                    break;
                }
                printf("Введите символ данных: ");
                clear_input_buffer();
                val_buf = getchar();

                if (list_insert_sorted(&list, key_buf, val_buf)) {
                    printf("\n[Успех] Элемент (\"%s\", '%c') вставлен с сохранением порядка.\n\n",
                           key_buf, val_buf);
                } else {
                    printf("\n[Ошибка] Не удалось вставить элемент (список полон).\n\n");
                }
                break;
            }

            case 6: {
                if (list_is_empty(&list)) {
                    printf("\n[Предупреждение] Список пуст, удалять нечего.\n\n");
                    break;
                }
                printf("Введите строковый ключ для удаления: ");
                if (scanf("%63s", key_buf) != 1) {
                    clear_input_buffer();
                    break;
                }

                if (list_remove_by_key(&list, key_buf)) {
                    printf("\n[Успех] Элемент с ключом \"%s\" успешно удален.\n\n", key_buf);
                } else {
                    printf("\n[Инфо] Элемент с ключом \"%s\" не найден в списке.\n\n", key_buf);
                }
                break;
            }

            case 7: {
                if (list_is_empty(&list)) {
                    printf("\n[Предупреждение] Список пуст.\n\n");
                    break;
                }
                printf("Введите строковый ключ для поиска: ");
                if (scanf("%63s", key_buf) != 1) {
                    clear_input_buffer();
                    break;
                }

                char found_val;
                if (list_find_by_key(&list, key_buf, &found_val)) {
                    printf("\n[Найдено] По ключу \"%s\" найдено значение: '%c'\n\n", key_buf, found_val);
                } else {
                    printf("\n[Инфо] Элемент с ключом \"%s\" не найден.\n\n", key_buf);
                }
                break;
            }

            case 8: {
                if (list_is_empty(&list)) {
                    printf("\n[Предупреждение] Список пуст.\n\n");
                    break;
                }
                list_sort(&list);
                printf("\n[Успех] Список успешно отсортирован пузырьком через итераторы.\n\n");
                list_print_all(&list);
                break;
            }

            case 9: {
                list_print_all(&list);
                break;
            }

            case 10: {
                list_print_internal(&list);
                break;
            }

            case 11: {
                load_demo_data(&list);
                break;
            }

            case 12: {
                list_clear(&list);
                printf("\n[Успех] Список очищен.\n\n");
                break;
            }

            default:
                printf("\n[Ошибка] Неизвестный пункт меню. Попробуйте снова.\n\n");
                break;
        }
    }

    return 0;
}
