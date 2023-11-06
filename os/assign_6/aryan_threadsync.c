#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#define BUFFER_SIZE 10

int in = 0;
int buffer[BUFFER_SIZE];
int out = 0;
int wait_time = 1;

sem_t empty, full;
pthread_mutex_t mutex;

void insert(int item) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}
int delete() {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return item;
}

void* producer(void* args) {
    int item = 0;
    while (item < (int)(size_t)args) {
        item++;
        printf("Produced : %d\n", item);
        insert(item);
        sleep(wait_time);
    }
}

void* consumer(void* args) {
    int i = 0;
    while (i < (int)(size_t)args) {
        int item = delete ();
        printf("Consumed : %d\n", item);
        sleep(wait_time);
        i++;
    }
}

int main() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_t producer_thread, consumer_thread;

    int n;
    printf("Enter the number of items : ");
    scanf("%d", &n);

    pthread_create(&producer_thread, NULL, producer, (void*)(size_t)n);
    pthread_create(&consumer_thread, NULL, consumer, (void*)(size_t)n);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);
    return 0;
}
