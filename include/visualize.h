#ifndef VISUALIZE_H
#define VISUALIZE_H

#define VISUALIZE(arr,size, intervalInSeconds) \
    do { \
        if(intervalInSeconds > 0)\
        { \
            write(STDOUT_FILENO, "\033[H", 3); /* Move cursor to the top-left corner */ \
            visualizeArray(arr, size); \
            usleep(intervalInSeconds * 1000000); \
        } \
    } while(0)

int findMax(int *array, int size);
void visualizeArray(int *array, int size);

#endif // VISUALIZE_H