#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100

int is_delimiter(char c);
int split_into_words(char* line, char words[][MAX_LINE_LEN], int* word_count);

int main() {

    setlocale(LC_CTYPE, "RUS");

    FILE* input_file, * output_file;
    char input_filename[] = "winter.txt";
    char output_filename[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];
    char words[MAX_WORDS][MAX_LINE_LEN];
    int word_count;

    // Открываем входной файл
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", input_filename);
        return 1;
    }

    // Запрашиваем имя выходного файла у пользователя
    printf("Введите имя выходного файла: ");
    fgets(output_filename, MAX_LINE_LEN, stdin);
    // Удаляем символ новой строки
    output_filename[strcspn(output_filename, "\n")] = '\0';

    // Открываем выходной файл
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Ошибка: не удалось создать файл %s\n", output_filename);
        fclose(input_file);
        return 1;
    }

    printf("Обработка файла...\n");

    // Читаем файл построчно
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        // Удаляем символ новой строки в конце строки
        line[strcspn(line, "\n")] = '\0';

        // Разбиваем строку на слова
        split_into_words(line, words, &word_count);

        if (word_count == 0) {
            // Пустая строка
            fprintf(output_file, "\n");
            continue;
        }

        // Собираем строку с добавлением знака % перед каждым вторым словом
        for (int i = 0; i < word_count; i++) {
            if (i % 2 == 1) { // i=1 (второе слово), i=3 (четвертое слово) и т.д.
                fprintf(output_file, "%%%s", words[i]);
            }
            else {
                fprintf(output_file, "%s", words[i]);
            }

            // Добавляем пробел между словами, кроме последнего
            if (i < word_count - 1) {
                fprintf(output_file, " ");
            }
        }
        fprintf(output_file, "\n");
    }

    // Закрываем файлы
    fclose(input_file);
    fclose(output_file);

    printf("Обработка завершена. Результат записан в файл %s\n", output_filename);

    return 0;
}

int is_delimiter(char c) {
    return c == ' ' || c == ',' || c == '.' || c == '!' || c == '?' || c == ';' || c == ':' || c == '\t' || c == '\n';
}

int split_into_words(char* line, char words[][MAX_LINE_LEN], int* word_count) {
    *word_count = 0;
    int i = 0;
    int len = strlen(line);

    while (i < len && *word_count < MAX_WORDS) {
        // Пропускаем разделители
        while (i < len && is_delimiter(line[i])) {
            i++;
        }

        if (i >= len) break;

        // Начинаем новое слово
        int j = 0;
        while (i < len && !is_delimiter(line[i]) && j < MAX_LINE_LEN - 1) {
            words[*word_count][j++] = line[i++];
        }
        words[*word_count][j] = '\0';
        (*word_count)++;
    }

    return 0;
}