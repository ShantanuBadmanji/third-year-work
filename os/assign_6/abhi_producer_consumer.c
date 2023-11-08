#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define size 5
int in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex;
int buffer[size];

void *producer(void) {
    char msg[5] = {'A', 'B', 'C', 'D', 'E'};
    for (int i = 0; i < size; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = msg[i];
        printf("The character produced by the producer %c on%d\n", buffer[in], in);
        in = (in + 1) % size;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void) {
    for (int i = 0; i < size; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        char c = buffer[out];
        printf("The character consumed by the consumer %cfrom %d\n", c, out);
        out = (out + 1) % size;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
int main() {
    pthread_t prod[size], consume[size];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, size);
    sem_init(&full, 0, 0);
    for (int i = 0; i < size; i++) {
        pthread_create(&prod[i], NULL, (void *)producer, NULL);
    }
    for (int i = 0; i < size; i++) {
        pthread_create(&consume[i], NULL, (void *)consumer, NULL);
    }
    for (int i = 0; i < size; i++) {
        pthread_join(prod[i], NULL);
    }
    for (int i = 0; i < size; i++) {
        pthread_join(consume[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}