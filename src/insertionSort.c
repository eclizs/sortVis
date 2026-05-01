#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "insertionSort.h"
#include "visualize.h"

void insertionSort(int *array, int size, int intervalInSeconds)
{
    for (int i = 1; i < size; i++)
    {
        int key = array[i];
        int j = i - 1;
        
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
            VISUALIZE(array, size, intervalInSeconds);
        }
        array[j + 1] = key;
        VISUALIZE(array, size, intervalInSeconds);
    }
}