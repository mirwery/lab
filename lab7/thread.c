#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_rwlock_t rwlock;
int count = 1;


void* read_arr(void* arg)
{
    int num_thread = *(int*) arg; 
    pthread_t tid = pthread_self();
    
    for (int i = 0; i < 10; ++i)
    {
        usleep((rand() % 90 + 10) * 1000); 
        pthread_rwlock_rdlock(&rwlock);
        int current_count = count;
        pthread_rwlock_unlock(&rwlock);
        printf("[Поток %d] count = %d\n", num_thread, current_count);
    }
    printf("--- Поток %d завершился (tid: %lu) ---\n", num_thread, (unsigned long)tid);
    return 0;
}

void* counter_arr(void* arg)
{
    for (int i = 0; i < 100000; ++i)
    {
        usleep(7);
        pthread_rwlock_wrlock(&rwlock);
        count += 1;
        pthread_rwlock_unlock(&rwlock);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    pthread_t arr_thread_read[10];
    pthread_t thread_counter;
    int thread_ids[10]; 
    if (pthread_rwlock_init(&rwlock, NULL) != 0) {
        perror("Error, init rwlock");
        return 1;
    }

    pthread_create(&thread_counter, 0, counter_arr, 0);
    for (int i = 0; i < 10; ++i)
    {
        thread_ids[i] = i;
        if (pthread_create(&arr_thread_read[i], 0, read_arr, &thread_ids[i]) != 0) 
        {  
            printf("Error, creat thread \n");
            exit(1);
            return 1;
        }
    }
    
    for (int i = 0; i < 10; ++i)
    {
        pthread_join(arr_thread_read[i], 0);
    }
    pthread_join(thread_counter, 0);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
