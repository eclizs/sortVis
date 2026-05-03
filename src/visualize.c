#include<stdio.h>
#include<stdlib.h>
#include "platform.h"
#include "visualize.h"

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
    int bufsize = max * (size * 2 + 1) + 1; // Each element takes 2 chars (" #") and a newline, plus null terminator
    
    char *buf = malloc(bufsize);
    if (buf == NULL) {
        fprintf(stderr, "Array size too big!\n");
        return;
    }
    char *ptr = buf;

    for(int i = max; i > 0; i--)
    {
        for (int j = 0; j < size; j++)
        {
           *ptr++ = (array[j] >= i) ? '*' : ' ';
           *ptr++ = ' ';
        }
        *ptr++ = '\n';
    }
    *ptr = '\0'; // Null-terminate the string
    
    fwrite(buf, 1, ptr-buf, stdout);
    free(buf);
}