#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

#define MAX_X_FOR_E 10

int handle_h_flag(long long x);
int handle_p_flag(long long x);
int handle_s_flag(long long x);
int handle_e_flag(int x);
int handle_a_flag(long long x);
int handle_f_flag(int x);
long long calculate_factorial(int n);

int is_number(const char *str, long long *num);

int is_number(const char *str, long long *num)
{
    char *endptr;
    errno = 0;
    *num = strtoll(str, &endptr, 10);
    if (errno == ERANGE)
    {
        return 0;
    }
    if (*endptr != '\0' || str[0] == '\0')
    {
        return 0;
    }
    return 1;
}

int handle_h_flag(long long x)
{
    if (x == 0)
    {
        printf("No multiples of 0 found within 1 to 100.\n");
        return 0;
    }
    if (x < 0)
    {
        x = -x;
    }

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
        printf("No multiples of %lld found within 1 to 100.\n", x);
    }
    else
    {
        printf("\n");
    }
    return 0;
}

int handle_p_flag(long long x)
{
    int is_prime = 1;
    if (x <= 1)
    {
        is_prime = 0;
    }
    else
    {
        for (long long i = 2; i * i <= x; i++)
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
        printf("%lld is a prime number.\n", x);
    }
    else if (x > 1)
    {
        printf("%lld is a composite number.\n", x);
    }
    else
    {
        printf("%lld is neither prime nor composite.\n", x);
    }
    return 0;
}

int handle_s_flag(long long x)
{
    if (x == 0)
    {
        printf("0\n");
        return 0;
    }
    if (x < 0)
    {
        printf("-");
        x = -x;
    }
    char hex_digits[] = "0123456789ABCDEF";
    char hex_representation[64];
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

int handle_e_flag(int x)
{
    if (x < 0)
    {
        fprintf(stderr, "Error: x must be non-negative for -e flag.\n");
        return 4;
    }
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
                if (LLONG_MAX / result < base)
                {
                    fprintf(stderr, "Error: Calculation overflow for base %d, exponent %d.\n", base, exp);
                    return 5;
                }
                result *= base;
            }
            printf("%lld ", result);
        }
        printf("\n");
    }
    return 0;
}

int handle_a_flag(long long x)
{
    if (x < 0)
    {
        fprintf(stderr, "Error: x must be non-negative for -a flag.\n");
        return 6;
    }

    if (x > LLONG_MAX / 2)
    {
        fprintf(stderr, "Error: Input value too large for sum calculation.\n");
        return 6;
    }

    unsigned long long n = (unsigned long long)x;
    unsigned long long sum = n * (n + 1) / 2;

    printf("Sum of numbers from 1 to %lld: %llu\n", x, sum);
    return 0;
}

int handle_f_flag(int x)
{
    if (x < 0)
    {
        fprintf(stderr, "Error: Factorial is not defined for negative numbers.\n");
        return 7;
    }
    long long factorial = calculate_factorial(x);
    if (factorial == -1)
    {
        return 8;
    }
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
                return -1;
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

    long long x_ll;
    if (!is_number(x_str, &x_ll))
    {
        fprintf(stderr, "Error: Invalid number format or overflow.\n");
        return 2;
    }

    if (strcmp(flag, "-h") == 0)
    {
        return handle_h_flag(x_ll);
    }
    else if (strcmp(flag, "-p") == 0)
    {
        return handle_p_flag(x_ll);
    }
    else if (strcmp(flag, "-s") == 0)
    {
        return handle_s_flag(x_ll);
    }
    else if (strcmp(flag, "-e") == 0)
    {
        if (x_ll > INT_MAX || x_ll < 0)
        {
            fprintf(stderr, "Error: Input value is out of range for -e flag.\n");
            return 10;
        }
        return handle_e_flag((int)x_ll);
    }
    else if (strcmp(flag, "-a") == 0)
    {
        return handle_a_flag(x_ll);
    }
    else if (strcmp(flag, "-f") == 0)
    {

        if (x_ll > INT_MAX || x_ll < 0)
        {
            fprintf(stderr, "Error: Input value is out of range for -f flag.\n");
            return 11;
        }
        return handle_f_flag((int)x_ll);
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