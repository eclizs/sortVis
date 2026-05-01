#ifndef VISUALIZE_H
#define VISUALIZE_H

#define VISUALIZE(arr,size, intervalInSeconds) \
    do { \
            system("clear"); \
            visualizeArray(arr, size); \
            printf("\n"); \
            usleep(intervalInSeconds * 1000000); \
    } while(0)

int findMax(int *array, int size);
void visualizeArray(int *array, int size);

#endif // VISUALIZE_H