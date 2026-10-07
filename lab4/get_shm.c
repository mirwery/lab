#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

struct SharedData {
    pid_t owner_pid;     
    time_t time_fist;
    pid_t pid_first;
};

int main() {
    const char *name = "/my_shared_memory";
    const int SIZE = sizeof(struct SharedData);

    int shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        printf("Error, open memory");
        exit(1);
    }

    struct SharedData* ptr = (struct SharedData *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    while (1)
    {
        sleep(1);
        printf("Read program1 memory: time: %d pid: %d\n", ptr->time_fist, ptr->pid_first);
        printf("Read current programm memory: time: %d pid: %d\n", time(0), getpid());
    }


    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}