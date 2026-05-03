#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef WIN_32
#include<windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#include<unistd.h>
#define CLEAR_SCREEN() system("clear")
#endif

#endif // PLATFORM_H