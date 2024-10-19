// NOT WORK
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

// Функция для печати целого числа римскими цифрами
int print_roman(int num, FILE *stream)
{
    if (num <= 0 || num > 3999)
    {
        return -1; // Диапазон римских цифр
    }

    const char *roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    int i = 0;
    while (num > 0)
    {
        if (num >= values[i])
        {
            if (fprintf(stream, "%s", roman[i]) < 0)
                return -1;
            num -= values[i];
        }
        else
        {
            i++;
        }
    }
    return 0;
}

// Функция для печати цекендорфова представления
int print_zeckendorf(unsigned int num, FILE *stream)
{
    if (num == 0)
    {
        if (fprintf(stream, "0") < 0)
            return -1; //  Для 0 выводим просто 0
        return 0;
    }

    unsigned int fib[47]; // Максимальное число Фибоначчи, которое помещается в unsigned int
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < 47; i++)
    {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    char representation[48]; // Строка для представления
    memset(representation, '0', sizeof(representation) - 1);
    representation[47] = '\0';

    int k = 46;
    while (num > 0 && k >= 0)
    {
        if (num >= fib[k])
        {
            representation[46 - k] = '1';
            num -= fib[k];
        }
        k--;
    }

    // Убираем ведущие нули
    int start = 0;
    while (representation[start] == '0' && representation[start + 1] != '\0')
    {
        start++;
    }

    if (fprintf(stream, "%s", representation + start) < 0)
        return -1;
    return 0;
}

// Функция для печати числа в заданной системе счисления
int print_base(int num, int base, FILE *stream, int uppercase)
{
    if (base < 2 || base > 36)
    {
        return -1; // Возвращаем ошибку для некорректного основания
    }

    char buffer[33];
    char *ptr = buffer + 32;
    *--ptr = '\0';

    int sign = (num < 0);
    unsigned int unum = (unsigned int)(num < 0 ? -num : num);

    do
    {
        int digit = unum % base;
        *--ptr = (digit < 10) ? (digit + '0') : (digit - 10 + (uppercase ? 'A' : 'a'));
    } while ((unum /= base) > 0);

    if (sign)
        *--ptr = '-';

    if (fprintf(stream, "%s", ptr) < 0)
        return -1;
    return 0;
}

// Функция для перевода строки в число из заданной системы счисления
int str_to_int(const char *str, int base, FILE *stream, int uppercase)
{
    if (base < 2 || base > 36)
    {
        base = 10;
    }

    char *endptr;
    long long result = strtoll(str, &endptr, base);

    if (result == LLONG_MAX || result == LLONG_MIN)
    {
        if (errno == ERANGE)
        {
            return -1; // Переполнение
        }
    }

    if (*endptr != '\0')
    {
        // Некорректные символы в строке
        return -1;
    }

    if (fprintf(stream, "%lld", result) < 0)
        return -1;
    return 0;
}

// Функция для печати дампа памяти
int print_memory_dump(const void *ptr, size_t size, FILE *stream)
{
    const unsigned char *bytes = (const unsigned char *)ptr;
    for (size_t i = 0; i < size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            if (fprintf(stream, "%d", (bytes[i] >> j) & 1) < 0)
                return -1;
        }
        if (i < size - 1)
        {
            if (fprintf(stream, " ") < 0)
                return -1;
        }
    }
    return 0;
}

