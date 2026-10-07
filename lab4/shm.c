#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

struct SharedData {
    pid_t owner_pid;     
    time_t time_fist;
    pid_t pid_first;
};

int main() {
    char *name = "/my_shared_memory";
    int SIZE = sizeof(struct SharedData);

    int shm_fd = shm_open(name, O_RDWR, 0666);
    
    if (shm_fd != -1) {
        struct SharedData *old_ptr = (struct SharedData *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        
        pid_t old_pid = old_ptr->owner_pid;
        
        if (old_pid > 0) {
            printf("Find old copy (PID: %d).\n", old_pid);
            if (kill(old_pid, SIGTERM) == 0) {
                sleep(1); 
            } else if (errno == ESRCH) {
                printf("Old process kill\n");
            }
        }
        munmap(old_ptr, SIZE);
        close(shm_fd);
    }

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    
    struct SharedData *ptr = (struct SharedData *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    ptr->owner_pid = getpid();

    printf("Program start PID: %d\n", getpid());
    while (1)
    {
        ptr->time_fist = time(0);
        ptr->pid_first = getpid();
        sleep(1);
    }

    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(name);

    return 0;
}
