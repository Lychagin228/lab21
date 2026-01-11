#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#define MAX_SIZE 100
#define FILENAME "cornices.txt"
#define FILENAME_NEW "cornices_new.txt"


typedef struct Cornice {

    int length;
    int ceilingWidth;
    int wallHeight;
    char material[20];

} cornice_t;

int getRandomNumber(int min, int max);
void getRandomString(char* str, int max_len);
void fill_array_random(cornice_t* cornices, int size);
void fill_array_const(cornice_t* cornices, int* size);
void print_cornice(cornice_t cornice);
void print_array(cornice_t* cornices, int size);
cornice_t* search_material(cornice_t* cornices, int size, char* material);
cornice_t* search_length(cornice_t* cornices, int size, int length);
cornice_t* search_width(cornice_t* cornices, int size, int width);
int compare_area(const void* a, const void* b);
int compare_length(const void* a, const void* b);
int compare_width(const void* a, const void* b);
int compare_height(const void* a, const void* b);
int compare_material(const void* a, const void* b);
void sort_array(cornice_t* cornices, int size, int field);
int output_file(char* filename, cornice_t* arr, int n);
int input_file(char* filename, cornice_t* arr, int max_size);
void edit_record(cornice_t* cornices, int size);
void search_record(cornice_t* cornices, int size);
void display_menu();



int main() {

    setlocale(LC_ALL, "RUS");

    cornice_t cornices[MAX_SIZE];
    int size = 0;
    int choice;

    printf("=== ПРОГРАММА УЧЕТА СТРОИТЕЛЬНЫХ МАТЕРИАЛОВ ===\n");

    do {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Введите количество записей (макс. %d): ", MAX_SIZE);
            scanf("%d", &size);
            if (size > 0 && size <= MAX_SIZE) {
                fill_array_random(cornices, size);
            }
            else {
                printf("Неверное количество записей.\n");
            }
            break;

        case 2:
            fill_array_const(cornices, &size);
            break;

        case 3:
            print_array(cornices, size);
            break;

        case 4:
            edit_record(cornices, size);
            break;

        case 5:
            search_record(cornices, size);
            break;

        case 6:
            if (size == 0) {
                printf("Массив пуст. Нечего сортировать.\n");
                break;
            }
            printf("\nВыберите поле для сортировки:\n");
            printf("1. Длина\n");
            printf("2. Ширина\n");
            printf("3. Высота\n");
            printf("4. Площадь (длина * ширина)\n");
            printf("5. Материал\n");
            printf("Ваш выбор: ");
            int field;
            scanf("%d", &field);
            sort_array(cornices, size, field);
            break;

        case 7:
            if (size > 0) {
                output_file(FILENAME, cornices, size);
            }
            else {
                printf("Массив пуст. Нет данных для сохранения.\n");
            }
            break;

        case 8:
            size = input_file(FILENAME, cornices, MAX_SIZE);
            break;

        case 9:
            if (size > 0) {
                output_file(FILENAME_NEW, cornices, size);
            }
            else {
                printf("Массив пуст. Нет данных для сохранения.\n");
            }
            break;

        case 0:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 0);

    return 0;

}


int getRandomNumber(int min, int max) {

    return rand() % (max - min + 1) + min;
}


void getRandomString(char* str, int max_len) {

    const char* materials[] = { "Дерево", "Алюминий", "Пластик", "Сталь", "Металл", "Композит", "ДСП", "МДФ", "Полиуретан", "Пенопласт" };
    int count = sizeof(materials) / sizeof(materials[0]);
    strcpy(str, materials[rand() % count]);

}


void fill_array_random(cornice_t* cornices, int size) {

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        cornices[i].length = getRandomNumber(1, 10) * 100;
        cornices[i].ceilingWidth = getRandomNumber(1, 5) * 10;
        cornices[i].wallHeight = getRandomNumber(2, 8) * 10;
        getRandomString(cornices[i].material, 20);
    }
    printf("Массив заполнен случайными значениями.\n");

}


void fill_array_const(cornice_t* cornices, int* size) {

    cornice_t const_cornices[] = {
        {2000, 50, 50, "Композит"},
        {1500, 40, 40, "Дерево"},
        {1800, 45, 35, "Алюминий"},
        {2200, 60, 50, "Пластик"},
        {1700, 35, 45, "Сталь"}
    };

    int const_size = sizeof(const_cornices) / sizeof(const_cornices[0]);
    for (int i = 0; i < const_size; i++) {
        cornices[i] = const_cornices[i];
    }
    *size = const_size;
    printf("Массив заполнен константными значениями.\n");

}


void print_cornice(cornice_t cornice) {

    printf("| %4d мм | %3d мм | %3d мм | %-15s |\n",
        cornice.length, cornice.ceilingWidth,
        cornice.wallHeight, cornice.material);

}



void print_array(cornice_t* cornices, int size) {

    if (size == 0) {
        printf("Массив пуст.\n");
        return;
    }

    printf("\n===============================================\n");
    printf("| Длина | Ширина | Высота | Материал        |\n");
    printf("===============================================\n");
    for (int i = 0; i < size; i++) {
        print_cornice(cornices[i]);
    }
    printf("===============================================\n");
    printf("Всего записей: %d\n", size);

}


cornice_t* search_material(cornice_t* cornices, int size, char* material) {

    for (int i = 0; i < size; i++) {
        if (strcmp(cornices[i].material, material) == 0)
            return (cornices + i);
    }
    return NULL;

}


cornice_t* search_length(cornice_t* cornices, int size, int length) {

    for (int i = 0; i < size; i++) {
        if (cornices[i].length == length)
            return (cornices + i);
    }
    return NULL;

}


