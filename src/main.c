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

void visualizeAlgorithm(SortingFunction sf, int *array, int size, struct winsize w)
{
    randomizeArray(array, size, w.ws_row - 2);
    system("clear");
    printf("%s Visualization...\nPress enter to start...", sf.name); getchar();
    sf.function(array, size, 0);
    printf("%s Visualization Completed.\nPress enter to continue...", sf.name); getchar();
}

int main()
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
        visualizeAlgorithm(sortingFunctions[i], arr, arraysize, w);
    }

    return 0;
}