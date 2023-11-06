#include <stdio.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    printf("missing me\n");
    pid_t p_id = getpid(), pp_id = getppid();
    pid_t fork_id = fork();
    // printf("hello hi: %d %d %d\n", p_id, pp_id, fork_id);

    int number = 100;
    int childStatus;
        wait(&childStatus);
    if (fork_id > 0)
        for (int i = 0; i < number; i++)
            printf("PARENT:%d\n", i);
    else
    {
        for (int j = 0; j < number; j++)
            printf("\t\tCHILD:%d\n", j);
        printf("Child Status: %d", childStatus);
    }

    return 0;
}
