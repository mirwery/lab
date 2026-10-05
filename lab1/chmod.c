#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

struct flag_group
{
    bool user;
    bool group;
    bool other;
};

mode_t add_rights(mode_t old_mode, mode_t rights, int status_add)
{
    if (status_add == 0)
    {
        old_mode = old_mode & ~(rights);
    }
    else
    {
        old_mode = old_mode | (rights);
    }
    return old_mode;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Error, argc < 3");
        exit(1);
    }

    mode_t mode = (mode_t)strtol(argv[1], NULL, 8);
    struct stat file_stat;
    struct flag_group flag = {0};
    int indx = 0;

    if (mode != 0)
    {
        if (chmod(argv[2], mode) != 0)
        {
            printf("Error, uncorrect mode");
            exit(1);
        }
    }
    else
    {
        if (stat(argv[2], &file_stat) != 0) 
        {
            exit(1);
        }
        if (argv[1][0] == '+' || argv[1][0] == '-')
        {
            flag.group = 1;
            flag.user = 1;
            flag.other = 1;
        }

        while ((argv[1][indx] != '+') && (argv[1][indx] != '-'))
        {
            if (argv[1][indx] == 'g')
            {
                flag.group = 1;
            }
            else if (argv[1][indx] == 'u')
            {
                flag.user = 1;
            }
            else if (argv[1][indx] == 'o')
            {
                flag.other = 1;
            }
            else
            {
                printf("Error, uncorrect flag");
                exit(1);
            }
            ++indx;
        }
        bool status_add = 1;
        if (argv[1][indx] == '-')
        {
            status_add = 0;
        } 
        ++indx;
        mode_t new_mode = file_stat.st_mode;

        while (argv[1][indx] != '\0')
        {
            if (argv[1][indx] == 'r')
            {
                if (flag.group == 1)
                {
                    new_mode = add_rights(new_mode, S_IRGRP, status_add);
                }
                if (flag.user == 1)
                {
                    new_mode = add_rights(new_mode, S_IRUSR, status_add);
                }
                if (flag.other = 1)
                {
                    new_mode = add_rights(new_mode, S_IROTH, status_add);
                }
            }
            else if (argv[1][indx] == 'w')
            {
                if (flag.group == 1)
                {
                    new_mode = add_rights(new_mode, S_IWGRP, status_add);
                }
                if (flag.user == 1)
                {
                    new_mode = add_rights(new_mode, S_IWUSR, status_add);
                }
                if (flag.other = 1)
                {
                    new_mode = add_rights(new_mode, S_IWOTH, status_add);
                }
            }
            else if (argv[1][indx] == 'x')
            {
                if (flag.group == 1)
                {
                    new_mode = add_rights(new_mode, S_IXGRP, status_add);
                }
                if (flag.user == 1)
                {
                    new_mode = add_rights(new_mode, S_IXUSR, status_add);
                }
                if (flag.other = 1)
                {
                    new_mode = add_rights(new_mode, S_IXOTH, status_add);
                }
            }
            else if (argv[1][indx] == 's')
            {
                if (flag.group == 1)
                {
                    new_mode = add_rights(new_mode, S_ISGID, status_add);
                }
                if (flag.user == 1)
                {
                    new_mode = add_rights(new_mode, S_ISUID, status_add);
                }
                if (flag.other = 1)
                {
                    new_mode = add_rights(new_mode, S_ISVTX, status_add);
                }
            }
            ++indx;
        }
        chmod(argv[2], new_mode);
    }
    


    //printf(" u: %d \n g: %d \n o: %d \n", flag.user, flag.group, flag.other);
    //printf("%d", mode);

    return 0;
}