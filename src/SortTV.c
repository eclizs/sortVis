#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<getopt.h>
#include "platform.h"
#include "visualize.h"
#include "algorithms.h"

#define ALGORITHM(fn, name) extern void fn(int *arr, int size, double intervalInSeconds);
ALGORITHMS
#undef ALGORITHM


#define DEFAULT_INTERVAL 0.01
#define STATUS_LINES 5

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

void visualizeAlgorithm(SortingFunction sf, int *array, int size, double intervalInSeconds)
{
    int *tempArray = malloc(size * sizeof(int));
    if(tempArray == NULL) {
        fprintf(stderr, "Array size too big!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(tempArray, array, size * sizeof(int));
    CLEAR_SCREEN();
    VISUALIZE(tempArray, size, 0.001);
    printf("%s Sort Visualization...\nPress enter to start...", sf.name); getchar();
    sf.function(tempArray, size, intervalInSeconds);
    printf("%s Sort Visualization Completed.\nPress enter to continue...", sf.name); getchar();
    free(tempArray);
}

void benchmarkAlgorithm(SortingFunction sf, int *array, int size)
{
    struct timespec start, end;
    int *tempArray = malloc(size * sizeof(int));
    if(tempArray == NULL) {
        fprintf(stderr, "Array size too big!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(tempArray, array, size * sizeof(int));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    sf.function(tempArray, size, 0); // No visualization for benchmark
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    double timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%s Sort Benchmark: %.6f seconds\n", sf.name, timeTaken);
    free(tempArray);
}


void printHelp(char *programName)
{
    printf("Usage: %s [OPTIONS]\n", programName);
    printf("Options:\n");
    printf("  -s, --sort [SORT_NAME]   Specify sorting algorithm (bubble, selection, insertion, merge, quick)\n");
    printf("  -b, --benchmark=[VALUE]  Run in benchmark mode (no visualization; [VALUE] is the array size; default is 10000)\n");
    printf("  -w, --width=[WIDTH]      Set visualization width (default and maximum: terminal width)\n");
    printf("  -e, --height=[HEIGHT]    Set visualization height (default and maximum: terminal height)\n");
    printf("  -i, --interval=[SECONDS] Set interval between steps in seconds (default is 0.01; gets ignored if in benchmark mode)\n");
    printf("  -h, --help               Display this help message\n");
}

void parseArguments(int argc, char *argv[], char **sortName, int *benchmark, int *benchmarkSize, int *width, int *height, double *intervalInSeconds)
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
                if(optarg != NULL)
                {
                    const char *val = (optarg[0] == '=') ? optarg + 1 : optarg;
                    *benchmarkSize = atoi(val);
                }
                else
                {
                    *benchmarkSize = 10000;
                }
                break;
            case 'w':
                if(optarg != NULL)
                {
                    const char *val = (optarg[0] == '=') ? optarg + 1 : optarg;
                    *width = atoi(val);
                }
                else
                {
                    *width = atoi(optarg);
                }
                break;
            case 'e':
                if(optarg != NULL)
                {
                    const char *val = (optarg[0] == '=') ? optarg + 1 : optarg;
                    *height = atoi(val);
                }
                else
                {
                    *height = atoi(optarg);
                }
                break;
            case 'i':
                *intervalInSeconds = atof(optarg);
                break;
            case '?':
                fprintf(stderr, "Unknown option. Use --help for usage.\n");
                exit(EXIT_FAILURE);
            case 'h':
            default:
                printHelp(argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[])
{
    int benchmark = 0;
    int benchmarkSize = 10000;
    char *sortName = NULL;
    int width = -1, height = -1;
    double intervalInSeconds = DEFAULT_INTERVAL; // Default interval
    
    parseArguments(argc, argv, &sortName, &benchmark, &benchmarkSize, &width, &height, &intervalInSeconds);
    CLEAR_SCREEN();

    if(sortName != NULL) sortName[0] = toupper(sortName[0]); // Capitalize first letter for display

    WindowSize w = getConsoleInfo();

    if((width < 0 || width > w.ws_col - 2 ||
        height < 0 || height > w.ws_row - STATUS_LINES)
        && !benchmark) // Only get terminal size if not in benchmark mode
    {
        if(width < 0 || width > w.ws_col / 2 ) width = w.ws_col / 2;
        if(height < 0 || height > w.ws_row - STATUS_LINES) height = w.ws_row - STATUS_LINES;
    }

    if(benchmark)
    {
        if(benchmarkSize > 0)
        {
            width = benchmarkSize;
            height = benchmarkSize;
        }
        else
        {
            fprintf(stderr, "Invalid benchmark size specified. Using default of 10000.\n");
            width = 10000;
            height = 10000;
        }
    }

    int arraySize = width;
    int maxValue = height;
    srand((unsigned)time(NULL));

    int *arr = malloc(arraySize * sizeof(int));
    if(arr == NULL) {
        fprintf(stderr, "Array size too big!\n");
        exit(EXIT_FAILURE);
    }

    #define ALGORITHM(fn, name) {fn, name},
    SortingFunction sortingFunctions[] = {
        ALGORITHMS
    };

    #undef ALGORITHM
    int numSortingFunctions = sizeof(sortingFunctions) / sizeof(sortingFunctions[0]);

    randomizeArray(arr, arraySize, maxValue);

    if(benchmark)
    {
        if(arraySize <= w.ws_col / 2)
        {
            VISUALIZE(arr, arraySize, 0.001);
            printf("Initial Array...\nPress enter to start..."); getchar();
        }
        
        printf("Benchmarking all sorting algorithms for an array of size %d:\n", arraySize);
        for (int i = 0; i < numSortingFunctions; i++)
        {
            benchmarkAlgorithm(sortingFunctions[i], arr, arraySize);
        }
        return 0;
    }

    if(sortName != NULL)
    {
        for (int i = 0; i < numSortingFunctions; i++)
        {
            if (strcmp(sortName, sortingFunctions[i].name) == 0)
            {
                if(arraySize <= w.ws_col / 2)
                {
                    VISUALIZE(arr, arraySize, 0.001);
                    printf("Initial Array...\nPress enter to start..."); getchar();
                }

                if(benchmark) benchmarkAlgorithm(sortingFunctions[i], arr, arraySize);
                else visualizeAlgorithm(sortingFunctions[i], arr, arraySize, intervalInSeconds);
                return 0;
            }
        }
        fprintf(stderr, "Unknown sorting algorithm: %s\n", sortName);
        exit(EXIT_FAILURE);
    }

    
    for (int i = 0; i < numSortingFunctions; i++)
    {
        visualizeAlgorithm(sortingFunctions[i], arr, arraySize, intervalInSeconds);
    }

    free(arr);
    return 0;
}