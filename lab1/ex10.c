#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define MAX_BASE 36
#define MAX_STR_LEN 100

int str_to_long(const char *str, int base, long *result)
{
    if (base < 2 || base > MAX_BASE)
    {
        return 1;
    }

    *result = 0;
    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    while (*str != '\0')
    {
        int digit;
        if (isdigit(*str))
        {
            digit = *str - '0';
        }
        else if (isalpha(*str) && isupper(*str))
        {
            digit = *str - 'A' + 10;
        }
        else
        {
            return 1;
        }

        if (digit >= base)
        {
            return 1;
        }

        if (*result > (LONG_MAX - digit) / base)
        {
            if (sign == 1)
            {
                *result = LONG_MAX;
            }
            else
            {
                *result = LONG_MIN;
            }
            return 2;
        }

        *result = *result * base + digit;
        str++;
    }
    *result *= sign;

    return 0;
}

char *long_to_str(long num, int base)
{
    if (base < 2 || base > MAX_BASE)
    {
        return NULL;
    }

    char buffer[MAX_STR_LEN];
    int i = 0;
    long n = num;

    if (n == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        int sign = (n < 0) ? -1 : 1;
        n = labs(n);
        while (n > 0)
        {
            int remainder = n % base;
            buffer[i++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
            n /= base;
        }
        if (sign == -1)
        {
            buffer[i++] = '-';
        }
    }

    buffer[i] = '\0';

    char *result = (char *)malloc(sizeof(char) * (i + 1));
    if (!result)
    {
        return NULL;
    }

    int j, k;
    for (j = 0, k = i - 1; k >= 0; j++, k--)
    {
        result[j] = buffer[k];
    }
    result[j] = '\0';
    return result;
}

int main()
{
    int base;

    if (scanf("%d", &base) != 1 || base < 2 || base > MAX_BASE)
    {
        printf("Invalid base\n");
        return 1;
    }

    char input[MAX_STR_LEN];
    long max_abs = 0;
    char *max_abs_str = NULL;
    long current_num;

    while (scanf("%s", input) == 1)
    {
        if (strcmp(input, "Stop") == 0)
        {
            break;
        }

        int status = str_to_long(input, base, current_num);
        if (status == 1)
        {
            printf("Invalid number: %s\n", input);
            continue;
        }
        else if (status == 2)
        {
            printf("Overflow detected: %s\n", input);
        }

        if (llabs(current_num) > llabs(max_abs))
        {
            max_abs = current_num;
            free(max_abs_str);
            max_abs_str = strdup(input);
            if (!max_abs_str)
            {
                perror("Memory allocation failed");
                return 1;
            }
        }
    }

    if (max_abs_str == NULL)
    {
        printf("No numbers entered\n");
        return 0;
    }

    printf("%s\n", max_abs_str);

    for (int b = 9; b <= 36; b += 9)
    {
        char *converted = long_to_str(max_abs, b);
        if (converted)
        {
            printf("%s\n", converted);
            free(converted);
        }
        else
        {
            printf("Conversion error (base %d)\n", b);
        }
    }
    free(max_abs_str);
    return 0;
}