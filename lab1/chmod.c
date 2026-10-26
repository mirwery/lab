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

bool is_octal_mode(const char *s)
{
    if (s == NULL || *s == '\0')
    {
        return false;
    }

    for (; *s != '\0'; ++s)
    {
        if (*s < '0' || *s > '7')
        {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Error, argc < 3\n");
        return 1;
    }
    if (is_octal_mode(argv[1]))
    {
        mode_t mode = (mode_t)strtol(argv[1], NULL, 8);

        if (chmod(argv[2], mode) != 0)
        {
            perror("chmod");
            return 1;
        }

        return 0;
    }

    struct stat file_stat;
    struct flag_group flag = {0};
    int indx = 0;

    if (stat(argv[2], &file_stat) != 0)
    {
        perror("stat");
        return 1;
    }

    if (argv[1][0] == '+' || argv[1][0] == '-')
    {
        flag.group = true;
        flag.user = true;
        flag.other = true;
    }

    while (argv[1][indx] != '\0' &&
           argv[1][indx] != '+' &&
           argv[1][indx] != '-')
    {
        if (argv[1][indx] == 'g')
        {
            flag.group = true;
        }
        else if (argv[1][indx] == 'u')
        {
            flag.user = true;
        }
        else if (argv[1][indx] == 'o')
        {
            flag.other = true;
        }
        else
        {
            printf("Error, uncorrect flag\n");
            return 1;
        }

        ++indx;
    }

    if (argv[1][indx] == '\0')
    {
        printf("Error, no + or -\n");
        return 1;
    }

    bool status_add = true;

    if (argv[1][indx] == '-')
    {
        status_add = false;
    }

    ++indx;

    mode_t new_mode = file_stat.st_mode & 07777;

    while (argv[1][indx] != '\0')
    {
        if (argv[1][indx] == 'r')
        {
            if (flag.group)
            {
                new_mode = add_rights(new_mode, S_IRGRP, status_add);
            }
            if (flag.user)
            {
                new_mode = add_rights(new_mode, S_IRUSR, status_add);
            }
            if (flag.other)
            {
                new_mode = add_rights(new_mode, S_IROTH, status_add);
            }
        }
        else if (argv[1][indx] == 'w')
        {
            if (flag.group)
            {
                new_mode = add_rights(new_mode, S_IWGRP, status_add);
            }
            if (flag.user)
            {
                new_mode = add_rights(new_mode, S_IWUSR, status_add);
            }
            if (flag.other)
            {
                new_mode = add_rights(new_mode, S_IWOTH, status_add);
            }
        }
        else if (argv[1][indx] == 'x')
        {
            if (flag.group)
            {
                new_mode = add_rights(new_mode, S_IXGRP, status_add);
            }
            if (flag.user)
            {
                new_mode = add_rights(new_mode, S_IXUSR, status_add);
            }
            if (flag.other)
            {
                new_mode = add_rights(new_mode, S_IXOTH, status_add);
            }
        }
        else if (argv[1][indx] == 's')
        {
            if (flag.group)
            {
                new_mode = add_rights(new_mode, S_ISGID, status_add);
            }
            if (flag.user)
            {
                new_mode = add_rights(new_mode, S_ISUID, status_add);
            }
            if (flag.other)
            {
                new_mode = add_rights(new_mode, S_ISVTX, status_add);
            }
        }
        else
        {
            printf("Error, uncorrect right\n");
            return 1;
        }

        ++indx;
    }

    if (chmod(argv[2], new_mode) != 0)
    {
        perror("chmod");
        return 1;
    }

    return 0;
}