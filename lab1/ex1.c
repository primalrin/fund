#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_X_FOR_E 10

int handle_h_flag(const char *x_str);
int handle_p_flag(const char *x_str);
int handle_s_flag(const char *x_str);
int handle_e_flag(const char *x_str);
int handle_a_flag(const char *x_str);
int handle_f_flag(const char *x_str);
int is_number(const char *str);
long long calculate_factorial(int n);

int is_number(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

int handle_h_flag(const char *x_str)
{
    int x = atoi(x_str);
    int found = 0;

    for (int i = 1; i <= 100; i++)
    {
        if (i % x == 0)
        {
            printf("%d ", i);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No multiples of %d found within 1 to 100.\n", x);
    }
    else
    {
        printf("\n");
    }

    return 0;
}

int handle_p_flag(const char *x_str)
{
    int x = atoi(x_str);
    int is_prime = 1;

    if (x <= 1)
    {
        is_prime = 0;
    }
    else
    {
        for (int i = 2; i * i <= x; i++)
        {
            if (x % i == 0)
            {
                is_prime = 0;
                break;
            }
        }
    }

    if (is_prime)
    {
        printf("%d is a prime number.\n", x);
    }
    else if (x > 1)
    {
        printf("%d is a composite number.\n", x);
    }
    else
    {
        printf("%d is neither prime nor composite.\n", x);
    }

    return 0;
}

int handle_s_flag(const char *x_str)
{
    int x = atoi(x_str);

    if (x == 0)
    {
        printf("0\n");
        return 0;
    }

    char hex_digits[] = "0123456789ABCDEF";
    char hex_representation[32];
    int index = 0;

    while (x > 0)
    {
        hex_representation[index++] = hex_digits[x % 16];
        x /= 16;
    }

    for (int i = index - 1; i >= 0; i--)
    {
        printf("%c ", hex_representation[i]);
    }
    printf("\n");

    return 0;
}

int handle_e_flag(const char *x_str)
{
    int x = atoi(x_str);

    if (x > MAX_X_FOR_E)
    {
        fprintf(stderr, "Error: x must be less than or equal to %d for -e flag.\n", MAX_X_FOR_E);
        return 4;
    }

    for (int base = 1; base <= 10; base++)
    {
        printf("Powers of %d:\n", base);
        for (int exp = 1; exp <= x; exp++)
        {
            long long result = 1;
            for (int i = 0; i < exp; i++)
            {
                result *= base;
            }
            printf("%lld ", result);
        }
        printf("\n");
    }

    return 0;
}

int handle_a_flag(const char *x_str)
{
    int x = atoi(x_str);
    long long sum = 0;

    for (int i = 1; i <= x; i++)
    {
        sum += i;
    }

    printf("Sum of numbers from 1 to %d: %lld\n", x, sum);

    return 0;
}

int handle_f_flag(const char *x_str)
{
    int x = atoi(x_str);

    if (x < 0)
    {
        fprintf(stderr, "Error: Factorial is not defined for negative numbers.\n");
        return 5;
    }

    long long factorial = calculate_factorial(x);

    printf("Factorial of %d: %lld\n", x, factorial);

    return 0;
}

long long calculate_factorial(int n)
{
    if (n == 0)
    {
        return 1;
    }
    else
    {
        long long result = 1;
        for (int i = 2; i <= n; i++)
        {
            if ((LLONG_MAX / result) < i)
            {
                fprintf(stderr, "Error: Factorial calculation overflow.\n");
                exit(6);
            }
            result *= i;
        }
        return result;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <flag> <number>\n", argv[0]);
        return 1;
    }

    const char *flag = argv[1];
    const char *x_str = argv[2];

    if (!is_number(x_str))
    {
        fprintf(stderr, "Error: Invalid number format.\n");
        return 2;
    }

    if (strcmp(flag, "-h") == 0)
    {
        return handle_h_flag(x_str);
    }
    else if (strcmp(flag, "-p") == 0)
    {
        return handle_p_flag(x_str);
    }
    else if (strcmp(flag, "-s") == 0)
    {
        return handle_s_flag(x_str);
    }
    else if (strcmp(flag, "-e") == 0)
    {
        return handle_e_flag(x_str);
    }
    else if (strcmp(flag, "-a") == 0)
    {
        return handle_a_flag(x_str);
    }
    else if (strcmp(flag, "-f") == 0)
    {
        return handle_f_flag(x_str);
    }
    else
    {
        fprintf(stderr, "Error: Unknown flag '%s'.\n", flag);
        return 3;
    }
}

// gcc ex1.c -o ex1

// ./ex1 -h 5
// ./ex1 -p 17
// ./ex1 -s 255
// ./ex1 -e 3
// ./ex1 -a 10
// ./ex1 -f 5