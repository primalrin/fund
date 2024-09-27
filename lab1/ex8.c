#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BASE 36
#define MAX_NUMBER_LEN 100

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
    }

    return max_digit + 1;
}

long long convert_to_decimal(const char *str, int base)
{
    long long decimal = 0;
    long long power = 1;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        if (isdigit(str[i]))
        {
            decimal += (str[i] - '0') * power;
        }
        else if (isalpha(str[i]))
        {
            decimal += (tolower(str[i]) - 'a' + 10) * power;
        }
        power *= base;
    }
    return decimal;
}

int main()
{
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL)
    {
        perror("Ошибка открытия входного файла");
        return 1;
    }

    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL)
    {
        perror("Ошибка открытия выходного файла");
        fclose(input_file);
        return 1;
    }

    char number_str[MAX_NUMBER_LEN];

    while (fscanf(input_file, "%s", number_str) == 1)
    {
        int i = 0;
        while (number_str[i] == '0' && number_str[i + 1] != '\0')
        {
            i++;
        }

        int min_base = find_min_base(number_str + i);

        if (min_base > MAX_BASE || min_base < 2)
        {
            fprintf(stderr, "Число %s имеет недопустимое основание\n", number_str);
            continue;
        }

        long long decimal_value = convert_to_decimal(number_str + i, min_base);

        fprintf(output_file, "%s %d %lld\n", number_str + i, min_base, decimal_value);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

// gcc ex8.c -o ex8

// ./ex8.exe