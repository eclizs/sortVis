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

#define GNU_SOURCE
#define DEFAULT_INTERVAL 0.01

typedef struct SortingFunction {
    void (*function)(int *, int, double);
    const char *name;
} SortingFunction;

static struct option long_options[] = {
    {"sort", required_argument, 0, 's'},
    {"benchmark", optional_argument, 0, 'b'},
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

void benchmarkalgorithm(SortingFunction sf, int *array, int size, int max)
{
    struct timespec start, end;
    randomizeArray(array, size, max);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    sf.function(array, size, 0); // No visualization for benchmark
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    double timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%s Sort Benchmark: %.6f seconds\n", sf.name, timeTaken);
}

void parseArguments(int argc, char *argv[], char **sortName, int *benchmark, int *width, int *height, double *intervalInSeconds)
{
    int opt;
    while((opt = getopt_long(argc, argv, "s:b::w:e:i:h", long_options, NULL)) != -1)
    {
        switch(opt)
        {
            case 's':
                *sortName = optarg;
                break;
            case 'b':
                *benchmark = 1;
                *width = (optarg != NULL) ? atoi(optarg) : 10000;
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
                printf("  -b, --benchmark=[VALUE]  Run in benchmark mode (no visualization; <VALUE> is the array size; default is 10000)\n");
                printf("  -w, --width=[WIDTH]      Set visualization width (default: terminal width)\n");
                printf("  -e, --height=[HEIGHT]    Set visualization height (default: terminal height)\n");
                printf("  -i, --interval=[SECONDS] Set interval between steps in seconds (default is 0.01; gets ignored if in benchmark mode)\n");
                printf("  -h, --help               Display this help message\n");
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
        printf("  -b, --benchmark=[VALUE]  Run in benchmark mode (no visualization; <VALUE> is the array size; default is 10000)\n");
        printf("  -w, --width=[WIDTH]      Set visualization width (default: terminal width)\n");
        printf("  -e, --height=[HEIGHT]    Set visualization height (default: terminal height)\n");
        printf("  -i, --interval=[SECONDS] Set interval between steps in seconds (default is 0.01; gets ignored if in benchmark mode)\n");
        printf("  -h, --help               Display this help message\n");
        return 1;
    }

    int benchmark = 0;
    char *sortName = NULL;
    int width = -1, height = -1;
    double intervalInSeconds = DEFAULT_INTERVAL; // Default interval

    parseArguments(argc, argv, &sortName, &benchmark, &width, &height, &intervalInSeconds);

    if((width < 0 || height < 0) && !benchmark) // Only get terminal size if not in benchmark mode
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if(width < 0) width = w.ws_col / 2 - 1;
        if(height < 0) height = w.ws_row - 2;
    }

    int arraysize = width;
    int maxValue = height;
    srand((unsigned)time(NULL));

    int arr[arraysize];

    SortingFunction sortingFunctions[] = {
        {bubbleSort,    "bubble"},
        {selectionSort, "selection"},
        {insertionSort, "insertion"},
        {mergeSort,     "merge"},
        {quickSort,     "quick"},
    };
    int numSortingFunctions = sizeof(sortingFunctions) / sizeof(sortingFunctions[0]);

    if(sortName != NULL)
    {
        for (int i = 0; i < numSortingFunctions; i++)
        {
            if (strcmp(sortName, sortingFunctions[i].name) == 0)
            {
                if(benchmark) benchmarkalgorithm(sortingFunctions[i], arr, arraysize, maxValue);
                else visualizeAlgorithm(sortingFunctions[i], arr, arraysize, maxValue, intervalInSeconds);
                return 0;
            }
        }
        fprintf(stderr, "Unknown sorting algorithm: %s\n", sortName);
        exit(EXIT_FAILURE);
    }

    if(benchmark)
    {
        printf("Benchmarking all sorting algorithms for an array of size %d:\n", arraysize);
        for (int i = 0; i < numSortingFunctions; i++)
        {
            benchmarkalgorithm(sortingFunctions[i], arr, arraysize, maxValue);
        }
        return 0;
    }
    
    for (int i = 0; i < numSortingFunctions; i++)
    {
        visualizeAlgorithm(sortingFunctions[i], arr, arraysize, maxValue, intervalInSeconds);
    }

    return 0;
}