#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define M_PI 3.14159265358979323846

// Function to calculate e using limit
double calculate_e_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double e = 0.0;
    double prev_e = -1.0; // Initialize with a different value
    int n = 1;

    while (fabs(e - prev_e) > epsilon && n < INT_MAX)
    {
        prev_e = e;
        e = pow(1.0 + (1.0 / n), n);
        n++;
    }

    return e;
}

// Function to calculate e using series
double calculate_e_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double e = 0.0;
    double term = 1.0;
    int n = 0;

    while (fabs(term) > epsilon && n < INT_MAX)
    {
        e += term;
        n++;
        term /= n;
    }

    return e;
}

// Function to calculate pi using limit
double calculate_pi_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double pi = 0.0;
    double prev_pi = -1.0;
    int k = 0;

    while (fabs(pi - prev_pi) > epsilon && k < MAX_ITERATIONS)
    {
        prev_pi = pi;
        double term = (1.0 / pow(16, k)) *
                      ((4.0 / (8 * k + 1)) -
                       (2.0 / (8 * k + 4)) -
                       (1.0 / (8 * k + 5)) -
                       (1.0 / (8 * k + 6)));
        pi += term;
        k++;
    }

    return pi;
}

// Function to calculate pi using series
double calculate_pi_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double pi = 0.0;
    double term = 0.0;
    int n = 1;

    while ((fabs(term) > epsilon || term == 0.0) && n < INT_MAX)
    {
        term = 4.0 * pow(-1, n - 1) / (2 * n - 1);
        pi += term;
        n++;
    }

    return pi;
}

// Function to calculate ln(2) using limit
double calculate_ln2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double ln2 = 0.0;
    double prev_ln2 = -1.0;
    int n = 1;

    while (fabs(ln2 - prev_ln2) > epsilon && n < INT_MAX)
    {
        prev_ln2 = ln2;
        ln2 = n * (pow(2.0, 1.0 / n) - 1.0);
        n++;
    }

    return ln2;
}

// Function to calculate ln(2) using series
double calculate_ln2_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double ln2 = 0.0;
    double term = 0.0;
    int n = 1;

    while ((fabs(term) > epsilon || term == 0.0) && n < INT_MAX)
    {
        term = pow(-1, n - 1) / n;
        ln2 += term;
        n++;
    }

    return ln2;
}

// Function to calculate sqrt(2) using limit
double calculate_sqrt2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double x = -0.5;
    double next_x;

    do
    {
        next_x = x - (x * x / 2) + 1;
        if (fabs(next_x - x) < epsilon)
        {
            break;
        }
        x = next_x;
    } while (1);

    return next_x;
}

// Function to calculate sqrt(2) using Newton's method
double calculate_sqrt2_newton(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    double x = 1.0; // Initial guess

    while (fabs(x * x - 2.0) > epsilon)
    {
        x = x - (x * x - 2.0) / (2.0 * x);
    }

    return x;
}

// Function to solve equations using the bisection method
double bisection_method(double (*f)(double), double a, double b, double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0; // Indicate error for invalid epsilon
    }
    if (f(a) * f(b) >= 0)
    {
        return NAN; // Indicate error: f(a) and f(b) must have opposite signs
    }

    double c;
    int iterations = 0;

    while (fabs(b - a) > epsilon && iterations < MAX_ITERATIONS)
    {
        c = (a + b) / 2.0;
        if (f(c) * f(a) < 0)
        {
            b = c;
        }
        else
        {
            a = c;
        }
        iterations++;
    }

    if (iterations >= MAX_ITERATIONS)
    {
        return NAN; // Indicate error: max iterations reached
    }

    return c;
}

// Functions for equations (to be used with bisection_method)
double equation_ln_x(double x)
{
    return log(x) - 1.0;
}

double equation_cos_x(double x)
{
    return cos(x) + 1.0;
}

double equation_pi(double x)
{
    return sin(x);
}

