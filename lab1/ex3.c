#include <stdio.h>
#include <limits.h>
#include <float.h>

double machine_infinity()
{
    double infinity = 1.0;

    while (infinity * 2.0 != infinity)
    {
        infinity *= 2.0;
    }

    return infinity;
}

int main()
{
    printf("Машинная бесконечность (double): %f\n", machine_infinity());

    return 0;
}