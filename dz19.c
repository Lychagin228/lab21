#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Структура для хранения даты рождения
typedef struct {
    int day;
    int month;
    int year;
    char place[50];
} BirthDate;

// Структура для хранения информации об игроке
typedef struct {
    char surname[50];
    BirthDate birth;
    char position[30];
    int games;
    int yellow_cards;
} Player;

// Функция вычисления возраста
int calculate_age(BirthDate birth) {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);

    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    int age = current_year - birth.year;

    if (current_month < birth.month ||
        (current_month == birth.month && current_day < birth.day)) {
        age--;
    }

    return age;
}

// Функция проверки условия по карточкам
int check_cards_condition(Player player) {
    if (player.games == 0) return 0;

    int blocks_of_10_games = player.games / 10;
    if (player.games % 10 != 0) {
        blocks_of_10_games++;
    }

    return player.yellow_cards <= blocks_of_10_games;
}

// Функция ввода информации об игроке
void input_player(Player* player) {
    printf("\nВведите фамилию игрока: ");
    scanf("%s", player->surname);

    printf("Введите дату рождения (день месяц год): ");
    scanf("%d %d %d", &player->birth.day, &player->birth.month, &player->birth.year);

    printf("Введите место рождения: ");
    scanf("%s", player->birth.place);

    printf("Введите амплуа игрока: ");
    scanf("%s", player->position);

    printf("Введите количество игр: ");
    scanf("%d", &player->games);

    printf("Введите количество желтых карточек: ");
    scanf("%d", &player->yellow_cards);
}

// Функция вывода информации об игроке
void print_player(Player player) {
    printf("\nФамилия: %s\n", player.surname);
    printf("Дата рождения: %02d.%02d.%d\n", player.birth.day, player.birth.month, player.birth.year);
    printf("Место рождения: %s\n", player.birth.place);
    printf("Амплуа: %s\n", player.position);
    printf("Количество игр: %d\n", player.games);
    printf("Желтых карточек: %d\n", player.yellow_cards);
}

// ФУНКЦИЯ ДЛЯ ЗАПИСИ ДАННЫХ В ФАЙЛ
int writefile(char* fname, Player* players, int size) {
    FILE* out;
    if ((out = fopen(fname, "wt")) == NULL) {
        printf("Ошибка открытия файла для записи");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        fprintf(out, "Фамилия:%s ; Дата:%02d:%02d:%04d ; Место:%s ; Амплуа:%s ; Игры:%d ; Карточки:%d\n",
            players[i].surname,
            players[i].birth.day, players[i].birth.month, players[i].birth.year,
            players[i].birth.place,
            players[i].position,
            players[i].games,
            players[i].yellow_cards);
    }

    fclose(out);
    return 1;
}

// ФУНКЦИЯ ДЛЯ ЧТЕНИЯ ДАННЫХ ИЗ ФАЙЛА
int readfile(char* fname, Player* players, int max_size) {
    FILE* in;
    if ((in = fopen(fname, "rt")) == NULL) {
        printf("Ошибка открытия файла для чтения");
        return 0;
    }

    int count = 0;
    while (count < max_size &&
        fscanf(in, "Фамилия:%49s ; Дата:%d:%d:%d ; Место:%49s ; Амплуа:%29s ; Игры:%d ; Карточки:%d\n",
            players[count].surname,
            &players[count].birth.day, &players[count].birth.month, &players[count].birth.year,
            players[count].birth.place,
            players[count].position,
            &players[count].games,
            &players[count].yellow_cards) == 8) {
        count++;
    }

    fclose(in);
    return count;
}

int main() {
    setlocale(LC_CTYPE, "RUS");

    int num_players;
    Player players[10];
    char filename[100];
    int choice;

    printf("=== Программа учета футболистов ===\n");
    printf("1. Ввести новых игроков\n");
    printf("2. Загрузить игроков из файла\n");
    printf("Выберите действие: ");
    scanf("%d", &choice);
    getchar(); // Очистка буфера ввода

    if (choice == 1) {
        // Ввод новых данных
        printf("\nСколько игроков вы хотите ввести (5-10)? ");
        scanf("%d", &num_players);

        if (num_players < 5 || num_players > 10) {
            printf("Ошибка! Введите от 5 до 10 игроков.\n");
            return 1;
        }

        printf("\n=== Ввод информации об игроках ===\n");
        for (int i = 0; i < num_players; i++) {
            printf("\nИгрок #%d:\n", i + 1);
            input_player(&players[i]);
        }

        // Сохранение в файл
        printf("\nВведите имя файла для сохранения данных: ");
        scanf("%s", filename);
        if (writefile(filename, players, num_players)) {
            printf("Данные успешно сохранены в файл '%s'\n", filename);
        }
    }
    else if (choice == 2) {
        // Загрузка из файла
        printf("\nВведите имя файла для загрузки данных: ");
        scanf("%s", filename);
        num_players = readfile(filename, players, 10);

        if (num_players > 0) {
            printf("Загружено %d игроков из файла '%s'\n", num_players, filename);
        }
        else {
            printf("Не удалось загрузить данные из файла\n");
            return 1;
        }
    }
    else {
        printf("Неверный выбор\n");
        return 1;
    }

    // Вывод всех игроков
    printf("\n=== Все игроки ===\n");
    for (int i = 0; i < num_players; i++) {
        printf("\nИгрок #%d:", i + 1);
        print_player(players[i]);
    }

    // Поиск игроков по условиям
    printf("\n=== Игроки, соответствующие условиям ===\n");
    printf("(старше 20 лет и не более 1 желтой карточки за 10 игр)\n");

    int found = 0;
    for (int i = 0; i < num_players; i++) {
        int age = calculate_age(players[i].birth);
        int cards_ok = check_cards_condition(players[i]);

        if (age > 20 && cards_ok) {
            printf("\n--- Игрок #%d ---", i + 1);
            print_player(players[i]);
            printf("Возраст: %d лет\n", age);
            found = 1;
        }
    }

    if (!found) {
        printf("\nИгроки, соответствующие условиям, не найдены.\n");
    }

    // Дополнительная опция: запись результатов в файл
    printf("\nХотите сохранить результаты поиска в файл? (1 - да, 0 - нет): ");
    int save_results;
    scanf("%d", &save_results);

    if (save_results == 1) {
        printf("Введите имя файла для сохранения результатов: ");
        scanf("%s", filename);

        FILE* results_file = fopen(filename, "wt");
        if (results_file) {
            fprintf(results_file, "=== Результаты поиска ===\n");
            fprintf(results_file, "Игроки старше 20 лет с не более 1 карточки за 10 игр:\n\n");

            for (int i = 0; i < num_players; i++) {
                int age = calculate_age(players[i].birth);
                int cards_ok = check_cards_condition(players[i]);

                if (age > 20 && cards_ok) {
                    fprintf(results_file, "Фамилия: %s\n", players[i].surname);
                    fprintf(results_file, "Дата рождения: %02d.%02d.%d\n",
                        players[i].birth.day, players[i].birth.month, players[i].birth.year);
                    fprintf(results_file, "Место рождения: %s\n", players[i].birth.place);
                    fprintf(results_file, "Амплуа: %s\n", players[i].position);
                    fprintf(results_file, "Игры: %d, Карточки: %d\n",
                        players[i].games, players[i].yellow_cards);
                    fprintf(results_file, "Возраст: %d лет\n\n", age);
                }
            }

            fclose(results_file);
            printf("Результаты сохранены в файл '%s'\n", filename);
        }
        else {
            printf("Ошибка при создании файла результатов\n");
        }
    }

    return 0;
}