// Функция overfprintf
int oversprintf(char *str, size_t size, const char *format, ...)
{
    if (!str || !format || size == 0)
    {
        return -1;
    }

    va_list args;
    va_start(args, format);
    int res = 0;
    int written = 0;

    for (const char *p = format; *p; p++)
    {
        if (*p == '%')
        {
            p++;
            if (!*p)
            {
                res = -1;
                break;
            }
            switch (*p)
            {
            case 'R':
                if (*(p + 1) == 'o')
                {
                    written = snprintf(str + res, size - res, "%s", ""); // Очищаем буфер
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_roman(va_arg(args, int), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }

                    p++;
                }
                else
                {
                    res = -1;
                }
                break;
            case 'Z':
                if (*(p + 1) == 'r')
                {
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_zeckendorf(va_arg(args, unsigned int), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                }
                else
                {
                    res = -1;
                }
                break;
            case 'C':
                if (*(p + 1) == 'v')
                {
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_base(va_arg(args, int), va_arg(args, int), open_memstream(&str[res], &size), 0) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                }
                else if (*(p + 1) == 'V')
                {
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_base(va_arg(args, int), va_arg(args, int), open_memstream(&str[res], &size), 1) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                }
                else
                {
                    res = -1;
                }
                break;
            case 't':
                if (*(p + 1) == 'o')
                {
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (str_to_int(va_arg(args, char *), va_arg(args, int), open_memstream(&str[res], &size), 0) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                }
                else
                {
                    res = -1;
                }
                break;
            case 'T':
                if (*(p + 1) == 'O')
                {
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (str_to_int(va_arg(args, char *), va_arg(args, int), open_memstream(&str[res], &size), 1) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                }
                else
                {
                    res = -1;
                }
                break;
            case 'm':
                switch (*(p + 1))
                {
                case 'i':
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_memory_dump(va_arg(args, void *), sizeof(int), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                    break;
                case 'u':
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_memory_dump(va_arg(args, void *), sizeof(unsigned int), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                    break;
                case 'd':
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_memory_dump(va_arg(args, void *), sizeof(double), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                    break;
                case 'f':
                    written = snprintf(str + res, size - res, "%s", "");
                    if (written < 0 || (size_t)written >= size - res)
                    {
                        res = -1;
                        break;
                    }
                    if (print_memory_dump(va_arg(args, void *), sizeof(float), open_memstream(&str[res], &size)) < 0)
                    {
                        res = -1;
                    }
                    else
                    {
                        res += written;
                    }
                    p++;
                    break;
                default:
                    res = -1;
                }
                break;
            default:
                written = snprintf(str + res, size - res, "%%%c", *p);
                if (written < 0 || (size_t)written >= size - res)
                {
                    res = -1;
                }
                else
                {
                    res += written;
                }
            }
        }
        else
        {
            written = snprintf(str + res, size - res, "%c", *p);
            if (written < 0 || (size_t)written >= size - res)
            {
                res = -1;
            }
            else
            {
                res += written;
            }
        }

        if (res < 0)
        {
            break;
        }
    }

    va_end(args);

    if (res < 0)
    {
        return -1;
    }

    str[res] = '\0';
    return res;
}

int oversprintf(char *str, size_t size, const char *format, ...)
{
    if (!str || !format || size == 0)
    {
        return -1;
    }

    va_list args;
    va_start(args, format);

    // Используем vsnprintf для форматирования строки с ограничением размера
    int res = vsnprintf(str, size, format, args);

    va_end(args);

    if (res < 0 || (size_t)res >= size)
    {
        return -1; // Ошибка или переполнение буфера
    }

    return res; // Возвращаем количество записанных символов
}

int main()
{
    FILE *file = fopen("output.txt", "w");
    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

    int i = -10;
    unsigned int u = 10;
    double d = 3.14;
    float f = 2.71f;

    overfprintf(file, "Roman: %Ro\n", 1984);
    overfprintf(file, "Zeckendorf: %Zr\n", u);
    overfprintf(file, "Base 2 (Cv): %Cv\n", i, 2);
    overfprintf(file, "Base 16 (CV): %CV\n", i, 16);
    overfprintf(file, "String to int (to): %to\n", "1010", 2);
    overfprintf(file, "String to int (TO): %TO\n", "1010", 2);
    overfprintf(file, "Memory dump (int): %mi\n", &i);
    overfprintf(file, "Memory dump (unsigned int): %mu\n", &u);
    overfprintf(file, "Memory dump (double): %md\n", &d);
    overfprintf(file, "Memory dump (float): %mf\n", &f);

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        return 1;
    }

    return 0;
}