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

enum quadratic_equation_result solve_quadratic_equation(const double a, const double b, const double c, const double epsilon, double *x1, double *x2);
int check_multiple(const int num1, const int num2);
int check_right_triangle(const double a, const double b, const double c, const double epsilon);

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
            fprintf(stderr, "Error: Incorrect number of arguments for -q flag. Expected 5, got %d.\n", argc - 1);
            return 1;
        }

        char *endptr;
        double epsilon = strtod(argv[2], &endptr);
        if (*endptr != '\0' || epsilon <= 0)
        {
            fprintf(stderr, "Invalid epsilon value: %s\n", argv[2]);
            return 1;
        }

        double coeffs[3];
        for (int i = 0; i < 3; i++)
        {
            coeffs[i] = strtod(argv[i + 3], &endptr);
            if (*endptr != '\0')
            {
                fprintf(stderr, "Invalid coefficient value: %s\n", argv[i + 3]);
                return 1;
            }
        }

        double x1, x2;

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
                    switch (result)
                    {
                    case INVALID_INPUT:
                        fprintf(stderr, "Error: Invalid input for quadratic equation.\n");
                        return 1;
                    case NO_ROOTS:
                        printf("No real roots for coefficients %.6f, %.6f, %.6f\n", coeffs[i], coeffs[j], coeffs[k]);
                        break;
                    }
                }
            }
        }
    }
    else if (strcmp(argv[1], "-m") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Error: Incorrect number of arguments for -m flag. Expected 3, got %d.\n", argc - 1);
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
            fprintf(stderr, "Error: Incorrect number of arguments for -t flag. Expected 5, got %d.\n", argc - 1);
            return 1;
        }

        char *endptr;
        double epsilon = strtod(argv[2], &endptr);
        if (*endptr != '\0' || epsilon <= 0)
        {
            fprintf(stderr, "Invalid epsilon value: %s\n", argv[2]);
            return 1;
        }

        double sides[3];
        for (int i = 0; i < 3; i++)
        {
            sides[i] = strtod(argv[i + 3], &endptr);
            if (*endptr != '\0' || sides[i] <= 0)
            {
                fprintf(stderr, "Invalid side value: %s\n", argv[i + 3]);
                return 1;
            }
        }

        if (check_right_triangle(sides[0], sides[1], sides[2], epsilon))
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
        fprintf(stderr, "Error: Invalid flag provided: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

enum quadratic_equation_result solve_quadratic_equation(const double a, const double b, const double c, const double epsilon, double *x1, double *x2)
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

            return NO_ROOTS;
        }
    }
}

int check_multiple(const int num1, const int num2)
{
    return num1 % num2 == 0;
}

int check_right_triangle(const double a, const double b, const double c, const double epsilon)
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