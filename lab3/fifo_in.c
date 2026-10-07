#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <time.h>

int main(int argc, char* argv[])
{
    char* path_fifo = "fifo.txt";
    mkfifo(path_fifo, 0666);

    int fd = open(path_fifo, O_WRONLY);
    pid_t pid = getpid();
    time_t current_time = time(0);
    printf("--------------------------Process1 input time-------------------------\n");
    printf("Process 1   pid:%d\n", pid);
    write(fd, &current_time, sizeof(time_t));
    printf("----------------------------------------------------------------------\n\n");
    sleep(5);
    printf("--------------------------Process1 input pid--------------------------\n");
    write(fd, &pid, sizeof(pid_t));
    printf("----------------------------------------------------------------------\n\n");
    close(fd);
    printf("Process 1 finish\n");
    return 0;
}