#include <unistd.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct flag_cat
{
    bool n;
    bool E;
    bool b;
};


char print_dollar(bool flag)
{
    if (flag)
    {
        return '$';
    }
    return ' ';
}



int main(int argc, char* argv[])
{
    struct flag_cat flag;
    int opt;
    flag.n = 0;
    flag.E = 0;
    flag.b = 0;
    while ((opt = getopt(argc, argv, "nbE")) != -1) 
    {
        switch (opt)
        {
        case 'n':
            flag.n = 1;
            break;
        case 'b':
            if (flag.n == 1)
            {
                flag.n = 0;
            }
            flag.b = 1;
            break;
        case 'E':
            flag.E = 1;
            break;
        }
    }
    FILE *fp;
    if((fp=fopen(argv[optind], "r"))== 0)
    {
        printf ("Cannot open file.\n");
        exit(1);
    }
    int count = 1;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != 0)
    {
        bool status_str = 1;
        int indx = strcspn(buffer, "\n");
        if (buffer[0] == '\n' || buffer[0] == '\0')
        {
            status_str = 0;
        }
        buffer[indx] = print_dollar(flag.E);
        buffer[++indx] = '\n';
        buffer[++indx] = '\0';
        if (flag.n == 1)
        {
            printf("%d %s", count, buffer);
            ++count;
        }
        else if (flag.b == 1)
        {
            if (status_str == 1)
            {
                printf("%d %s", count, buffer);
                ++count;
            }  
            else
            {
                printf("%s", buffer);
            }
        }
        else
        {
            printf("%s", buffer);
        }
    }
    printf("n: %d ", flag.n);
    printf("E: %d ", flag.E);
    printf("b: %d ", flag.b);
    return 0;
}