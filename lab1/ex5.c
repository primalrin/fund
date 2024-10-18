#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

enum ret_type_t
{
    SUCCESS,    // Успех
    ERROR_NAN,  // Не является числом
    ERROR_FULL, // Переполнение
    ERROR_ARGS, // Количество аргументов неверное
    ERROR_EPS   // Слишком большое значение эпсилон
};
typedef int (*callback)(double, double);

// Перевод из строкового представления в double-овое
double chToD(char *x)
{
    double xD = 0;
    double m = 10;
    double mBuf = m;
    int flag = 1;

    for (int i = 0; i < (int)strlen(x); ++i)
    {
        if (x[i] == '.')
        {
            flag = 0;
            m = 10.0;
            mBuf = m;
            continue;
        }
        if (flag)
        {
            xD = xD * m + (x[i] - '0');
        }
        else
        {
            xD = xD + ((double)(x[i] - '0')) / m;
            m *= 10.0;
            if (mBuf > m)
            {
                return ERROR_FULL;
            }
            mBuf = m;
        }
    }
    return xD;
}

// Вычисляет сумму под буквой a
int funcForA(double eps, double x)
{
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i)
    {
        double nowEl = prevEl * x / (double)i;
        double nowSum = prevSum + nowEl;
        if ((fabs(nowSum - prevSum) < eps) || (fabs(nowEl) < eps))
        {
            printf("The result of sum A is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой b
int funcForB(double eps, double x)
{
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i)
    {
        double nowEl = prevEl * (-1.0) * x * x / (2 * (i - 1) + 1) / (2 * i);
        double nowSum = prevSum + nowEl;
        if ((fabs(nowSum - prevSum) < eps) || (fabs(nowEl) < eps))
        {
            printf("The result of sum B is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой c
int funcForC(double eps, double x)
{
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i)
    {
        double nowEl = prevEl * 27 * pow(i, 3) * x * x / (3 * (i - 1) + 1) / (3 * (i - 1) + 2) / (3 * i);
        double nowSum = prevSum + nowEl;
        if ((fabs(nowSum - prevSum) < eps) || (fabs(nowEl) < eps))
        {
            printf("The result of sum C is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой d
int funcForD(double eps, double x)
{
    double prevEl = -0.5 * x * x;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i)
    {
        double nowEl = prevEl * (-1.0) * (x * x) * (2.0 * (double)(i - 1) + 1) / 2.0 / (double)i;
        double nowSum = prevSum + nowEl;
        if ((fabs(nowSum - prevSum) < eps) || (fabs(nowEl) < eps))
        {
            printf("The result of sum D is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Проверяет является ли строковое представление вещественным числом
int checkNumber(char *x)
{
    int flag = 0;
    for (int i = 0; i < (int)strlen(x); i++)
    {
        if ((x[i] < '0' || x[i] > '9') && (x[i] != '.'))
        {
            return ERROR_NAN;
        }
        if (x[i] == '.')
        {
            flag++;
            if (flag > 1)
            {
                return ERROR_NAN;
            }
        }
    }
    return SUCCESS;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Wrong number of arguments");
        return ERROR_ARGS;
    }

    if (checkNumber(argv[1]) || checkNumber(argv[2]))
    {
        printf("There must be an epsilon and x");
        return ERROR_NAN;
    }

    if (chToD(argv[1]) == ERROR_FULL && ((int)strlen(argv[1]) > 3))
    {
        printf("Your number is too big");
        return ERROR_FULL;
    }
    if (chToD(argv[2]) == ERROR_FULL && ((int)strlen(argv[2]) > 3))
    {
        printf("Your number is too big");
        return ERROR_FULL;
    }

    double eps = chToD(argv[1]);
    double x = chToD(argv[2]);
    callback cbs[] = {&funcForA, &funcForB, &funcForC, &funcForD};
    for (int i = 0; i < 4; ++i)
    {
        callback find = cbs[i];

        if (find(eps, x))
        {
            printf("Your epsilon is too big for current sum");
            return ERROR_EPS;
        }
    }

    return 0;
}

// gcc ex5.c -o ex5

// ./ex5.exe 0.00001 1.5