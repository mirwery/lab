#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SHM_NAME  "/myshm_time"
#define SHM_SIZE  256
#define SEM_KEY   0x1234
#define SEM_MUTEX 0
#define SEM_READY 1

int main(void) {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); exit(1); }
    if (ftruncate(fd, SHM_SIZE) == -1) { perror("ftruncate"); exit(1); }

    char *shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);

    int semid = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    if (semid == -1) { perror("semget"); exit(1); }
    semctl(semid, SEM_MUTEX, SETVAL, 1);
    semctl(semid, SEM_READY, SETVAL, 0);

    while (1) {
        struct sembuf lock_mtx = { SEM_MUTEX, -1, 0 };
        if (semop(semid, &lock_mtx, 1) == -1) { perror("semop"); exit(1); }

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char ts[64];
        strftime(ts, sizeof ts, "%Y-%m-%d %H:%M:%S", t);
        snprintf(shm, SHM_SIZE,
                 "[%s] PID писателя=%d", ts, getpid());

        struct sembuf unlock_ready[2] = {
            { SEM_MUTEX, +1, 0 },
            { SEM_READY, +1, 0 },
        };
        if (semop(semid, unlock_ready, 2) == -1) { perror("semop"); exit(1); }

        sleep(3);
    }
}