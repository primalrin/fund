#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SUCCESS 0
#define FAILURE -1

double integrate_a(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.5 * (log((1 + a) / a) + log((1 + b) / b));
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += log((1 + x) / x);
    }
    return h * sum;
}

double integrate_b(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.5 * (exp(-a * a / 2) + exp(-b * b / 2));
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += exp(-x * x / 2);
    }
    return h * sum;
}

double integrate_c(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.5 * (log(1.0 / (1 - a)) + log(1.0 / (1 - b)));
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += log(1.0 / (1 - x));
    }
    return h * sum;
}

double integrate_d(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.5 * (pow(a, a) + pow(b, b));
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += pow(x, x);
    }
    return h * sum;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <epsilon>\n", argv[0]);
        return FAILURE;
    }

    char *endptr;
    double epsilon = strtod(argv[1], &endptr);
    if (*endptr != '\0' || epsilon <= 0)
    {
        fprintf(stderr, "Invalid epsilon value.\n");
        return FAILURE;
    }

    int n = 1;
    double prev_result, current_result;

    // Example usage for integral a
    prev_result = integrate_a(0, 1, n);
    do
    {
        n *= 2;
        current_result = integrate_a(0, 1, n);
    } while (fabs(current_result - prev_result) > epsilon);
    printf("Integral a: %f\n", current_result);

    n = 1; // Reset n for the next integral
    prev_result = integrate_b(0, 1, n);
    do
    {
        n *= 2;
        current_result = integrate_b(0, 1, n);
    } while (fabs(current_result - prev_result) > epsilon);
    printf("Integral b: %f\n", current_result);

    n = 1; // Reset n for the next integral
    prev_result = integrate_c(0, 1, n);
    do
    {
        n *= 2;
        current_result = integrate_c(0, 1, n);
    } while (fabs(current_result - prev_result) > epsilon);
    printf("Integral c: %f\n", current_result);

    n = 1; // Reset n for the next integral
    prev_result = integrate_d(0, 1, n);
    do
    {
        n *= 2;
        current_result = integrate_d(0, 1, n);
    } while (fabs(current_result - prev_result) > epsilon);
    printf("Integral d: %f\n", current_result);

    return SUCCESS;
}