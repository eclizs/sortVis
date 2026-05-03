#define _GNU_SOURCE
#include "platform.h"

WindowSize getConsoleInfo()
{
    WindowSize size;
    #ifdef _WIN32
    if(_isatty(_fileno(stdout)))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
        size.ws_col = consoleInfo.dwSize.X;
        size.ws_row = consoleInfo.dwSize.Y;
        return size;
    }
    else return (WindowSize){80, 24}; // Default size for non-terminal output
    #else
    if(isatty(STDOUT_FILENO))
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        size.ws_col = w.ws_col;
        size.ws_row = w.ws_row;
        return size;
    }
    else return (WindowSize){80, 24}; // Default size for non-terminal output
    #endif
}

double measureElapsedTime(void (*sortingFunction)(int *, int, double), int *array, int size)
{
    #ifdef _WIN32
        struct _FILETIME begin, creation_time, exit_time, kernel_time, end;
        GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &begin);
        sortingFunction(array, size, 0); // No visualization for benchmark
        GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &end);
        ULARGE_INTEGER start;
        start.LowPart = begin.dwLowDateTime;
        start.HighPart = begin.dwHighDateTime;

        ULARGE_INTEGER finish;
        finish.LowPart = end.dwLowDateTime; 
        finish.HighPart = end.dwHighDateTime;

        double elapsed_time = (finish.QuadPart - start.QuadPart) * 1e-7; // Convert to seconds
        return elapsed_time;
    #else
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        sortingFunction(array, size, 0); // No visualization for benchmark
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        return elapsed_time;
    #endif
}