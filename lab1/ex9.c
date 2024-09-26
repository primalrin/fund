#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define FIXED_ARRAY_SIZE 100

// Function to swap two integers
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Function to find and swap min and max elements in an array
int min_max_swap(int arr[], int size)
{
    if (size <= 0 || !arr)
    {
        return 1;
    }

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int min_idx = -1;
    int max_idx = -1;

    for (int i = 0; i < size; i++)
    {
        if (arr[i] < min_val)
        {
            min_val = arr[i];
            min_idx = i;
        }
        if (arr[i] > max_val)
        {
            max_val = arr[i];
            max_idx = i;
        }
    }

    if (min_idx != -1 && max_idx != -1 && min_idx != max_idx)
    {
        swap(&arr[min_idx], &arr[max_idx]);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    int a, b;
    if (sscanf(argv[1], "%d", &a) != 1 || sscanf(argv[2], "%d", &b) != 1)
    {
        fprintf(stderr, "Invalid input: a and b must be integers.\n");
        return 1;
    }

    if (a >= b)
    {
        fprintf(stderr, "Invalid input: a must be less than b.\n");
        return 1;
    }

    int fixed_array[FIXED_ARRAY_SIZE];
    srand(time(NULL));

    for (int i = 0; i < FIXED_ARRAY_SIZE; i++)
    {
        fixed_array[i] = a + rand() % (b - a + 1);
    }

    if (min_max_swap(fixed_array, FIXED_ARRAY_SIZE) != 0)
    {
        fprintf(stderr, "Error in min_max_swap function.\n");
        return 1;
    }

    // Dynamic arrays part
    int size_a = 10 + rand() % (10000 - 10 + 1);
    int size_b = 10 + rand() % (10000 - 10 + 1);

    int *a_arr = (int *)malloc(size_a * sizeof(int));
    int *b_arr = (int *)malloc(size_b * sizeof(int));
    int *c_arr = (int *)malloc(size_a * sizeof(int));

    if (!a_arr || !b_arr || !c_arr)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        free(a_arr);
        free(b_arr);
        free(c_arr);
        return 1;
    }

    for (int i = 0; i < size_a; i++)
    {
        a_arr[i] = -1000 + rand() % (1000 - (-1000) + 1);
    }
    for (int i = 0; i < size_b; i++)
    {
        b_arr[i] = -1000 + rand() % (1000 - (-1000) + 1);
    }

    for (int i = 0; i < size_a; i++)
    {
        int closest_val = b_arr[0];
        int min_diff = abs(a_arr[i] - b_arr[0]);
        for (int j = 1; j < size_b; j++)
        {
            int diff = abs(a_arr[i] - b_arr[j]);
            if (diff < min_diff)
            {
                min_diff = diff;
                closest_val = b_arr[j];
            }
        }
        c_arr[i] = a_arr[i] + closest_val;
    }

    free(a_arr);
    free(b_arr);
    free(c_arr);

    return 0;
}