#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum quadratic_equation_result
{
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    LINEAR_EQUATION = 3,
    INVALID_INPUT = -1
};

enum quadratic_equation_result solve_quadratic_equation(double a, double b, double c, double epsilon, double *x1, double *x2);
int check_multiple(int num1, int num2);
int check_right_triangle(double a, double b, double c, double epsilon);

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: No flag provided.\n");
        return 1;
    }

    if (strcmp(argv[1], "-q") == 0)
    {
        if (argc != 6)
        {
            fprintf(stderr, "Error: Incorrect number of arguments for -q flag.\n");
            return 1;
        }

        char *endptr;
        double epsilon = strtod(argv[2], &endptr);
        if (*endptr != '\0' || epsilon <= 0)
        {
            fprintf(stderr, "Invalid epsilon value: %s\n", argv[2]);
            return 1;
        }

        double a = strtod(argv[3], &endptr);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Invalid coefficient 'a' value: %s\n", argv[3]);
            return 1;
        }
        double b = strtod(argv[4], &endptr);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Invalid coefficient 'b' value: %s\n", argv[4]);
            return 1;
        }
        double c = strtod(argv[5], &endptr);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Invalid coefficient 'c' value: %s\n", argv[5]);
            return 1;
        }

        double x1, x2;
        double coeffs[3] = {a, b, c};

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == j)
                    continue;
                for (int k = 0; k < 3; ++k)
                {
                    if (k == i || k == j)
                        continue;

                    enum quadratic_equation_result result = solve_quadratic_equation(coeffs[i], coeffs[j], coeffs[k], epsilon, &x1, &x2);
                    if (result == INVALID_INPUT)
                    {
                        fprintf(stderr, "Error solving quadratic equation.\n");
                    }
                }
            }
        }
    }
    else if (strcmp(argv[1], "-m") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Error: Incorrect number of arguments for -m flag.\n");
            return 1;
        }

        int num1 = atoi(argv[2]);
        int num2 = atoi(argv[3]);

        if (num2 == 0)
        {
            fprintf(stderr, "Error: The second number cannot be zero.\n");
            return 1;
        }

        if (check_multiple(num1, num2))
        {
            printf("%d is a multiple of %d\n", num1, num2);
        }
        else
        {
            printf("%d is not a multiple of %d\n", num1, num2);
        }
    }
    else if (strcmp(argv[1], "-t") == 0)
    {
        if (argc != 6)
        {
            fprintf(stderr, "Error: Incorrect number of arguments for -t flag.\n");
            return 1;
        }

        char *endptr;
        double epsilon = strtod(argv[2], &endptr);
        if (*endptr != '\0' || epsilon <= 0)
        {
            fprintf(stderr, "Invalid epsilon value: %s\n", argv[2]);
            return 1;
        }

        double a = strtod(argv[3], &endptr);
        if (*endptr != '\0' || a <= 0)
        {
            fprintf(stderr, "Invalid side 'a' value: %s\n", argv[3]);
            return 1;
        }

        double b = strtod(argv[4], &endptr);
        if (*endptr != '\0' || b <= 0)
        {
            fprintf(stderr, "Invalid side 'b' value: %s\n", argv[4]);
            return 1;
        }

        double c = strtod(argv[5], &endptr);
        if (*endptr != '\0' || c <= 0)
        {
            fprintf(stderr, "Invalid side 'c' value: %s\n", argv[5]);
            return 1;
        }

        if (check_right_triangle(a, b, c, epsilon))
        {
            printf("The sides can form a right triangle.\n");
        }
        else
        {
            printf("The sides cannot form a right triangle.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error: Invalid flag provided.\n");
        return 1;
    }

    return 0;
}

enum quadratic_equation_result solve_quadratic_equation(double a, double b, double c, double epsilon, double *x1, double *x2)
{
    if (fabs(a) < epsilon)
    {
        if (fabs(b) < epsilon)
        {
            return INVALID_INPUT;
        }
        else
        {
            *x1 = -c / b;
            printf("x = %.6f\n", *x1);
            return LINEAR_EQUATION;
        }
    }
    else
    {
        double discriminant = b * b - 4 * a * c;
        if (discriminant > epsilon)
        {
            *x1 = (-b + sqrt(discriminant)) / (2 * a);
            *x2 = (-b - sqrt(discriminant)) / (2 * a);
            printf("x1 = %.6f, x2 = %.6f\n", *x1, *x2);
            return TWO_ROOTS;
        }
        else if (fabs(discriminant) < epsilon)
        {
            *x1 = -b / (2 * a);
            printf("x = %.6f\n", *x1);
            return ONE_ROOT;
        }
        else
        {
            printf("No real roots\n");
            return NO_ROOTS;
        }
    }
}

int check_multiple(int num1, int num2)
{
    return num1 % num2 == 0;
}

int check_right_triangle(double a, double b, double c, double epsilon)
{
    if (a <= 0 || b <= 0 || c <= 0)
    {
        return 0;
    }

    double a2 = a * a;
    double b2 = b * b;
    double c2 = c * c;

    if (fabs(a2 + b2 - c2) < epsilon || fabs(a2 + c2 - b2) < epsilon || fabs(b2 + c2 - a2) < epsilon)
    {
        return 1;
    }

    return 0;
}

// gcc ex3.c -o ex3

// ./ex3.exe -q 0.00001 1 2 3
// ./ex3.exe -m 10 5
// ./ex3.exe -t 0.00001 3 4 5