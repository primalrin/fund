#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

// Функция сравнения подстрок без учета регистра
int string_compare(const char *substring, const char *str)
{
    size_t sub_len = strlen(substring);
    size_t str_len = strlen(str);

    if (sub_len > str_len)
    {
        return 0;
    }

    for (size_t i = 0; i < sub_len; ++i)
    {
        if (tolower(substring[i]) != tolower(str[i]))
        {
            return 0;
        }
    }

    return 1;
}

// Функция поиска подстроки в файле
int search_substring_in_file(const char *substring, const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        perror("Ошибка открытия файла");
        return -1;
    }

    int line_number = 1;
    int char_number = 1;
    char line[1024]; // Буфер для чтения строки из файла

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *line_ptr = line;
        char_number = 1; // Сброс номера символа для каждой новой строки
        while (*line_ptr != '\0')
        {
            if (string_compare(substring, line_ptr))
            {
                printf("Файл: %s, Строка: %d, Символ: %d\n", filepath, line_number, char_number);
                line_ptr += strlen(substring) - 1; // Сдвиг указателя после нахождения совпадения
                char_number += strlen(substring) - 1;
            }
            line_ptr++;
            char_number++;
        }
        line_number++;
    }

    if (fclose(file) != 0)
    {
        perror("Ошибка закрытия файла");
        return -1;
    }
    return 0;
}

// Функция с переменным числом аргументов
int search_substring(const char *substring, ...)
{
    va_list files;
    va_start(files, substring);

    const char *filepath;
    while ((filepath = va_arg(files, const char *)) != NULL)
    {
        if (search_substring_in_file(substring, filepath))
        {
            va_end(files);
            return -1;
        };
    }

    va_end(files);
    return 0;
}

int main()
{
    // Пример использования
    if (search_substring("test", "file1.txt", "file2.txt", "file3.txt", NULL) != 0)
    {
        fprintf(stderr, "Ошибка поиска подстроки.\n");
        return 1;
    };
    return 0;
}

// gcc ex3.c -o ex3