double equation_e_pow_x(double x)
{
    return exp(x) - 2.0;
}

// Function for equation sqrt(2) (to be used with bisection_method)
double equation_sqrt2(double x)
{
    return x * x - 2.0;
}

double calculate_gamma_comb(int m_max)
{
    double gamma = 0.0;
    for (int k = 1; k <= m_max; k++)
    {
        double term = pow(-1, k) * log(tgamma(k + 1)) / k;
        if (isnan(term) || isinf(term)) // Проверка на NaN или бесконечность
        {
            continue; // Пропускаем невалидные значения
        }
        gamma += term;
    }
    return gamma;
}

double calculate_gamma_sqrt_sum(int k_max)
{
    double gamma = -M_PI * M_PI / 6.0;
    for (int k = 2; k <= k_max; k++)
    {
        double term = (1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k);
        gamma += term;
    }
    return gamma;
}

double calculate_gamma_prime_product(int t_max)
{
    double product = 1.0;
    for (int p = 2; p <= t_max; p++)
    {
        if (is_prime(p))
        {
            product *= pow((p - 1.0) / p, 1.0 / p);
        }
    }
    return exp(-product);
}

int is_prime(int num)
{
    if (num < 2)
        return 0;
    for (int i = 2; i <= sqrt(num); i++)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    double epsilon = atof(argv[1]);

    if (epsilon <= 0.0)
    {
        fprintf(stderr, "Epsilon must be a positive number.\n");
        return 1;
    }

    printf("Calculating constants with epsilon = %f\n", epsilon);

    printf("e (limit): %.10f\n", calculate_e_limit(epsilon));
    printf("e (series): %.10f\n", calculate_e_series(epsilon));
    double e_bisection = bisection_method(equation_ln_x, 2.0, 3.0, epsilon);
    if (isnan(e_bisection))
    {
        printf("e (bisection): Error: Solution not found or max iterations reached\n");
    }
    else
    {
        printf("e (bisection): %.10f\n", e_bisection);
    }

    printf("pi (limit): %.10f\n", calculate_pi_limit(epsilon));
    printf("pi (series): %.10f\n", calculate_pi_series(epsilon));
    double pi_bisection = bisection_method(equation_pi, 3.0, 3.5, epsilon);
    if (isnan(pi_bisection))
    {
        printf("pi (bisection): Error: Solution not found or max iterations reached\n");
    }
    else
    {
        printf("pi (bisection): %.10f\n", pi_bisection);
    }

    printf("ln(2) (limit): %.10f\n", calculate_ln2_limit(epsilon));
    printf("ln(2) (series): %.10f\n", calculate_ln2_series(epsilon));
    double ln2_bisection = bisection_method(equation_e_pow_x, 0.5, 1.0, epsilon);
    if (isnan(ln2_bisection))
    {
        printf("ln(2) (bisection): Error: Solution not found or max iterations reached\n");
    }
    else
    {
        printf("ln(2) (bisection): %.10f\n", ln2_bisection);
    }

    printf("sqrt(2) (limit): %.10f\n", calculate_sqrt2_limit(epsilon));
    printf("sqrt(2) (Newton's): %.10f\n", calculate_sqrt2_newton(epsilon));
    double sqrt2_bisection = bisection_method(equation_sqrt2, 1.0, 2.0, epsilon);
    if (isnan(sqrt2_bisection))
    {
        printf("sqrt(2) (bisection): Error: Solution not found or max iterations reached\n");
    }
    else
    {
        printf("sqrt(2) (bisection): %.10f\n", sqrt2_bisection);
    }

    printf("Gamma (combinatorial): %.10f\n", calculate_gamma_comb(1000));
    printf("Gamma (sqrt sum): %.10f\n", calculate_gamma_sqrt_sum(1000));
    printf("Gamma (prime product): %.10f\n", calculate_gamma_prime_product(1000));

    return 0;
}