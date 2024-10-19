#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum ret_type_t {
    SUCCESS,
    ERROR_NAN,
    ERROR_FULL,
    ERROR_ARGS,
    ERROR_EPS
};

typedef int (*callback)(double, double);

// Вычисляет сумму под буквой a
int funcForA(double eps, double x) {
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i) {
        double nowEl = prevEl * x / i;
        double nowSum = prevSum + nowEl;
        if (fabs(nowSum - prevSum) < eps || fabs(nowEl) < eps) {
            printf("The result of sum A is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой b
int funcForB(double eps, double x) {
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i) {
        double nowEl = prevEl * (-1.0) * x * x / (2 * (i - 1) + 1) / (2 * i);
        double nowSum = prevSum + nowEl;
        if (fabs(nowSum - prevSum) < eps || fabs(nowEl) < eps) {
            printf("The result of sum B is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой c
int funcForC(double eps, double x) {
    double prevEl = 1.0;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i) {
        double nowEl = prevEl * 27 * pow(i, 3) * x * x / (3 * (i - 1) + 1) / (3 * (i - 1) + 2) / (3 * i);
        double nowSum = prevSum + nowEl;
        if (fabs(nowSum - prevSum) < eps || fabs(nowEl) < eps) {
            printf("The result of sum C is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}

// Вычисляет сумму под буквой d
int funcForD(double eps, double x) {
    double prevEl = -0.5 * x * x;
    double prevSum = prevEl;

    for (int i = 1; i < 1000000; ++i) {
        double nowEl = prevEl * (-1.0) * (x * x) * (2.0 * (i - 1) + 1) / 2.0 / i;
        double nowSum = prevSum + nowEl;
        if (fabs(nowSum - prevSum) < eps || fabs(nowEl) < eps) {
            printf("The result of sum D is: %f\n", nowSum);
            return SUCCESS;
        }
        prevEl = nowEl;
        prevSum = nowSum;
    }
    return ERROR_EPS;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return ERROR_ARGS;
    }

    char *endptr_eps;
    char *endptr_x;

    double eps = strtod(argv[1], &endptr_eps);
    if (*endptr_eps != '\0') {
        printf("Invalid epsilon value.\n");
        return ERROR_NAN;
    }

    double x = strtod(argv[2], &endptr_x);
    if (*endptr_x != '\0') {
        printf("Invalid x value.\n");
        return ERROR_NAN;
    }


    callback cbs[] = {&funcForA, &funcForB, &funcForC, &funcForD};
    for (int i = 0; i < 4; ++i) {
        callback find = cbs[i];

        if (find(eps, x)) {
            printf("Your epsilon is too big for current sum\n");
            return ERROR_EPS;
        }
    }

    return 0;
}

// gcc ex5.c -o ex5

// ./ex5.exe 0.00001 1.5
