#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Структура для представления узла списка (одночлена)
typedef struct Node
{
    int coefficient;
    int exponent;
    struct Node *next;
} Node;

// Структура для представления многочлена
typedef struct Polynomial
{
    Node *head;
} Polynomial;

// Функция для создания нового узла списка
Node *createNode(int coefficient, int exponent)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    newNode->coefficient = coefficient;
    newNode->exponent = exponent;
    newNode->next = NULL;
    return newNode;
}

// Функция для создания пустого многочлена
Polynomial *createPolynomial()
{
    Polynomial *poly = (Polynomial *)malloc(sizeof(Polynomial));
    if (!poly)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    poly->head = NULL;
    return poly;
}

// Функция для добавления одночлена в многочлен
int addTerm(Polynomial *poly, int coefficient, int exponent)
{
    Node *newNode = createNode(coefficient, exponent);
    if (!newNode)
    {
        return -1;
    }

    if (!poly->head || exponent > poly->head->exponent)
    {
        newNode->next = poly->head;
        poly->head = newNode;
    }
    else
    {
        Node *current = poly->head;
        while (current->next && exponent < current->next->exponent)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    return 0;
}

// Функция для печати многочлена
void printPolynomial(const Polynomial *poly)
{
    if (!poly->head)
    {
        printf("0\n");
        return;
    }

    Node *current = poly->head;
    while (current)
    {
        if (current->coefficient != 0)
        {
            if (current->coefficient > 0 && current != poly->head)
            {
                printf("+");
            }
            if (current->coefficient != 1 || current->exponent == 0)
            {
                printf("%d", current->coefficient);
            }
            if (current->exponent > 0)
            {
                printf("x");
                if (current->exponent > 1)
                {
                    printf("^%d", current->exponent);
                }
            }
        }
        current = current->next;
    }
    printf("\n");
}

// Функция для добавления одночлена в многочлен (из предыдущего ответа)
int addTerm(Polynomial *poly, int coefficient, int exponent)
{
    // ... (реализация) ...
}

// Функция для печати многочлена (из предыдущего ответа)
void printPolynomial(const Polynomial *poly)
{
    // ... (реализация) ...
}

// Функция для копирования многочлена
Polynomial *copyPolynomial(const Polynomial *poly)
{
    Polynomial *newPoly = createPolynomial();
    if (!newPoly)
    {
        return NULL;
    }
    Node *current = poly->head;
    while (current)
    {
        if (addTerm(newPoly, current->coefficient, current->exponent) != 0)
        {
            // Ошибка при добавлении члена - освобождаем память и возвращаем NULL
            destroyPolynomial(newPoly);
            return NULL;
        }
        current = current->next;
    }
    return newPoly;
}

// Функция для освобождения памяти, занимаемой многочленом
void destroyPolynomial(Polynomial *poly)
{
    Node *current = poly->head;
    while (current)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(poly);
}

// Функция для сложения двух многочленов
Polynomial *addPolynomials(const Polynomial *poly1, const Polynomial *poly2)
{
    Polynomial *result = createPolynomial();
    if (!result)
    {
        return NULL;
    }
    Node *current1 = poly1->head;
    Node *current2 = poly2->head;

    while (current1 || current2)
    {
        if (!current1 || (current2 && current2->exponent > current1->exponent))
        {
            if (addTerm(result, current2->coefficient, current2->exponent) != 0)
            {
                destroyPolynomial(result);
                return NULL;
            }
            current2 = current2->next;
        }
        else if (!current2 || (current1 && current1->exponent > current2->exponent))
        {
            if (addTerm(result, current1->coefficient, current1->exponent) != 0)
            {
                destroyPolynomial(result);
                return NULL;
            }
            current1 = current1->next;
        }
        else
        {
            if (addTerm(result, current1->coefficient + current2->coefficient, current1->exponent) != 0)
            {
                destroyPolynomial(result);
                return NULL;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
    }
    return result;
}

// Функция для вычитания двух многочленов
Polynomial *subtractPolynomials(const Polynomial *poly1, const Polynomial *poly2)
{
    Polynomial *negPoly2 = copyPolynomial(poly2); // Создаем копию poly2
    if (!negPoly2)
    {
        return NULL;
    }
    Node *current = negPoly2->head;
    while (current)
    {
        current->coefficient *= -1; // Инвертируем знаки коэффициентов
        current = current->next;
    }
    Polynomial *result = addPolynomials(poly1, negPoly2);
    destroyPolynomial(negPoly2); // Освобождаем память копии
    return result;
}

// Функция для умножения двух многочленов
Polynomial *multiplyPolynomials(const Polynomial *poly1, const Polynomial *poly2)
{
    Polynomial *result = createPolynomial();
    if (!result)
    {
        return NULL;
    }

    Node *current1 = poly1->head;
    while (current1)
    {
        Node *current2 = poly2->head;
        while (current2)
        {
            int newCoefficient = current1->coefficient * current2->coefficient;
            int newExponent = current1->exponent + current2->exponent;
            if (addTerm(result, newCoefficient, newExponent) != 0)
            {
                destroyPolynomial(result);
                return NULL;
            }
            current2 = current2->next;
        }
        current1 = current1->next;
    }

    // Упрощение результата (объединение одночленов с одинаковыми степенями)
    result = simplifyPolynomial(result);
    return result;
}

// Функция для упрощения многочлена (объединение одночленов с одинаковыми степенями)
Polynomial *simplifyPolynomial(Polynomial *poly)
{
    if (!poly || !poly->head)
    {
        return poly; // Пустой многочлен или NULL - ничего не делаем
    }

    Polynomial *simplified = createPolynomial();
    if (!simplified)
    {
        return NULL; // Ошибка выделения памяти
    }

    Node *current = poly->head;
    while (current)
    {
        int coefficient = current->coefficient;
        int exponent = current->exponent;
        Node *next = current->next;
        while (next)
        {
            if (next->exponent == exponent)
            {
                coefficient += next->coefficient;
                next = next->next;
            }
            else
            {
                break;
            }
        }
        if (coefficient != 0)
        { // Добавляем только ненулевые коэффициенты
            if (addTerm(simplified, coefficient, exponent) != 0)
            {
                destroyPolynomial(simplified);
                return NULL; // Ошибка выделения памяти
            }
        }
        current = next;
    }

    destroyPolynomial(poly); // Освобождаем память исходного многочлена
    return simplified;
}

// Функция для целочисленного деления многочленов (Div)
Polynomial *dividePolynomials(const Polynomial *dividend, const Polynomial *divisor, Polynomial **remainder)
{
    if (!dividend || !divisor)
    {
        fprintf(stderr, "Error: NULL polynomial passed to dividePolynomials\n");
        return NULL;
    }

    if (!divisor->head || divisor->head->coefficient == 0)
    {
        fprintf(stderr, "Error: Division by zero polynomial\n");
        return NULL;
    }

    Polynomial *quotient = createPolynomial();
    if (!quotient)
    {
        return NULL; // Ошибка выделения памяти
    }

    *remainder = copyPolynomial(dividend); // Изначально остаток равен делимому
    if (!*remainder)
    {
        destroyPolynomial(quotient);
        return NULL; // Ошибка выделения памяти
    }

    while (*remainder && (*remainder)->head && (*remainder)->head->exponent >= divisor->head->exponent)
    {
        int coefficient = (*remainder)->head->coefficient / divisor->head->coefficient;
        int exponent = (*remainder)->head->exponent - divisor->head->exponent;

        if (addTerm(quotient, coefficient, exponent) != 0)
        {
            destroyPolynomial(quotient);
            destroyPolynomial(*remainder);
            *remainder = NULL;
            return NULL; // Ошибка выделения памяти
        }

        Polynomial *term = createPolynomial();
        if (!term || addTerm(term, coefficient, exponent) != 0)
        {
            destroyPolynomial(quotient);
            destroyPolynomial(*remainder);
            destroyPolynomial(term);
            *remainder = NULL;
            return NULL; // Ошибка выделения памяти
        }

        Polynomial *product = multiplyPolynomials(term, divisor);
        if (!product)
        {
            destroyPolynomial(quotient);
            destroyPolynomial(*remainder);
            destroyPolynomial(term);
            *remainder = NULL;
            return NULL;
        }

        Polynomial *tempRemainder = subtractPolynomials(*remainder, product);
        destroyPolynomial(*remainder);
        *remainder = tempRemainder;
        destroyPolynomial(term);
        destroyPolynomial(product);

        *remainder = simplifyPolynomial(*remainder);
    }

    return quotient;
}

// Функция для нахождения остатка от деления многочленов (Mod)
Polynomial *moduloPolynomials(const Polynomial *dividend, const Polynomial *divisor)
{
    Polynomial *remainder;
    dividePolynomials(dividend, divisor, &remainder); // Деление, остаток сохраняется в remainder
    return remainder;
}

// Функция для вычисления значения многочлена в точке (Eval)
int evaluatePolynomial(const Polynomial *poly, int x)
{
    int result = 0;
    Node *current = poly->head;
    while (current)
    {
        result += current->coefficient * pow(x, current->exponent);
        current = current->next;
    }
    return result;
}

// Функция для нахождения производной многочлена (Diff)
Polynomial *differentiatePolynomial(const Polynomial *poly)
{
    Polynomial *derivative = createPolynomial();
    if (!derivative)
    {
        return NULL;
    }

    Node *current = poly->head;
    while (current)
    {
        if (current->exponent > 0)
        {
            if (addTerm(derivative, current->coefficient * current->exponent, current->exponent - 1) != 0)
            {
                destroyPolynomial(derivative);
                return NULL;
            }
        }
        current = current->next;
    }

    return derivative;
}

// Функция для композиции многочленов (Cmps)
Polynomial *composePolynomials(const Polynomial *poly1, const Polynomial *poly2)
{
    Polynomial *result = createPolynomial();
    if (!result)
    {
        return NULL;
    }

    Node *current1 = poly1->head;
    while (current1)
    {
        Polynomial *termResult = createPolynomial();
        if (!termResult)
        {
            destroyPolynomial(result);
            return NULL;
        }

        if (addTerm(termResult, current1->coefficient, 0) != 0)
        { // Добавляем коэффициент как константу
            destroyPolynomial(result);
            destroyPolynomial(termResult);
            return NULL;
        }

        for (int i = 0; i < current1->exponent; ++i)
        {
            Polynomial *temp = multiplyPolynomials(termResult, poly2);
            destroyPolynomial(termResult);
            termResult = temp;
            if (!termResult)
            {
                destroyPolynomial(result);
                return NULL;
            }
        }

        Polynomial *tempResult = addPolynomials(result, termResult);
        destroyPolynomial(result);
        destroyPolynomial(termResult);
        result = tempResult;
        if (!result)
        {
            return NULL;
        }

        current1 = current1->next;
    }

    return result;
}

// Функция для разбора многочлена из строки
Polynomial *parsePolynomial(const char *str)
{
    Polynomial *poly = createPolynomial();
    if (!poly)
    {
        return NULL;
    }

    int i = 0;
    while (str[i] != '\0')
    {
        int sign = 1;
        int coefficient = 0;
        int exponent = 0;

        // Пропускаем пробелы
        while (isspace(str[i]))
        {
            i++;
        }

        // Определяем знак
        if (str[i] == '+')
        {
            i++;
        }
        else if (str[i] == '-')
        {
            sign = -1;
            i++;
        }

        // Считываем коэффициент
        if (isdigit(str[i]))
        {
            while (isdigit(str[i]))
            {
                coefficient = coefficient * 10 + (str[i] - '0');
                i++;
            }
        }
        else if (str[i] == 'x')
        {
            coefficient = 1;
        }
        else
        {
            // Ошибка: неверный формат многочлена
            fprintf(stderr, "Error: Invalid polynomial format\n");
            destroyPolynomial(poly);
            return NULL;
        }

        // Пропускаем пробелы
        while (isspace(str[i]))
        {
            i++;
        }

        // Считываем степень
        if (str[i] == 'x')
        {
            i++;
            if (str[i] == '^')
            {
                i++;
                if (isdigit(str[i]))
                {
                    while (isdigit(str[i]))
                    {
                        exponent = exponent * 10 + (str[i] - '0');
                        i++;
                    }
                }
                else
                {
                    // Ошибка: неверный формат многочлена
                    fprintf(stderr, "Error: Invalid polynomial format\n");
                    destroyPolynomial(poly);
                    return NULL;
                }
            }
            else
            {
                exponent = 1;
            }
        }
        else
        {
            exponent = 0; // Если нет 'x', то это константа
        }

        // Добавляем член в многочлен
        if (addTerm(poly, sign * coefficient, exponent) != 0)
        {
            destroyPolynomial(poly);
            return NULL;
        }

        // Пропускаем пробелы
        while (isspace(str[i]))
        {
            i++;
        }
    }

    return poly;
}

// Функция для чтения инструкций из файла и обработки ошибок
int processFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    Polynomial *accumulator = createPolynomial(); // Инициализируем аккумулятор нулевым многочленом

    while (fgets(line, sizeof(line), file))
    {
        // ... (Реализация обработки строк: комментарии, инструкции) ...
    }

    destroyPolynomial(accumulator);
    fclose(file);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (processFile(argv[1]) != 0)
    {
        return 1;
    }

    return 0;
}