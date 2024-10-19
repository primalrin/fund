// NOT WORK CONVEX
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Функция для определения выпуклости многоугольника
int is_convex(int n, ...)
{
    if (n < 3)
    {
        fprintf(stderr, "Error: Less than three vertices provided.\n");
        return -1;
    }

    va_list args;
    va_start(args, n);

    double *x_coords = (double *)malloc(n * sizeof(double));
    double *y_coords = (double *)malloc(n * sizeof(double));

    if (x_coords == NULL || y_coords == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        return -1; // Или другой код ошибки
    }

    for (int i = 0; i < n; i++)
    {
        x_coords[i] = va_arg(args, double);
        y_coords[i] = va_arg(args, double);
    }

    va_end(args);

    int sign = 0;

    for (int i = 0; i < n; i++)
    {
        double x1 = x_coords[i];
        double y1 = y_coords[i];
        double x2 = x_coords[(i + 1) % n];
        double y2 = y_coords[(i + 1) % n];
        double x3 = x_coords[(i + 2) % n];
        double y3 = y_coords[(i + 2) % n];

        double cross_product = (x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1);
        int current_sign = (cross_product > 0) ? 1 : ((cross_product < 0) ? -1 : 0);

        if (sign == 0 && current_sign != 0)
        {
            sign = current_sign;
        }
        else if (sign != 0 && current_sign != 0 && sign != current_sign)
        {
            return 0; // Невыпуклый
        }
    }

    free(x_coords);
    free(y_coords);
    return 1; // Выпуклый
}

// Функция для вычисления значения многочлена
double polynomial_value(double x, int n, ...)
{
    if (n < 0)
    {
        fprintf(stderr, "Error: Invalid polynomial degree.\n");
        return -1.0;
    }

    va_list args;
    va_start(args, n);

    double result = 0.0;
    for (int i = n; i >= 0; --i)
    {
        double coeff = va_arg(args, double);
        result = result * x + coeff;
    }

    va_end(args);
    return result;
}

// Функция для преобразования строки в число в заданной системе счисления
unsigned long long str_to_base(const char *str, int base)
{
    unsigned long long num = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]))
        {
            fprintf(stderr, "Error: Invalid character in string: %c\n", str[i]);
            return ULLONG_MAX; // Indicate an error
        }

        int digit;
        if (isdigit(str[i]))
        {
            digit = str[i] - '0';
        }
        else
        {
            digit = toupper(str[i]) - 'A' + 10;
        }

        if (digit >= base)
        {
            fprintf(stderr, "Error: Invalid digit for base %d: %c\n", base, str[i]);
            return ULLONG_MAX; // Indicate an error
        }
        num = num * base + digit;
    }
    return num;
}

// Функция для проверки чисел Капрекара
void kaprekar_numbers(int base, ...)
{
    va_list args;
    va_start(args, base);

    char *str;
    while ((str = va_arg(args, char *)) != NULL)
    {
        unsigned long long num = str_to_base(str, base);
        if (num == ULLONG_MAX)
        { // error
            continue;
        }

        unsigned long long sq = num * num;

        char sq_str[100]; // Adjust size as needed
        sprintf(sq_str, "%llu", sq);

        int sq_len = strlen(sq_str);
        for (int i = 1; i < sq_len; i++)
        {
            unsigned long long right = sq % (unsigned long long)pow(base, i);
            unsigned long long left = sq / (unsigned long long)pow(base, i);

            if (left + right == num && right > 0)
            {
                printf("Kaprekar number: %s (base %d)\n", str, base);
                break; // Found a split, no need to check further
            }
        }
    }
    va_end(args);
}

int main()
{
    // Пример использования is_convex
    int convex = is_convex(0, 0, 0, 2, 1, 1, 4, 0, 0, 0); // Example with a non-convex polygon
    printf("Convex: %d\n", convex);

    convex = is_convex(0, 0, 1, 1, 2, 0, 0, 0); // Example with a convex polygon
    printf("Convex: %d\n", convex);

    // Пример использования polynomial_value
    double value = polynomial_value(2, 2, 1.0, 2.0, 3.0); // Explicitly use double literals
    printf("Polynomial value: %f\n", value);

    // Пример использования kaprekar_numbers
    kaprekar_numbers(10, "1", "45", "9", "297", NULL);
    kaprekar_numbers(17, "A", "10", NULL); // Example with base 17

    return 0;
}
