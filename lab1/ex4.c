#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

// Function to handle the -d flag (remove digits)
int handle_d(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        perror("Error opening input file");
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        perror("Error opening output file");
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

    fclose(input_file);
    fclose(output_file);
    return 0;
}

// Function to handle the -i flag (count Latin alphabet characters)
int handle_i(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        perror("Error opening input file");
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        perror("Error opening output file");
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

    fclose(input_file);
    fclose(output_file);
    return 0;
}

// Function to handle the -s flag (count non-alphanumeric/space characters)
int handle_s(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        perror("Error opening input file");
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        perror("Error opening output file");
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

    fclose(input_file);
    fclose(output_file);
    return 0;
}

// Function to handle the -a flag (replace non-digits with hex ASCII code)
int handle_a(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        perror("Error opening input file");
        return -1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        perror("Error opening output file");
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
                fprintf(output_file, "%X", (unsigned char)line[i]); // Use capital X for uppercase hex
            }
            else
            {
                fputc(line[i], output_file);
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }

    char *flag = argv[1];
    char *input_filename = argv[2];
    char *output_filename = NULL;

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
        output_filename = argv[3];
        custom_output = 1;
    }
    else
    {
        // Generate output filename
        size_t len = strlen(input_filename) + strlen("out_") + 1;
        output_filename = malloc(len);
        if (!output_filename)
        {
            perror("Memory allocation failed");
            return -1;
        }
        snprintf(output_filename, len, "out_%s", input_filename);
    }

    // Process based on the flag
    if (strcmp(flag, "-d") == 0 || strcmp(flag, "/d") == 0 || strcmp(flag, "-nd") == 0 || strcmp(flag, "/nd") == 0)
    {
        if (handle_d(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
    }
    else if (strcmp(flag, "-i") == 0 || strcmp(flag, "/i") == 0 || strcmp(flag, "-ni") == 0 || strcmp(flag, "/ni") == 0)
    {
        if (handle_i(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
    }
    else if (strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0 || strcmp(flag, "-ns") == 0 || strcmp(flag, "/ns") == 0)
    {
        if (handle_s(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
    }
    else if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0 || strcmp(flag, "-na") == 0 || strcmp(flag, "/na") == 0)
    {
        if (handle_a(input_filename, output_filename) != 0)
        {
            if (!custom_output)
                free(output_filename);
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "Invalid flag.\n");
        if (!custom_output)
            free(output_filename);
        return -1;
    }

    if (!custom_output)
        free(output_filename);

    return 0;
}