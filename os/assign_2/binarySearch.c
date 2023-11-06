#include <stdio.h>
#include <stdlib.h>

void displayArray(int *arr, int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        printf("%d ", arr[i]);
        printf("\n");
    }
}

void displayStringArray(char **arr, int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        printf("%s ", arr[i]);
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    // displayStringArray(argv, argc);

    if (argc < 3)
    {
        printf("Pass the sorted Array as argument vector.\n");
        exit(0);
    }

    int key = atoi(argv[1]);
    int arrSize = atoi(argv[2]);
    if (argc != 3 + arrSize)
    {
        printf("Pass array elements equal to given array size.\n");
        exit(0);
    }

    int *sortedArr = (int *)calloc(arrSize, sizeof(int));
    for (int i = 0; i < arrSize; i++)
        sortedArr[i] = atoi(argv[i + 3]);

    // displayArray(sortedArr, arrSize);

    int keyIndex = -1;
    int startIndex = 0, endIndex = arrSize - 1;
    while (startIndex <= endIndex)
    {
        int midIndex = startIndex + (endIndex - startIndex) / 2;
        // printf("the  midIndex is: %d", midIndex);

        if (sortedArr[midIndex] < key)
            startIndex = midIndex + 1;
        else if (key < sortedArr[midIndex])
            endIndex = midIndex - 1;
        else
        {
            keyIndex = midIndex;
            break;
        }
    }

    if (keyIndex >= 0)
        printf("The index of %d is %d.\n", key, keyIndex);
    else
        printf("%d is not in the Array.\n", key);

    return 0;
}
