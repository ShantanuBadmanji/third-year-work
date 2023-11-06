#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 256

struct sharedMemoryBlock {
    bool isLastMessage;
    bool isMessageRead;
    int messageSize;
    char messageBuffer[BUFFER_SIZE];
};

void handleError(int returnValue, char *error);
struct sharedMemoryBlock *getSharedMemoryVariable(char *fileName, int projectId, int memorySize);

int main(int argc, char const *argv[]) {
    struct sharedMemoryBlock *shmAddr = getSharedMemoryVariable("shared_mem.txt", 1212, sizeof(struct sharedMemoryBlock));

    bool isLastMessage = false;
    do {
        // wait for the client to read the message
        printf("\nWaiting for the message to be written....\n");
        while (shmAddr->messageSize <= 0) {
        };
        // copy message from shared-memory-block to local message-buffer
        char messageBuffer[BUFFER_SIZE];
        strcpy(messageBuffer, shmAddr->messageBuffer);
        isLastMessage = shmAddr->isLastMessage;
        memset(shmAddr->messageBuffer, '\0', BUFFER_SIZE);
        shmAddr->messageSize = 0;

        // write message to user
        printf("\n\nThe received message:\n");
        printf("%s\n", messageBuffer);
        // puts(messageBuffer);

        // check if this is last message/
        isLastMessage = shmAddr->isLastMessage;

        // set message-read flag to true
        shmAddr->isMessageRead = true;
    } while (!isLastMessage);

    handleError(shmdt(shmAddr), "Error in detaching the shared memory segment from the current process");

    return 0;
}

/*Handle the errors*/
void handleError(int returnValue, char *error) {
    if (returnValue == -1) {
        perror(error);
        exit(-1);
    }
}

struct sharedMemoryBlock *getSharedMemoryVariable(char *fileName, int projectId, int memorySize) {
    // get a System V IPC Key
    key_t key;
    handleError(key = ftok(fileName, projectId), "Error in getting a IPC key");
    printf("key is :%d\n", key);

    // getting Shared memory segment ID
    int shmId;
    handleError(shmId = shmget(key, memorySize, IPC_CREAT | 0600), "Error in getting Shared memory segment ID");
    printf("shmId is :%d\n", shmId);

    // attach the shared memory segment to the current process
    void *shmAddr;
    handleError((shmAddr = shmat(shmId, NULL, 0)) == (void *)-1 ? -1 : 0, "Error in attaching the shared memory segment to current process");
    printf("shmAddr is :%p\n", shmAddr);

    return (struct sharedMemoryBlock *)shmAddr;
}
