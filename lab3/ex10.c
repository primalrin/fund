#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_CHILDREN 100

// Структура узла дерева
typedef struct Node
{
    char value;                          // Значение узла
    struct Node *children[MAX_CHILDREN]; // Массив указателей на дочерние узлы
    int child_count;                     // Количество дочерних узлов
} Node;

// Создание нового узла
Node *createNode(char value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->value = value;
    node->child_count = 0;
    memset(node->children, 0, sizeof(node->children));
    return node;
}

// Освобождение памяти дерева
void freeTree(Node *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->child_count; i++)
    {
        freeTree(root->children[i]);
    }
    free(root);
}

// Парсинг выражения и построение дерева
Node *parseExpression(const char **expr)
{
    // Пропуск пробелов
    while (**expr == ' ')
        (*expr)++;

    if (**expr == '\0' || **expr == ')' || **expr == ',')
        return NULL;

    // Создание узла для текущего символа
    Node *node = createNode(**expr);
    if (node == NULL)
    {
        return NULL;
    }
    (*expr)++;

    // Пропуск пробелов
    while (**expr == ' ')
        (*expr)++;

    // Если после символа идет открывающая скобка
    if (**expr == '(')
    {
        (*expr)++; // Пропуск '('

        // Чтение всех дочерних узлов
        while (1)
        {
            Node *child = parseExpression(expr);
            if (child != NULL)
            {
                node->children[node->child_count++] = child;
            }

            // Пропуск пробелов
            while (**expr == ' ')
                (*expr)++;

            if (**expr == ')')
            {
                (*expr)++;
                break;
            }
            else if (**expr == ',')
            {
                (*expr)++;
            }
        }
    }

    return node;
}

// Вспомогательная функция для определения ширины узла
int getTreeWidth(Node *root)
{
    if (root == NULL)
        return 0;

    int width = 1;
    for (int i = 0; i < root->child_count; i++)
    {
        width += getTreeWidth(root->children[i]);
    }
    return width;
}

// Печать дерева в файл с отступами
void printTree(FILE *out, Node *root, int level, int *positions, int pos)
{
    if (root == NULL)
        return;

    // Сохранение текущей позиции
    positions[level] = pos;

    // Печать текущего уровня
    fprintf(out, "\n");
    for (int i = 0; i < level; i++)
    {
        for (int j = 0; j < positions[i]; j++)
        {
            fprintf(out, " ");
        }
        fprintf(out, "|");
    }
    for (int i = 0; i < pos; i++)
    {
        fprintf(out, " ");
    }
    fprintf(out, "%c", root->value);

    // Вычисление отступов для дочерних узлов
    int totalWidth = getTreeWidth(root);
    int currentPos = 0;

    // Рекурсивная печать всех дочерних узлов
    for (int i = 0; i < root->child_count; i++)
    {
        int childWidth = getTreeWidth(root->children[i]);
        printTree(out, root->children[i], level + 1, positions,
                  (totalWidth - currentPos - childWidth) / 2);
        currentPos += childWidth;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    // Открытие входного файла
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Cannot open input file: %s\n", argv[1]);
        return 1;
    }

    // Открытие выходного файла
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Cannot open output file: %s\n", argv[2]);
        fclose(input);
        return 1;
    }

    char line[MAX_LINE];
    int tree_number = 1;
    int positions[MAX_LINE] = {0}; // Массив для хранения позиций узлов

    // Обработка каждой строки входного файла
    while (fgets(line, sizeof(line), input))
    {
        // Удаление символа новой строки
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // Пропуск пустых строк
        if (strlen(line) == 0)
            continue;

        fprintf(output, "\nTree #%d for expression: %s\n", tree_number++, line);

        // Парсинг выражения и построение дерева
        const char *expr = line;
        Node *root = parseExpression(&expr);

        // Печать дерева
        if (root != NULL)
        {
            printTree(output, root, 0, positions, 0);
            fprintf(output, "\n");
            freeTree(root);
        }
        else if (strlen(line) != 0)
        {
            fprintf(output, "Memory error\n");
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}