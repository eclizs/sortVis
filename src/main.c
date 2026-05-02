#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/ioctl.h>
#include "visualize.h"
#include "bubbleSort.h"
#include "selectionSort.h"
#include "insertionSort.h"
#include "mergeSort.h"
#include "quickSort.h"

typedef struct SortingFunction {
    void (*function)(int *, int, int);
    const char *name;
} SortingFunction;

void randomizeArray(int *array, int arraysize, int max)
{
    for (int i = 0; i < arraysize; i++)
    {
        array[i] = rand() % max + 1;
    }
}

void visualizeAlgorithm(SortingFunction sf, int *array, int size, int max, int intervalInSeconds)
{
    randomizeArray(array, size, max);
    system("clear");
    printf("%s Visualization...\nPress enter to start...", sf.name); getchar();
    sf.function(array, size, intervalInSeconds);
    printf("%s Visualization Completed.\nPress enter to continue...", sf.name); getchar();
}

int main(int argc, char *argv[])
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int arraysize = w.ws_col / 2 - 1;
    printf("Array size: %d\n", arraysize); getchar();
    srand((unsigned)time(NULL));
    int arr[arraysize];

    SortingFunction sortingFunctions[] = {
        {bubbleSort, "Bubble Sort"},
        {selectionSort, "Selection Sort"},
        {insertionSort, "Insertion Sort"},
        {mergeSort, "Merge Sort"},
        {quickSort, "Quick Sort"}
    };
    
    int numSortingFunctions = sizeof(sortingFunctions) / sizeof(sortingFunctions[0]);
    for (int i = 0; i < numSortingFunctions; i++)
    {
        visualizeAlgorithm(sortingFunctions[i], arr, arraysize, w.ws_row - 2, 0.5);
    }

    return 0;
}