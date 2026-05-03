#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "bubbleSort.h"
#include "swap.h"
#include "visualize.h"

void bubbleSort(int *array, int size, double intervalInSeconds)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                swap(&array[j], &array[j + 1]);
                VISUALIZE(array, size, intervalInSeconds);
            }
        }
        
    }
}