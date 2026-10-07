#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        printf("Error, create pipe \n");
        exit(1);
    }

    switch ((pid = fork()))
    {
    case -1:
        printf("Error, fork \n");
        exit(1);
        break;
    case 0:
        time_t time_mess;
        pid_t pid_parent;
        close(pipefd[1]);
        printf("--------------------------Child input time---------------------------\n");
        printf("Child, process    pid:%d\n", getpid());;
        read(pipefd[0], &time_mess, sizeof(time_t));
        printf("---------------------------------------------------------------------\n\n");
        sleep(1);
        printf("--------------------------Child input pid----------------------------\n");
        read(pipefd[0], &pid_parent, sizeof(pid_t));
        printf("Mess parent time: %d  pid: %d \n", time_mess, pid_parent);
        printf("Mess child time: %d  pid: %d \n", time(0), getpid());
        printf("---------------------------------------------------------------------\n\n");
        close(pipefd[0]);
        printf("Child finish\n");
        exit(0);
        break;
    default:
        close(pipefd[0]); 
        printf("--------------------------Parent output time-------------------------\n");
        printf("Parent, process    pid:%d\n", pid);
        time_t current_time = time(0);
        write(pipefd[1], &current_time, sizeof(time_t));
        printf("---------------------------------------------------------------------\n\n");
        sleep(5);
        printf("--------------------------Parent output pid--------------------------\n");
        printf("Parent, process \n");
        write(pipefd[1], &pid, sizeof(pid_t));
        printf("---------------------------------------------------------------------\n\n");
        close(pipefd[1]);
        wait(0); 
        printf("Parent finish\n");
        exit(0);
        break;
    }
    return 0;
} 