#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32

#include<stdio.h>
#include<windows.h>
#include<io.h>
#define CLEAR_SCREEN() system("cls")
#define WRITE(fd, buf, count) _write(fd, buf, count)
#define SLEEP_S(seconds) Sleep((seconds) * 1000)

#else
#include<unistd.h>
#include<sys/ioctl.h>
#define CLEAR_SCREEN() system("clear")
#define WRITE(fd, buf, count) write(fd, buf, count)
#define SLEEP_S(seconds) usleep((seconds) * 1000000)
#endif
typedef struct {
    int ws_col;
    int ws_row;
} WindowSize;

WindowSize getConsoleInfo();

#endif // PLATFORM_H