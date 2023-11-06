#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int first(void *index) {
    int i;
    for (i = 0; i < 3; i++)

    {
        printf("\n%d:%d ", i, *((int *)index));
        // sleep(1);
    }
    pthread_exit(index);
}

int main()

{
    pthread_t th;
    int status;

    for (int i = 0; i < 3; i++) {
        pthread_create(&th, NULL, (void *)&first, (void *)&i);
        sleep(1);
        pthread_join(th, (void *)&status);
        printf("\n%d\n", status);
    }
    return 0;
}