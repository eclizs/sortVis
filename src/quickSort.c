#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "quickSort.h"
#include "swap.h"
#include "visualize.h"

static int partition(int *array, int low, int high, int size, double intervalInSeconds)
{
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            swap(&array[i], &array[j]);
            VISUALIZE(array, size, intervalInSeconds);
        }
    }

    swap(&array[i + 1], &array[high]);
    VISUALIZE(array, size, intervalInSeconds);
    return i + 1;
}

static void quickSort_rec(int *array, int low, int high, int size, double intervalInSeconds)
{
    if (low < high)
    {
        int pivot = partition(array, low, high, size, intervalInSeconds);
        quickSort_rec(array, low, pivot - 1, size, intervalInSeconds);
        quickSort_rec(array, pivot + 1, high, size, intervalInSeconds);
    }
}

void quickSort(int *array, int size, double intervalInSeconds)
{
    quickSort_rec(array, 0, size - 1, size, intervalInSeconds);
}
