#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

int handle_d(const char *input_filename, const char *output_filename);
int handle_i(const char *input_filename, const char *output_filename);
int handle_s(const char *input_filename, const char *output_filename);
int handle_a(const char *input_filename, const char *output_filename);

int handle_d(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        fprintf(stderr, "Error opening input file: %s\n", input_filename);
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        fclose(input_file);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file))
    {
        for (size_t i = 0; line[i] != '\0'; ++i)
        {
            if (!isdigit(line[i]))
            {
                fputc(line[i], output_file);
            }
        }
    }
    if (ferror(input_file))
    {
        fprintf(stderr, "Error reading input file: %s\n", input_filename);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int handle_i(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        fprintf(stderr, "Error opening input file: %s\n", input_filename);
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        fclose(input_file);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file))
    {
        int count = 0;
        for (size_t i = 0; line[i] != '\0'; ++i)
        {
            if (isalpha(line[i]))
            {
                count++;
            }
        }
        fprintf(output_file, "%d\n", count);
    }

    if (ferror(input_file))
    {
        fprintf(stderr, "Error reading input file: %s\n", input_filename);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int handle_s(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        fprintf(stderr, "Error opening input file: %s\n", input_filename);
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        fclose(input_file);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file))
    {
        int count = 0;
        for (size_t i = 0; line[i] != '\0'; ++i)
        {
            if (!isalnum(line[i]) && !isspace(line[i]))
            {
                count++;
            }
        }
        fprintf(output_file, "%d\n", count);
    }
    if (ferror(input_file))
    {
        fprintf(stderr, "Error reading input file: %s\n", input_filename);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int handle_a(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        fprintf(stderr, "Error opening input file: %s\n", input_filename);
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        fclose(input_file);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file))
    {
        for (size_t i = 0; line[i] != '\0'; ++i)
        {
            if (!isdigit(line[i]))
            {
                fprintf(output_file, "%02X", (unsigned char)line[i]);
            }
            else
            {
                fputc(line[i], output_file);
            }
        }
    }
    if (ferror(input_file))
    {
        fprintf(stderr, "Error reading input file: %s\n", input_filename);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Invalid number of arguments. Usage: %s [-d|-i|-s|-a|/-nd|-ni|-ns|-na] input_file [output_file]\n", argv[0]);
        return -1;
    }

    char *flag = argv[1];
    char *input_filename = argv[2];
    char *output_filename = NULL;
    char flag_char;

    if (strlen(flag) < 2 || (flag[0] != '-' && flag[0] != '/'))
    {
        fprintf(stderr, "Invalid flag format.\n");
        return -1;
    }

    int custom_output = 0;
    if (flag[1] == 'n')
    {
        if (argc != 4)
        {
            fprintf(stderr, "Output filename required with -n option.\n");
            return -1;
        }
        if (strlen(flag) < 3 || (flag[2] != 'd' && flag[2] != 'i' && flag[2] != 's' && flag[2] != 'a'))
        {
            fprintf(stderr, "Invalid flag format.\n");
            return -1;
        }

        output_filename = argv[3];
        custom_output = 1;
        flag_char = flag[2]; // запоминаем  символ флага
    }
    else
    {
        flag_char = flag[1]; // запоминаем символ флага
    }

    if (!custom_output)
    {
        size_t len = strlen(input_filename) + strlen("out_") + 1;
        output_filename = malloc(len);
        if (!output_filename)
        {
            perror("Memory allocation failed");
            return -1;
        }
        int ret = snprintf(output_filename, len, "out_%s", input_filename);
        if (ret < 0 || (size_t)ret >= len)
        {
            perror("snprintf failed");
            free(output_filename);
            return -1;
        }
    }

    // Используем flag_char для выбора функции обработки
    switch (flag_char)
    {
    case 'd':
        if (handle_d(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
        break;
    case 'i':
        if (handle_i(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
        break;
    case 's':
        if (handle_s(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
        break;
    case 'a':
        if (handle_a(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
        break;
    default:
        fprintf(stderr, "Invalid flag.\n");
        if (!custom_output)
            free(output_filename);
        return -1;
    }

    if (!custom_output)
        free(output_filename);

    return 0;
}

// gcc ex4.c -o ex4

// ./ex4.exe -d input4.txt
// ./ex4.exe -i input4.txt
// ./ex4.exe -s input4.txt
// ./ex4.exe -a input4.txt
// ./ex4.exe -nd input4.txt output4.txt
// ./ex4.exe -ni input4.txt output4.txt
// ./ex4.exe -ns input4.txt output4.txt
// ./ex4.exe -na input4.txt output4.txt