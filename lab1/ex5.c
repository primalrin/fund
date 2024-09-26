#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Function to calculate the factorial using tgamma
double factorial(int n)
{
    if (n < 0)
    {
        return -1; // Indicate error for negative input
    }
    return tgamma(n + 1);
}

double double_factorial(int n)
{
    if (n < -1)
        return -1;
    if (n <= 1)
        return 1;
    double res = 1.0;
    for (int i = n; i >= 2; i -= 2)
    {
        res *= i;
    }
    return res;
}

// Function to calculate the first sum
int calculate_sum_a(double x, double epsilon, double *result)
{
    double sum = 0.0;
    double term;
    int n = 0;

    do
    {
        double fact_val = factorial(n);
        if (fact_val == -1)
        {
            return 1; // Indicate error in factorial calculation
        }
        term = pow(x, n) / fact_val;
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int calculate_sum_b(double x, double epsilon, double *result)
{
    double sum = 0.0;
    double term;
    int n = 0;
    do
    {
        double fact_val = factorial(2 * n);
        if (fact_val == -1)
        {
            return 1;
        }

        term = pow(-1, n) * pow(x, 2 * n) / fact_val;
        sum += term;
        n++;
    } while (fabs(term) > epsilon);
    *result = sum;
    return 0;
}
int calculate_sum_c(double x, double epsilon, double *result)
{
    double sum = 1.0;
    double term = 1.0;
    int n = 1;
    do
    {
        term *= 27 * x * x / ((3 * n - 2) * (3 * n - 1) * (3 * n));
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int calculate_sum_d(double x, double epsilon, double *result)
{
    double sum = 0.0;
    double term = -x * x / 2; // Initialize first term (n=1)
    int n = 1;

    do
    {
        sum += term;
        n++;
        term *= -x * x * (2.0 * n - 1.0) / (2.0 * n * (2.0 * n - 2.0)); // Correct iterative calculation
    } while (fabs(term) > epsilon);

    *result = sum;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <x_value> <epsilon_value>\n", argv[0]);
        return 1;
    }

    char *endptr;
    double x = strtod(argv[1], &endptr);
    if (*endptr != '\0' || x == HUGE_VAL || x == -HUGE_VAL)
    {
        fprintf(stderr, "Invalid x value.\n");
        return 1;
    }

    double epsilon = strtod(argv[2], &endptr);
    if (*endptr != '\0' || epsilon == HUGE_VAL || epsilon == -HUGE_VAL || epsilon <= 0)
    {
        fprintf(stderr, "Invalid epsilon value.\n");
        return 1;
    }

    double result_a, result_b, result_c, result_d;

    if (calculate_sum_a(x, epsilon, &result_a) != 0)
    {
        fprintf(stderr, "Error calculating sum a.\n");
        return 1;
    }
    printf("a: %lf\n", result_a);
    if (calculate_sum_b(x, epsilon, &result_b) != 0)
    {
        fprintf(stderr, "Error calculating sum b.\n");
        return 1;
    }
    printf("b: %lf\n", result_b);
    if (calculate_sum_c(x, epsilon, &result_c) != 0)
    {
        fprintf(stderr, "Error calculating sum c.\n");
        return 1;
    }
    printf("c: %lf\n", result_c);
    if (calculate_sum_d(x, epsilon, &result_d) != 0)
    {
        fprintf(stderr, "Error calculating sum d.\n");
        return 1;
    }
    printf("d: %lf\n", result_d);

    return 0;
}