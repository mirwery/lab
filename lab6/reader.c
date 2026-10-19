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
    int fd;
    while ((fd = shm_open(SHM_NAME, O_RDWR, 0666)) == -1) {
        fprintf(stderr, "Ждём writer (shm_open)...\n");
        sleep(1);
    }
    char *shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);

    int semid;
    while ((semid = semget(SEM_KEY, 2, 0666)) == -1) {
        fprintf(stderr, "Ждём writer (semget)...\n");
        sleep(1);
    }
    while (1) {
        struct sembuf wait_ready = { SEM_READY, -1, 0 };
        if (semop(semid, &wait_ready, 1) == -1) { perror("semop"); exit(1); }

        struct sembuf lock_mtx = { SEM_MUTEX, -1, 0 };
        if (semop(semid, &lock_mtx, 1) == -1) { perror("semop"); exit(1); }

        char buf[SHM_SIZE];
        strncpy(buf, shm, SHM_SIZE - 1);
        buf[SHM_SIZE - 1] = '\0';

        struct sembuf unlock_mtx = { SEM_MUTEX, +1, 0 };
        if (semop(semid, &unlock_mtx, 1) == -1) { perror("semop"); exit(1); }

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char ts[64];
        strftime(ts, sizeof ts, "%Y-%m-%d %H:%M:%S", t);
        printf("[%s] PID читателя=%d | Получено: %s\n",
               ts, getpid(), buf);
        fflush(stdout);
    }
}