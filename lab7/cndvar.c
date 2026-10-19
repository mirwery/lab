#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READERS_COUNT 10

// Общие ресурсы
int shared = 0;
int ready_to_read = 0; // Флаг: 1 — данные обновились, 0 — данных нет или уже прочитаны

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;

void* writer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        usleep(5);
        ++shared;
        ready_to_read = 1; 
        pthread_cond_broadcast(&condvar);

        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

// Читающие потоки
void* reader_thread(void* arg) {
    int reader_id = *(int*)arg;
    pthread_t tid = pthread_self();

    while (1) {
        pthread_mutex_lock(&mutex);

        while (ready_to_read == 0) {
            pthread_cond_wait(&condvar, &mutex);
        }
        printf("[Поток-читатель %d | tid: %lu] Считал: %d\n", reader_id, (unsigned long)tid, shared);
        ready_to_read = 0; 
        pthread_mutex_unlock(&mutex);
        usleep(10000); 
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t writer;
    pthread_t readers[READERS_COUNT];
    int reader_ids[READERS_COUNT];

    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condvar, 0);

    pthread_create(&writer, NULL, writer_thread, 0);

    for (int i = 0; i < READERS_COUNT; ++i) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader_thread, &reader_ids[i]);
    }

    pthread_join(writer, NULL);
    for (int i = 0; i < READERS_COUNT; ++i) {
        pthread_join(readers[i], NULL);
    }

    // Очистка ресурсов
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condvar);

    return 0;
}
