#include<stdio.h>
#include "visualize.h"

int findMax(int *array, int size)
{
    int max = array[0];
    for (int i = 1; i < size; i++)
    {
        if (array[i] > max) max = array[i];
    }
    return max;
}

void visualizeArray(int *array, int size)
{
    int max = findMax(array, size);
    for(int i = max; i > 0; i--)
    {
        for (int j = 0; j < size; j++)
        {
            if (array[j] >= i)
            {
                printf("* ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}