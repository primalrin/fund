#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define MAX_ITERATIONS 1000000
#define MAX_ITERATIONS_FAC 47

double calculate_e_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    double e = 0.0;
    double prev_e = -1.0;
    int n = 1;

    while (fabs(e - prev_e) > epsilon && n < INT_MAX)
    {
        prev_e = e;
        e = pow(1.0 + (1.0 / n), n);
        if (n == INT_MAX)
        {
            return -1.0;
        }
        n++;
    }

    return e;
}

double calculate_e_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
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

double calculate_pi_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    double pi_approx = 0.0;
    int n = 1;

    while (1)
    {
        double prev_pi_approx = pi_approx;

        double numerator = pow(pow(2.0, n) * tgamma(n + 1), 4);
        double denominator = n * pow(tgamma(2 * n + 1), 2);

        pi_approx = numerator / denominator;

        if (fabs(pi_approx - prev_pi_approx) < epsilon || n > MAX_ITERATIONS_FAC)
        {
            return pi_approx;
        }
        n++;
    }
}

double calculate_pi_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
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

double calculate_ln2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
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

double calculate_ln2_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
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

double calculate_sqrt2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
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

double calculate_sqrt2_formula(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    double result = 1.0;
    int k = 2;
    double term;

    while ((fabs(result - term) > epsilon) && k < INT_MAX)
    {
        term = result;
        result *= pow(2.0, pow(2.0, -k));
        k++;
    }

    return result;
}

double bisection_method(double (*f)(double), double a, double b, double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    if (f(a) * f(b) >= 0)
    {
        return NAN;
    }

    double c;
    int iterations = 0;

    while (fabs(b - a) > epsilon && iterations < INT_MAX)
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

    if (iterations >= INT_MAX)
    {
        return NAN;
    }

    return c;
}

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

double equation_sqrt2(double x)
{
    return x * x - 2.0;
}

double calculate_gamma_comb(int m)
{
    double sum = 0.0;
    for (int k = 1; k <= m; k++)
    {
        double binomial_coefficient = tgamma(m + 1) / (tgamma(k + 1) * tgamma(m - k + 1));
        double term = binomial_coefficient * pow(-1, k) * lgamma(k + 1) / k;
        if (isnan(term) || isinf(term))
        {
            return -1;
        }
        sum += term;
    }
    return sum;
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
    printf("sqrt(2) (formula): %.10f\n", calculate_sqrt2_formula(epsilon));
    double sqrt2_bisection = bisection_method(equation_sqrt2, 1.0, 2.0, epsilon);
    if (isnan(sqrt2_bisection))
    {
        printf("sqrt(2) (bisection): Error: Solution not found or max iterations reached\n");
    }
    else
    {
        printf("sqrt(2) (bisection): %.10f\n", sqrt2_bisection);
    }

    printf("Gamma (combinatorial): %.10f\n", calculate_gamma_comb(MAX_ITERATIONS_FAC));
    printf("Gamma (sqrt sum): %.10f\n", calculate_gamma_sqrt_sum(MAX_ITERATIONS));
    printf("Gamma (prime product): %.10f\n", calculate_gamma_prime_product(MAX_ITERATIONS));

    return 0;
}

// gcc ex2.c -o ex2

// ./ex2.exe 0.00001