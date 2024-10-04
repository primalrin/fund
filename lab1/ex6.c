#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000

double integrate_trapezoidal(double a, double b, int n, double (*f)(double))
{
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += f(x);
    }
    return h * sum;
}

double f_a(double x) { return log((1 + x) / x); }
double f_b(double x) { return exp(-x * x / 2); }
double f_c(double x) { return log(1.0 / (1 - x)); }
double f_d(double x) { return pow(x, x); }

double integrate(double a, double b, double epsilon, double (*f)(double))
{
    int n = 1;
    int iteration = 0;
    double prev_result = integrate_trapezoidal(a, b, n, f);
    do
    {
        n *= 2;
        double current_result = integrate_trapezoidal(a, b, n, f);
        if (fabs(current_result) > 1e100 || fabs(prev_result) > 1e100)
        {
            fprintf(stderr, "Integral diverges.\n");
            exit(-1);
        }
        iteration++;
        if (iteration == MAX_ITERATIONS)
        {
            fprintf(stderr, "Integral did not converge.\n");
            exit(-1);
        }

        prev_result = current_result;

    } while (fabs(prev_result - integrate_trapezoidal(a, b, n * 2, f)) > epsilon);
    return prev_result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <epsilon>\n", argv[0]);
        return -1;
    }

    char *endptr;
    double epsilon = strtod(argv[1], &endptr);
    if (*endptr != '\0' || epsilon <= 0)
    {
        fprintf(stderr, "Invalid epsilon value.\n");
        return -1;
    }

    printf("Integral a: %f\n", integrate(0.000000001, 1, epsilon, f_a));
    printf("Integral b: %f\n", integrate(0, 1, epsilon, f_b));
    printf("Integral c: %f\n", integrate(0, 0.999999999, epsilon, f_c));
    printf("Integral d: %f\n", integrate(0, 1, epsilon, f_d));

    return 0;
}

// gcc ex6.c -o ex6

// ./ex6.exe 0.00001