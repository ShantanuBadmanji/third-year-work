#include <stdio.h>
#include <sys/types.h>
// #include <syscall.h>
#include <unistd.h>
// #include <string.h>
#include <stdlib.h>

void displayArray(int *arr, int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
void displayStringArray(char **arr, int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        printf("%s ", arr[i]);
    }
    printf("\n");
}

void insertionSort(int *arr, int arrSize)
{
    for (int i = 1; i < arrSize; i++)
    {
        int j = i;
        while (j > 0 && arr[j - 1] > arr[j])
        {
            int x = arr[j - 1];
            arr[j - 1] = arr[j];
            arr[j] = x;
            j--;
        }
    }
}

void createArgVector(char **strArr, int *arr, int arrSize, int extraChar, int key)
{
    for (int i = 0; i < arrSize + 3; i++)
    {
        char *s = (char *)calloc(10, sizeof(char));
        if (i < extraChar)
        {
            switch (i)
            {
            case 0:
                s = "binarySearch";
                break;
            case 1:
                sprintf(s, "%d", key);
                break;
            case 2:
                sprintf(s, "%d", arrSize);
                break;

            default:
                break;
            }
        }
        else
            sprintf(s, "%d", arr[i - extraChar]);
        strArr[i] = s;
    }
}

int main(int argc, char const *argv[])
{
    // int arr[] = {51, 32, 68, 35, 254, 6, 25, -1, 56};
    // int arrSize = sizeof(arr) / sizeof(int);

    int arrSize;
    printf("Enter the size of array:");
    scanf("%d", &arrSize);
    int *arr = (int *)calloc(arrSize, sizeof(int));

    printf("Enter the elements of array:");
    for (int i = 0; i < arrSize; i++)
        scanf("%d", &arr[i]);
    printf("The Sorted Array: ");
    insertionSort(arr, arrSize);
    displayArray(arr, arrSize);

    int extraChar = 3;
    // int key = 51;
    int key;

    printf("Enter the number to be search: ");
    scanf("%d", &key);

    char **strArr = (char **)calloc(arrSize, sizeof(char *));
    createArgVector(strArr, arr, arrSize, extraChar, key);
    // displayStringArray(strArr, arrSize);

    pid_t fork_id = fork();

    if (fork_id == 0)
        execv("/home/dysspo/SHANTANU WORKSPACE/os/assign_2/binarySearch", strArr);

    else
    {
        int childStatus;
        wait(&childStatus);
        printf("childStatus: %d\n", childStatus);
    }
    return 0;
}
