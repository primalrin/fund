#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#define MAX_BASE 36
#define MAX_NUMBER_LEN 101

int find_min_base(const char *str)
{
    int max_digit = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
        {
            max_digit = (str[i] - '0' > max_digit) ? str[i] - '0' : max_digit;
        }
        else if (isalpha(str[i]))
        {
            max_digit = (tolower(str[i]) - 'a' + 10 > max_digit) ? tolower(str[i]) - 'a' + 10 : max_digit;
        }
        else
        {
            return -1; // Invalid character
        }
    }
    return (max_digit < 1) ? 2 : max_digit + 1;
}

long long convert_to_decimal(const char *str, int base)
{
    long long decimal = 0;
    long long power = 1;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        int digit;
        if (isdigit(str[i]))
        {
            digit = str[i] - '0';
        }
        else
        {
            digit = tolower(str[i]) - 'a' + 10;
        }

        if (decimal > LLONG_MAX / base || (decimal == LLONG_MAX / base && digit > LLONG_MAX % base))
        {
            errno = ERANGE;
            return -1; // Overflow
        }
        decimal += digit * power;
        if (power > LLONG_MAX / base)
        {
            errno = ERANGE;
            return -1; // Overflow
        }

        power *= base;
    }
    return decimal;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "Error: Input and output files must be different.\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    char number_str[MAX_NUMBER_LEN];

    while (fscanf(input_file, "%100s", number_str) == 1)
    {
        int i = 0;
        while (number_str[i] == '0' && number_str[i + 1] != '\0')
        {
            i++;
        }

        if (number_str[i] == '\0')
        {
            fprintf(output_file, "0 2 0\n");
            continue;
        }

        int min_base = find_min_base(number_str + i);

        if (min_base == -1 || min_base > MAX_BASE || min_base < 2)
        {
            fprintf(stderr, "Invalid number: %s\n", number_str);
            continue; // Skip invalid numbers
        }

        long long decimal_value = convert_to_decimal(number_str + i, min_base);
        if (decimal_value == -1 && errno == ERANGE)
        {
            fprintf(stderr, "Overflow detected for number: %s\n", number_str);
            continue;
        }

        fprintf(output_file, "%s %d %lld\n", number_str + i, min_base, decimal_value);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

// gcc ex8.c -o ex8

// ./ex8.exe input8.txt output8.txt