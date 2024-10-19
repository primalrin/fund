#include <stdio.h>
#include <math.h>

// Функция для нахождения корня уравнения методом дихотомии
double dichotomy(double a, double b, double eps, double (*f)(double))
{
    if (f(a) * f(b) > 0)
    {
        printf("Error: f(a) and f(b) must have different signs.\n");
        return -1; // Или другое значение, указывающее на ошибку
    }

    double c;
    while (fabs(b - a) > eps)
    {
        c = (a + b) / 2;
        if (f(a) * f(c) < 0)
        {
            b = c;
        }
        else
        {
            a = c;
        }
    }
    return c;
}

// Пример функции для уравнения x^2 - 4 = 0
double f1(double x)
{
    return x * x - 4;
}

// Пример функции для уравнения sin(x) = 0
double f2(double x)
{
    return sin(x);
}

int main()
{
    double a1 = 0, b1 = 3, eps1 = 0.001;
    double root1 = dichotomy(a1, b1, eps1, f1);
    printf("Root of x^2 - 4 = 0 in [%.2f, %.2f] with eps = %.3f: %.4f\n", a1, b1, eps1, root1);

    double a2 = 3, b2 = 4, eps2 = 0.0001;
    double root2 = dichotomy(a2, b2, eps2, f2);
    printf("Root of sin(x) = 0 in [%.2f, %.2f] with eps = %.4f: %.4f\n", a2, b2, eps2, root2);

    return 0;
}