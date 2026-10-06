#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

void succes_exit()
{
    printf("Succes finish programm: code 0 \n");
}

void erorr_return(int status, void* args)
{
    if (status == 30)
    {
        printf("Error, custom return function, atexit \n");
    }
    if (status == -1)
    {
        printf("Error, fork\n");
    }
}

void custom_ctr_c(int sig)
{
    printf("Custom ctr c \n");
    exit(0);
}

void custom_term(int sig)
{
    printf("Signal term\n");
    exit(0);
}


int main()
{
    struct sigaction sa;
    sa.sa_handler = custom_term;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        printf("Error, call sigaction");
        exit(1);
    }
    pid_t pid;
    int status_fork;
    int exit_status;
    signal(SIGINT, custom_ctr_c);
    if (on_exit(erorr_return, 0) != 0)
    {
        printf("Error, custom return function, on_exit \n");
        exit(1);
    }
    if (atexit(succes_exit) != 0)
    {
        exit(30);
    }
    switch ((status_fork = fork()))
    {
        case -1:
            exit(-1);
            break;
        case 0:
            printf("Child process \n");
            printf("Child pid: %d \n", getpid());
            printf("Child ppid: %d \n", getppid());
            printf("---------------------------------------------------------------------\n");
            printf("Child: exit!\n");
            exit(0);
            break;
        default:
            printf("---------------------------------------------------------------------\n");
            printf("Parent process \n");
            printf("Parent pid: %d \n", getpid());
            printf("Parent ppid: %d \n", getppid());
            printf("Child parent pid: %d \n", status_fork);
            printf("---------------------------------------------------------------------\n");
            wait(&exit_status);
            printf("Parent: code return child:%d\n", exit_status);
            int a;
            scanf("%d", &a);
            while (a != 0)
            {
                scanf("%d", &a);
            }
    }
    return 0;
}