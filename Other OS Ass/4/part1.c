#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t mutexGlobal, mutexWriter;
int data = 0, readerCount = 0;

void *read(void *reader){
    sem_wait(&mutexGlobal);
    ++readerCount;
    if (readerCount == 1)
        sem_wait(&mutexWriter);
    sem_post(&mutexGlobal);
    printf("Reader: %d\t\tData Read: %d\n", ((int)reader), data);
    sleep(1);
    sem_wait(&mutexGlobal);
    --readerCount;
    if (readerCount == 0)
        sem_post(&mutexWriter);
    sem_post(&mutexGlobal);
}

void *write(void *writer) {
    sem_wait(&mutexWriter);
    ++data;
    printf("Writer: %d\t\tData Written: %d\n", ((int)writer), data);
    sleep(1);
    sem_post(&mutexWriter);
}


int main(void) {
    pthread_t readerId[5], writerId;
    sem_init(&mutexGlobal, 0, 1);
    sem_init(&mutexWriter, 0, 1);

    pthread_create(&writerId, NULL, write, (void *)0);

    int i;
    for (i = 0; i < 5; ++i) {
        pthread_create(&readerId[i], NULL, read, (void *)i);
    }

    pthread_join(writerId, NULL);

    for (i = 0; i < 5; ++i) {
        pthread_join(readerId[i], NULL);
    }


    return 0;
}