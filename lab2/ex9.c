#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

bool is_finite_representation(double number, int base)
{
    double fractional_part = number;
    // Максимальное количество итераций для предотвращения зацикливания
    const int MAX_ITERATIONS = 1000;
    int iterations = 0;

    while (fractional_part > 0 && iterations < MAX_ITERATIONS)
    {
        fractional_part *= base;
        int integer_part = (int)fractional_part;
        fractional_part -= integer_part;

        if (fabs(fractional_part) < 1e-15)
        {
            return true;
        }
        iterations++;
    }
    return false;
}

int check_numbers(int base, int count, ...)
{
    if (base < 2)
    {
        printf("Ошибка: Основание системы счисления должно быть больше или равно 2.\n");
        return 1;
    }

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i)
    {
        double number = va_arg(args, double);

        if (number <= 0 || number >= 1)
        {
            printf("Ошибка: Числа должны быть в диапазоне (0, 1).\n");
            va_end(args);
            return 1;
        }

        if (is_finite_representation(number, base))
        {
            printf("Число %.6f имеет конечное представление в системе счисления с основанием %d.\n",
                   number, base);
        }
        else
        {
            printf("Число %.6f не имеет конечного представления в системе счисления с основанием %d.\n",
                   number, base);
        }
    }

    va_end(args);
    return 0;
}

int main()
{
    printf("Тест 1: Двоичная система (основание 2)\n");
    check_numbers(2, 3, 0.5, 0.25, 0.125);

    printf("\nТест 2: Десятичная система (основание 10)\n");
    check_numbers(10, 2, 0.1, 0.3333);

    printf("\nТест 3: Троичная система (основание 3)\n");
    check_numbers(3, 2, 1.0 / 3.0, 0.2);

    return 0;
}