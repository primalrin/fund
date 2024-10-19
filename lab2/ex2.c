#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Функция для вычисления среднего геометрического чисел
double geometric_mean(int count, ...)
{
    if (count <= 0)
    {
        fprintf(stderr, "Error: Number of elements must be positive.\n");
        return -1.0;
    }

    va_list args;
    va_start(args, count);

    double product = 1.0;
    for (int i = 0; i < count; ++i)
    {
        double num = va_arg(args, double);
        if (num < 0)
        {
            fprintf(stderr, "Error: Cannot calculate geometric mean of negative numbers.\n");
            return -1.0;
        }
        else if (num == 0)
        {
            return 0;
        }
        product *= num;
    }

    va_end(args);

    return pow(product, 1.0 / count);
}

// Рекурсивная функция быстрого возведения в степень
double power(double base, int exp)
{
    if (exp == 0)
    {
        return 1;
    }
    else if (exp < 0)
    {
        return 1.0 / power(base, -exp);
    }
    else if (exp % 2 == 0)
    {
        double temp = power(base, exp / 2);
        return temp * temp;
    }
    else
    {
        return base * power(base, exp - 1);
    }
}

int main()
{
    double gm = geometric_mean(3, 2.0, 4.0, 8.0);
    if (gm < 0)
    {
        return 1;
    }
    printf("Geometric mean: %f\n", gm);

    double pow_result = power(2.0, 3);
    printf("2^3: %f\n", pow_result);

    pow_result = power(3.0, -2);
    printf("3^-2: %f\n", pow_result);

    gm = geometric_mean(0);
    if (gm < 0)
    {
        return 1;
    }

    return 0;
}

// gcc ex2.c -o ex2