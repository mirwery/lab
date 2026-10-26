#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <time.h>

int main(int argc, char** argv)
{
    char* path_fifo = "fifo.txt";
    int fd = open(path_fifo, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "open('%s') failed: %s (errno=%d)\n",
                path_fifo, strerror(errno), errno);
        return -1;
    }

    pid_t process1_pid;
    time_t process1_time;
    printf("--------------------------Process2 get time-------------------------\n");
    if (read(fd, &process1_time, sizeof(time_t)) == -1) {
        fprintf(stderr, "read(time_t) failed: %s (errno=%d)\n",
                strerror(errno), errno);
        close(fd);
        return -1;
    }
    printf("----------------------------------------------------------------------\n\n");
    sleep(5);
    printf("--------------------------Process2 get pid--------------------------\n");
    if (read(fd, &process1_pid, sizeof(pid_t)) == -1) {
        fprintf(stderr, "read(pid_t) failed: %s (errno=%d)\n",
                strerror(errno), errno);
        close(fd);
        return -1;
    }
    printf("Mess process1 time: %d  pid: %d \n", process1_time, process1_pid);
    printf("Mess process2 time: %d  pid: %d \n", time(0), getpid());
    printf("----------------------------------------------------------------------\n\n");
    if (close(fd) == -1) {
        fprintf(stderr, "close failed: %s (errno=%d)\n",
                strerror(errno), errno);
        return -1;
    }
    printf("Process 2 finish\n");
    return 0;
}