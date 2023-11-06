#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define WRITE 1
#define READ 0

void handleError(int returnStatus) {
    if (returnStatus == -1) {
        perror(" Unable to create pipe\n ");
        exit(1);
    }
}
void readMessageFromUser(char writeBuffer[], int maxSIze) {
    for (int i = 0; i < maxSIze; i++) {
        char ch = getchar();
        if (ch == '\n') {
            writeBuffer[i] = '\0';
            return;
        }
        writeBuffer[i] = ch;
    }
    writeBuffer[maxSIze - 1] = '\0';
    printf("Exceeded the max message size !!!\n");
}

char *getCustomMessage(char readBuffer[]) {
    return strlen(readBuffer) == 0 ? "[ Emtpy Message ]" : readBuffer;
}

int main(int argc, char const *argv[]) {
    int ChildParentPD[2];
    int ParentChildPD[2];
    char writeBuffer[BUFFER_SIZE];
    char readBuffer[BUFFER_SIZE];

    handleError(pipe(ChildParentPD));
    handleError(pipe(ParentChildPD));
    printf("(child -> parent) pipe descripters:%d %d\n", ChildParentPD[0], ChildParentPD[1]);
    printf("(parent -> child) pipe descripters:%d %d\n", ParentChildPD[0], ParentChildPD[1]);

    int forkId;
    handleError(forkId = fork());

    if (forkId == 0) {
        // Child Process
        close(ChildParentPD[READ]);
        close(ParentChildPD[WRITE]);

        //   write messages
        printf("\nEnter the message from child to parent:\n");
        readMessageFromUser(writeBuffer, BUFFER_SIZE);
        printf("Child: writing...\n");
        // strcpy(writeBuffer, "hello");
        handleError(write(ChildParentPD[WRITE], writeBuffer, sizeof(writeBuffer)));

        //  read messages
        printf("Child: reading...\n");
        handleError(read(ParentChildPD[READ], readBuffer, sizeof(readBuffer)));
        printf("Message (Parent -> Child): %s\n", getCustomMessage(readBuffer));
    } else {
        // Parent Process
        close(ChildParentPD[WRITE]);
        close(ParentChildPD[READ]);

        //  read messages
        printf("Parent: reading...\n");
        handleError(read(ChildParentPD[READ], readBuffer, sizeof(readBuffer)));
        printf("Message (Child -> Parent): %s\n", getCustomMessage(readBuffer));

        //  write messages
        printf("\nEnter the message from parent to child:\n");
        readMessageFromUser(writeBuffer, BUFFER_SIZE);
        printf("Parent: writing...\n");
        // strcpy(writeBuffer, "This is parent talking");
        handleError(write(ParentChildPD[WRITE], writeBuffer, sizeof(writeBuffer)));
    }
    return 0;
}
