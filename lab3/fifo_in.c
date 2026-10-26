#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char** argc)
{
    const char* path_fifo = "fifo.txt";
    if (mkfifo(path_fifo, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return 1;
    }

    struct stat st;
    if (stat(path_fifo, &st) == -1)
    {
        perror("stat");
        return 1;
    }
    if (!S_ISFIFO(st.st_mode))
    {
        fprintf(stderr, "Error: %s exists and is not a FIFO\n", path_fifo);
        return 1;
    }

    int fd = open(path_fifo, O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    pid_t pid = getpid();
    if (pid == -1)
    {
        perror("getpid");
        close(fd);
        return 1;
    }

    time_t current_time = time(0);
    if (current_time == (time_t)-1)
    {
        perror("time");
        close(fd);
        return 1;
    }

    printf("--------------------------Process1 input time-------------------------\n");
    printf("Process 1   pid:%d\n", pid);

    ssize_t written = write(fd, &current_time, sizeof(time_t));
    if (written == -1)
    {
        perror("write(time)");
        close(fd);
        return 1;
    }
    if (written != (ssize_t)sizeof(time_t))
    {
        fprintf(stderr, "Error: write(time) wrote %zd of %zu bytes\n",
                written, sizeof(time_t));
        close(fd);
        return 1;
    }

    printf("----------------------------------------------------------------------\n\n");
    sleep(5);

    printf("--------------------------Process1 input pid--------------------------\n");

    written = write(fd, &pid, sizeof(pid_t));
    if (written == -1)
    {
        perror("write(pid)");
        close(fd);
        return 1;
    }
    if (written != (ssize_t)sizeof(pid_t))
    {
        fprintf(stderr, "Error: write(pid) wrote %zd of %zu bytes\n",
                written, sizeof(pid_t));
        close(fd);
        return 1;
    }

    printf("----------------------------------------------------------------------\n\n");

    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    printf("Process 1 finish\n");
    return 0;
}