#define _GNU_SOURCE
#ifdef _WIN32
#include<stdio.h>
#include<io.h>
#else
#include <sys/ioctl.h>
#endif
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