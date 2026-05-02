#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/ioctl.h>
#include<getopt.h>
#include "visualize.h"
#include "bubbleSort.h"
#include "selectionSort.h"
#include "insertionSort.h"
#include "mergeSort.h"
#include "quickSort.h"

typedef struct SortingFunction {
    void (*function)(int *, int, double);
    const char *name;
} SortingFunction;

static struct option long_options[] = {
    {"sort", required_argument, 0, 's'},
    {"benchmark", no_argument, 0, 'b'},
    {"width", required_argument, 0, 'w'},
    {"height", required_argument, 0, 'e'},
    {"interval", required_argument, 0, 'i'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

void randomizeArray(int *array, int arraysize, int max)
{
    for (int i = 0; i < arraysize; i++)
    {
        array[i] = rand() % max + 1;
    }
}

void visualizeAlgorithm(SortingFunction sf, int *array, int size, int max, double intervalInSeconds)
{
    randomizeArray(array, size, max);
    system("clear");
    printf("%s Visualization...\nPress enter to start...", sf.name); getchar();
    sf.function(array, size, intervalInSeconds);
    printf("%s Visualization Completed.\nPress enter to continue...", sf.name); getchar();
}

void parseArguments(int argc, char *argv[], char **sortName, int *benchmark, int *width, int *height, double *intervalInSeconds)
{
    int opt;
    while((opt = getopt(argc, argv, "s:bw:e:i:h")) != -1)
    {
        switch(opt)
        {
            case 's':
                *sortName = optarg;
                break;
            case 'b':
                *benchmark = 1;
                break;
            case 'w':
                *width = atoi(optarg);
                break;
            case 'e':
                *height = atoi(optarg);
                break;
            case 'i':
                *intervalInSeconds = atof(optarg);
                break;
            case '?':
                fprintf(stderr, "Unknown option. Use --help for usage.\n");
                exit(EXIT_FAILURE);
            case 'h':
            default:
                printf("Usage: %s [OPTIONS]\n", argv[0]);
                printf("Options:\n");
                printf("  -s, --sort [SORT_NAME]   Specify sorting algorithm (bubble, selection, insertion, merge, quick)\n");
                printf("  -b, --benchmark          Run in benchmark mode\n");
                printf("  -w, --width=[WIDTH]     Set visualization width\n");
                printf("  -e, --height=[HEIGHT]   Set visualization height\n");
                printf("  -i, --interval=[SECONDS] Set interval between steps in seconds\n");
                printf("  -h, --help              Display this help message\n");
                exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Usage: %s [OPTIONS]\n", argv[0]);
        printf("Options:\n");
        printf("  -s, --sort [SORT_NAME]   Specify sorting algorithm (bubble, selection, insertion, merge, quick)\n");
        printf("  -b, --benchmark          Run in benchmark mode\n");
        printf("  -w, --width=[WIDTH]     Set visualization width\n");
        printf("  -e, --height=[HEIGHT]   Set visualization height\n");
        printf("  -i, --interval=[SECONDS] Set interval between steps in seconds\n");
        printf("  -h, --help              Display this help message\n");
        return 0;
    }

    int benchmark = 0;
    char *sortName = NULL;
    int width = -1, height = -1;
    double intervalInSeconds = 0.01; // Default interval

    parseArguments(argc, argv, &sortName, &benchmark, &width, &height, &intervalInSeconds);

    if(width < 0 || height < 0)
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if(width < 0) width = w.ws_col / 2 - 1;
        if(height < 0) height = w.ws_row - 2;
    }
    int arraysize = width;
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
        visualizeAlgorithm(sortingFunctions[i], arr, arraysize, height, intervalInSeconds);
    }

    return 0;
}