#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int shared_count = 0; 
sem_t sem_count;

void* writer_thread(void* arg) {
    while (1) {
        sleep(1);
        sem_wait(&sem_count);
        shared_count++;
        sem_post(&sem_count);
    }
    return 0;
}



void* reader_thread(void* arg) {
    pthread_t tid = pthread_self();
    while (1) {
        usleep(200000);
        sem_wait(&sem_count);
        printf("[tid: %lu] Current counter value: %d\n", (unsigned long)tid, shared_count);
        sem_post(&sem_count);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    pthread_t writer, reader;
    if (sem_init(&sem_count, 0, 1) != 0) {
        perror("Error, init sem");
        return 1;
    }
    if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
        perror("Error, creat write thread");
        return 1;
    }
    if (pthread_create(&reader, NULL, reader_thread, NULL) != 0) {
        perror("Error, creat read thread");
        return 1;
    }

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    sem_destroy(&sem_count);
    return 0;
}
