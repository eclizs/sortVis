#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "selectionSort.h"
#include "swap.h"
#include "visualize.h"

void selectionSort(int *array, int size, int intervalInSeconds)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < size; j++)
        {
            if (array[j] < array[minIndex])
            {
                minIndex = j;
            }
            VISUALIZE(array, size, intervalInSeconds);
        }
        swap(&array[i], &array[minIndex]);
        VISUALIZE(array, size, intervalInSeconds);
    }
}