cornice_t* search_width(cornice_t* cornices, int size, int width) {

    for (int i = 0; i < size; i++) {
        if (cornices[i].ceilingWidth == width)
            return (cornices + i);
    }
    return NULL;

}


int compare_area(const void* a, const void* b) {

    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    int areaA = corniceA->length * corniceA->ceilingWidth;
    int areaB = corniceB->length * corniceB->ceilingWidth;

    if (areaA < areaB) return -1;
    else if (areaA > areaB) return 1;
    return 0;

}


int compare_length(const void* a, const void* b) {

    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    if (corniceA->length < corniceB->length) return -1;
    else if (corniceA->length > corniceB->length) return 1;
    return 0;

}


int compare_width(const void* a, const void* b) {

    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    if (corniceA->ceilingWidth < corniceB->ceilingWidth) return -1;
    else if (corniceA->ceilingWidth > corniceB->ceilingWidth) return 1;
    return 0;

}


int compare_height(const void* a, const void* b) {

    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    if (corniceA->wallHeight < corniceB->wallHeight) return -1;
    else if (corniceA->wallHeight > corniceB->wallHeight) return 1;
    return 0;

}


int compare_material(const void* a, const void* b) {

    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;
    return strcmp(corniceA->material, corniceB->material);

}


void sort_array(cornice_t* cornices, int size, int field) {

    switch (field) {
    case 1:
        qsort(cornices, size, sizeof(cornice_t), compare_length);
        printf("Сортировка по длине выполнена.\n");
        break;
    case 2:
        qsort(cornices, size, sizeof(cornice_t), compare_width);
        printf("Сортировка по ширине выполнена.\n");
        break;
    case 3:
        qsort(cornices, size, sizeof(cornice_t), compare_height);
        printf("Сортировка по высоте выполнена.\n");
        break;
    case 4:
        qsort(cornices, size, sizeof(cornice_t), compare_area);
        printf("Сортировка по площади выполнена.\n");
        break;
    case 5:
        qsort(cornices, size, sizeof(cornice_t), compare_material);
        printf("Сортировка по материалу выполнена.\n");
        break;
    default:
        printf("Неверный выбор поля для сортировки.\n");
    }

}

int output_file(char* filename, cornice_t* arr, int n) {

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка открытия файла для записи.\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d %d %d %s\n",
            arr[i].length,
            arr[i].ceilingWidth,
            arr[i].wallHeight,
            arr[i].material);
    }

    fclose(file);
    printf("Данные успешно записаны в файл '%s' (%d записей).\n", filename, n);
    return 1;

}

int input_file(char* filename, cornice_t* arr, int max_size) {

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла для чтения.\n");
        return 0;
    }

    int count = 0;
    while (count < max_size &&
        fscanf(file, "%d %d %d %19s",
            &arr[count].length,
            &arr[count].ceilingWidth,
            &arr[count].wallHeight,
            arr[count].material) == 4) {
        count++;
    }

    fclose(file);
    printf("Загружено %d записей из файла '%s'.\n", count, filename);
    return count;

}

void edit_record(cornice_t* cornices, int size) {

    if (size == 0) {
        printf("Массив пуст. Нет записей для изменения.\n");
        return;
    }

    int index;
    printf("Введите номер записи для изменения (1-%d): ", size);
    scanf("%d", &index);

    if (index < 1 || index > size) {
        printf("Неверный номер записи.\n");
        return;
    }

    index--;

    printf("\nТекущая запись:\n");
    print_cornice(cornices[index]);

    printf("\nВведите новые значения:\n");
    printf("Длина (мм): ");
    scanf("%d", &cornices[index].length);
    printf("Ширина по потолку (мм): ");
    scanf("%d", &cornices[index].ceilingWidth);
    printf("Высота по стене (мм): ");
    scanf("%d", &cornices[index].wallHeight);
    printf("Материал: ");
    scanf("%s", cornices[index].material);

    printf("Запись успешно изменена.\n");

}

void search_record(cornice_t* cornices, int size) {

    if (size == 0) {
        printf("Массив пуст. Нет записей для поиска.\n");
        return;
    }

    int choice;
    printf("\nВыберите поле для поиска:\n");
    printf("1. Материал\n");
    printf("2. Длина\n");
    printf("3. Ширина\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    cornice_t* found = NULL;

    switch (choice) {
    case 1: {
        char material[20];
        printf("Введите материал для поиска: ");
        scanf("%s", material);
        found = search_material(cornices, size, material);
        break;
    }
    case 2: {
        int length;
        printf("Введите длину для поиска (мм): ");
        scanf("%d", &length);
        found = search_length(cornices, size, length);
        break;
    }
    case 3: {
        int width;
        printf("Введите ширину для поиска (мм): ");
        scanf("%d", &width);
        found = search_width(cornices, size, width);
        break;
    }
    default:
        printf("Неверный выбор.\n");
        return;
    }

    if (found != NULL) {
        printf("\nНайдена запись:\n");
        printf("===============================================\n");
        printf("| Длина | Ширина | Высота | Материал        |\n");
        printf("===============================================\n");
        print_cornice(*found);
        printf("===============================================\n");
    }
    else {
        printf("Запись не найдена.\n");
    }

}

void display_menu() {

    printf("\n=== МЕНЮ ПРОГРАММЫ ===\n");
    printf("1. Заполнить массив случайными значениями\n");
    printf("2. Заполнить массив константными значениями\n");
    printf("3. Вывести все записи\n");
    printf("4. Изменить запись\n");
    printf("5. Найти запись\n");
    printf("6. Отсортировать массив\n");
    printf("7. Сохранить в файл\n");
    printf("8. Загрузить из файла\n");
    printf("9. Сохранить в новый файл\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");

}