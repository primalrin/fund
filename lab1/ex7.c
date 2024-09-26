#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEME_LENGTH 1024

int read_lexeme(FILE *fp, char *lexeme)
{
    int ch;
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == EOF)
    {
        return EOF;
    }
    int i = 0;
    do
    {
        lexeme[i++] = ch;
    } while ((ch = fgetc(fp)) != EOF && !isspace(ch));
    lexeme[i] = '\0';
    return 0;
}

int write_lexeme(FILE *fp, const char *lexeme)
{
    if (fprintf(fp, "%s ", lexeme) < 0)
    {
        return -1;
    }
    return 0;
}

int process_r(const char *file1_path, const char *file2_path, const char *output_path)
{
    FILE *file1 = fopen(file1_path, "r");
    if (!file1)
    {
        perror("Error opening file1");
        return -1;
    }
    FILE *file2 = fopen(file2_path, "r");
    if (!file2)
    {
        fclose(file1);
        perror("Error opening file2");
        return -1;
    }
    FILE *output = fopen(output_path, "w");
    if (!output)
    {
        fclose(file1);
        fclose(file2);
        perror("Error opening output file");
        return -1;
    }

    char lexeme1[MAX_LEXEME_LENGTH], lexeme2[MAX_LEXEME_LENGTH];
    int eof1 = 0, eof2 = 0;
    int i = 1;

    while (!eof1 || !eof2)
    {
        if (i % 2 != 0 && !eof1)
        {
            eof1 = read_lexeme(file1, lexeme1);
            if (!eof1)
            {
                if (write_lexeme(output, lexeme1) < 0)
                {
                    fclose(file1);
                    fclose(file2);
                    fclose(output);
                    perror("Error writing to output file");
                    return -1;
                }
            }
        }
        else if (!eof2)
        {
            eof2 = read_lexeme(file2, lexeme2);
            if (!eof2)
            {
                if (write_lexeme(output, lexeme2) < 0)
                {
                    fclose(file1);
                    fclose(file2);
                    fclose(output);
                    perror("Error writing to output file");
                    return -1;
                }
            }
        }
        i++;
    }

    fclose(file1);
    fclose(file2);
    fclose(output);
    return 0;
}

// Helper function for base conversion
void to_base(char *lexeme, int base)
{
    char new_lexeme[MAX_LEXEME_LENGTH * 4] = {0}; // Increased size for longer base representations
    char *ptr = new_lexeme;
    for (int i = 0; lexeme[i] != '\0'; i++)
    {
        char buffer[10]; // Buffer to hold converted number
        sprintf(buffer, (base == 4) ? "%03o" : "%03o", lexeme[i]);
        strcat(ptr, buffer);
        ptr += strlen(buffer);
    }
    strcpy(lexeme, new_lexeme);
}

int process_a(const char *input_path, const char *output_path)
{
    // Implementation for -a flag (similar structure to process_r)
    // ...
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Insufficient arguments\n");
        return -1;
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        if (argc != 6)
        {
            fprintf(stderr, "Incorrect number of arguments for -r flag\n");
            return -1;
        }
        return process_r(argv[3], argv[4], argv[5]);
    }
    else if (strcmp(argv[1], "-a") == 0)
    {
        if (argc != 5)
        {
            fprintf(stderr, "Incorrect number of arguments for -a flag\n");
            return -1;
        }
        return process_a(argv[3], argv[4]);
    }
    else
    {
        fprintf(stderr, "Invalid flag\n");
        return -1;
    }
}