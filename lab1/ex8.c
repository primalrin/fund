#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBER_LENGTH 100

int char_to_digit(char c)
{
    if (isdigit(c))
    {
        return c - '0';
    }
    else if (isalpha(c))
    {
        return tolower(c) - 'a' + 10;
    }
    return -1;
}

int determine_min_base(const char *number_str)
{
    int max_digit = -1;
    for (int i = 0; number_str[i] != '\0'; ++i)
    {
        int digit = char_to_digit(number_str[i]);
        if (digit == -1)
        {
            return -1;
        }
        if (digit > max_digit)
        {
            max_digit = digit;
        }
    }
    return max_digit < 1 ? 2 : max_digit + 1;
}

long long convert_to_decimal(const char *number_str, int base)
{
    long long decimal_value = 0;
    long long power = 1;
    for (int i = strlen(number_str) - 1; i >= 0; --i)
    {
        int digit = char_to_digit(number_str[i]);
        if (digit == -1 || digit >= base)
        {
            return -1;
        }
        decimal_value += digit * power;
        power *= base;
    }
    return decimal_value;
}

int process_number(const char *number_str, FILE *output_file)
{
    int min_base = determine_min_base(number_str);
    if (min_base == -1)
    {
        return -1;
    }

    long long decimal_value = convert_to_decimal(number_str, min_base);
    if (decimal_value == -1)
    {
        return -1;
    }

    int start_index = 0;
    while (number_str[start_index] == '0' && number_str[start_index + 1] != '\0')
    {
        start_index++;
    }

    fprintf(output_file, "%s %d %lld\n", number_str + start_index, min_base, decimal_value);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file)
    {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    char number_str[MAX_NUMBER_LENGTH];
    while (fscanf(input_file, "%s", number_str) == 1)
    {

        if (process_number(number_str, output_file) != 0)
        {
            fprintf(stderr, "Invalid number format: %s\n", number_str);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}