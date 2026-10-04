#include <unistd.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    struct stat sb;    
    char buffer[1024];
    
    if (fstat(STDIN_FILENO, &sb) == -1) 
    {
        perror("Error, fstat");
        return 1;
    }

    if (S_ISFIFO(sb.st_mode)) 
    {
        if (argc < 2)
        {
            printf("Error, argc < 2");
            exit(1);
        }
        while (fgets(buffer, sizeof(buffer), stdin) != 0) 
        {
            if (strstr(buffer, argv[1]) != 0)
            {
                printf("%s", buffer);
            }
        }
    } 
    else
    {
        if (argc < 3)
        {
            printf("Error, argc < 3");
            exit(1);
        }
        FILE *fp;
        if((fp=fopen(argv[2], "r")) == 0)
        {
            printf ("Cannot open file.\n");
            exit(1);
        }
        while (fgets(buffer, sizeof(buffer), fp) != 0) 
        {
            if (strstr(buffer, argv[1]) != 0)
            {
                printf("%s", buffer);
            }
        }
    }
    return 0;
}