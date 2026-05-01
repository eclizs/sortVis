#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/ioctl.h>

#define GNU_SOURCE

#define VISUALIZE(arr,size, intervalInSeconds) \
    do { \
        system("clear"); \
        visualizeArray(arr, size); \
        printf("\n"); \
        usleep(intervalInSeconds * 1000000); \
    } while(0)

typedef struct SortingFunction {
    void (*function)(int *, int, int);
    const char *name;
} SortingFunction;

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

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void mergeSort_rec(int *array, int left, int right, int size, int intervalInSeconds)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        
        VISUALIZE(array, size, intervalInSeconds);
        mergeSort_rec(array, left, mid, size, intervalInSeconds);
        mergeSort_rec(array, mid + 1, right, size, intervalInSeconds);
        
        int i = left, j = mid + 1, k = 0;
        int *temp = (int *)malloc((right - left + 1) * sizeof(int));
        
        while (i <= mid && j <= right)
        {
            if (array[i] < array[j])
            {
                temp[k++] = array[i++];
            }
            else
            {
                temp[k++] = array[j++];
            }
            VISUALIZE(array, size, intervalInSeconds);
        }
        
        while (i <= mid)
        {
            temp[k++] = array[i++];
        }
        
        while (j <= right)
        {
            temp[k++] = array[j++];
        }
        
        for (i = left, k = 0; i <= right; i++, k++)
        {
            array[i] = temp[k];
            VISUALIZE(array, size, intervalInSeconds);
        }
        
        free(temp);
    }
}

void mergeSort(int *array, int size, int intervalInSeconds)
{
    mergeSort_rec(array, 0, size - 1, size, intervalInSeconds);
}

int partition(int *array, int low, int high, int size, int intervalInSeconds)
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
    return i + 1;
}

void quickSort_rec(int *array, int low, int high, int size, int intervalInSeconds)
{
    if (low < high)
    {
        int pivot = partition(array, low, high, size, intervalInSeconds);
        quickSort_rec(array, low, pivot - 1, size, intervalInSeconds);
        quickSort_rec(array, pivot + 1, high, size, intervalInSeconds);
    }
}

void quickSort(int *array, int size, int intervalInSeconds)
{
    quickSort_rec(array, 0, size - 1, size, intervalInSeconds);
}

void bubbleSort(int *array, int size, int intervalInSeconds)
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
        }
        swap(&array[i], &array[minIndex]);
        VISUALIZE(array, size, intervalInSeconds);
    }
}

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

void printMsg(char *sortingAlgorithm, int *array, int size)
{
    printf("%s Visualization:\n", sortingAlgorithm); getchar();
    VISUALIZE(array, size, 0.01);
    getchar();
}

void randomizeArray(int *array, int arraysize, int max)
{
    for (int i = 0; i < arraysize; i++)
    {
        array[i] = rand() % max + 1;
    }
}

void benchmarkSortingAlgorithm(SortingFunction sf, int *array, int size, struct winsize w)
{
    randomizeArray(array, size, w.ws_row - 1);
    printf("Benchmarking %s...\nPress enter to start...", sf.name); getchar();
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    sf.function(array, size, 0);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Time taken by %s: %f seconds\nPress enter to continue...", sf.name, time_taken); getchar();
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
        benchmarkSortingAlgorithm(sortingFunctions[i], arr, arraysize, w);
    }

    return 0;
}