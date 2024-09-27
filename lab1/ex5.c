#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

double factorial(int n)
{
    if (n < 0)
    {
        return -1;
    }
    if (n == 0)
    {
        return 1;
    }
    double result = 1.0;
    for (int i = 1; i <= n; ++i)
    {
        if (DBL_MAX / result < i)
        {
            return -2;
        }
        result *= i;
    }
    return result;
}

double double_factorial(int n)
{
    if (n < -1)
    {
        return -1;
    }
    if (n <= 1)
    {
        return 1;
    }
    double res = 1.0;
    for (int i = n; i >= 2; i -= 2)
    {
        if (DBL_MAX / res < i)
        {
            return -2;
        }
        res *= i;
    }
    return res;
}

int calculate_sum_a(double x, double epsilon, double *result)
{
    if (x == 0.0)
    {
        *result = 1.0;
        return 0;
    }

    double sum = 1.0;
    double term = 1.0;
    int n = 1;

    do
    {
        term *= x / n;
        if (isinf(term))
        {
            return 1;
        }
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int calculate_sum_b(double x, double epsilon, double *result)
{
    if (x == 0)
    {
        *result = 1.0;
        return 0;
    }

    double sum = 1.0;
    double term = 1.0;
    int n = 1;

    do
    {
        if (isinf(term))
        {
            return 1;
        }
        term *= -x * x / ((2.0 * n - 1) * (2.0 * n));
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int calculate_sum_c(double x, double epsilon, double *result)
{
    if (x == 0)
    {
        *result = 1.0;
        return 0;
    }

    double sum = 1.0;
    double term = 1.0;
    int n = 1;

    do
    {
        if (isinf(term))
        {
            return 1;
        }
        term *= 27.0 * x * x / ((3.0 * n - 2) * (3.0 * n - 1) * (3.0 * n));
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int calculate_sum_d(double x, double epsilon, double *result)
{
    if (x == 0)
    {
        *result = 0;
        return 0;
    }

    double sum = 0.0;
    double term = -x * x / 2.0;
    int n = 1;

    do
    {
        if (isinf(term))
        {
            return 1;
        }
        sum += term;
        n++;
        term *= -x * x * (2.0 * n - 1.0) / (2.0 * n * (2.0 * n - 2.0));
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <x_value> <epsilon_value>\n", argv[0]);
        return 1;
    }

    char *endptr;
    double x = strtod(argv[1], &endptr);
    if (*endptr != '\0' || x == HUGE_VAL || x == -HUGE_VAL)
    {
        fprintf(stderr, "Invalid x value.\n");
        return 1;
    }

    double epsilon = strtod(argv[2], &endptr);
    if (*endptr != '\0' || epsilon == HUGE_VAL || epsilon == -HUGE_VAL || epsilon <= 0)
    {
        fprintf(stderr, "Invalid epsilon value.\n");
        return 1;
    }

    double result_a, result_b, result_c, result_d;

    if (calculate_sum_a(x, epsilon, &result_a) != 0)
    {
        fprintf(stderr, "Error calculating sum a.\n");
        return 1;
    }
    printf("a: %lf\n", result_a);
    if (calculate_sum_b(x, epsilon, &result_b) != 0)
    {
        fprintf(stderr, "Error calculating sum b.\n");
        return 1;
    }
    printf("b: %lf\n", result_b);
    if (calculate_sum_c(x, epsilon, &result_c) != 0)
    {
        fprintf(stderr, "Error calculating sum c.\n");
        return 1;
    }
    printf("c: %lf\n", result_c);
    if (calculate_sum_d(x, epsilon, &result_d) != 0)
    {
        fprintf(stderr, "Error calculating sum d.\n");
        return 1;
    }
    printf("d: %lf\n", result_d);

    return 0;
}

// gcc ex5.c -o ex5

// ./ex5.exe 1.5 0.00001