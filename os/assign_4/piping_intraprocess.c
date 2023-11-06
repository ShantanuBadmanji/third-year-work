#include <stdio.h>
#include <unistd.h>
#include <string.h>

void handleError(int returnStatus)
{
    if (returnStatus == -1)
    {
        perror(" Unable to create pipe\n ");
        exit(1);
    }
}
int main(int argc, char const *argv[])
{
    int pipefds[2];
    char writeMessage[20];
    char readMessage[20];

    handleError(pipe(pipefds));
    printf("pipe descripters:%d %d\n", pipefds[0], pipefds[1]);
    int forkId = fork();
    //  write messages
    if (forkId == 0)
    {
        printf("child here:\n");
        do
        {
            gets(writeMessage);
            printf(" Writing to pipe - Message is % s\n ", writeMessage);
            handleError(write(pipefds[1], writeMessage, sizeof(writeMessage)));
        } while (strcmp(writeMessage, "over"));
        // handleError(write(pipefds[1], "over", sizeof("over")));
    }
    else
    {
        //  read messages
        printf("parent here:\n");
        do
        {
            handleError(read(pipefds[0], readMessage, sizeof(readMessage)));
            printf(" Reading from pipe - Message is % s\n ", readMessage);
        } while (strcmp(readMessage, "over"));
    }
    return 0;
}
