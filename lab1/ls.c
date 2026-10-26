#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <bits/getopt_core.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <limits.h>

void print_permissions(mode_t mode) {
    if (S_ISDIR(mode))        printf("d");
    else if (S_ISLNK(mode))   printf("l");
    else if (S_ISCHR(mode))   printf("c");
    else if (S_ISBLK(mode))   printf("b");
    else if (S_ISFIFO(mode))  printf("p");
    else if (S_ISSOCK(mode))  printf("s");
    else                      printf("-");

    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");

    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");

    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

struct flag_ls
{
    bool l;
    bool a;
};

int main(int argc, char** argv)
{
    struct stat sb;
    struct flag_ls flag = {0};
    int opt;
    char path[PATH_MAX];
    char fullpath[PATH_MAX];
    struct stat file_stat;
    struct dirent **namelist;
    bool status_out = 0;

    fstat(STDOUT_FILENO, &sb);

    if (S_ISFIFO(sb.st_mode))
    {
        setvbuf(stdout, NULL, _IONBF, 0);
        status_out = 1;
    }

    while ((opt = getopt(argc, argv, "la")) != -1)
    {
        switch (opt)
        {
        case 'l':
            flag.l = 1;
            break;
        case 'a':
            flag.a = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-la] [path]\n", argv[0]);
            exit(1);
        }
    }

    if (optind >= argc)
    {
        strcpy(path, ".");
    }
    else
    {
        if (strlen(argv[optind]) >= sizeof(path))
        {
            printf("Error, size path > %zu\n", sizeof(path) - 1);
            exit(1);
        }
        strcpy(path, argv[optind]);
    }

    int n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        exit(1);
    }
    int start_fileout = 0;
    if (flag.a == 0)
    {
        start_fileout = n;
        for (int i = 0; i < n; ++i)
        {
            if (namelist[i]->d_name[0] != '.')
            {
                start_fileout = i;
                break;
            }
        }
    }

    if (flag.l == 0)
    {
        for (int i = start_fileout; i < n; ++i)
        {
            printf("%s ", namelist[i]->d_name);
            if (status_out == 1)
            {
                printf("\n");
            }
            free(namelist[i]);
        }
        free(namelist);
        return 0;
    }
    else
    {
        for (int i = start_fileout; i < n; ++i)
        {
            int written = snprintf(fullpath, sizeof(fullpath),
                                   "%s/%s", path, namelist[i]->d_name);
            if (written < 0 || (size_t)written >= sizeof(fullpath))
            {
                printf("Error, path too long\n");
                free(namelist[i]);
                continue;
            }

            if (lstat(fullpath, &file_stat) == 0)
            {
                char color[16] = "\033[0m";
                if (S_ISDIR(file_stat.st_mode))
                {
                    strcpy(color, "\033[1;34m");
                }
                else if (S_ISREG(file_stat.st_mode))
                {
                    if (file_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
                    {
                        strcpy(color, "\033[1;32m");
                    }
                }
                else if (S_ISCHR(file_stat.st_mode))
                {
                    strcpy(color, "\033[1;36m");
                }

                print_permissions(file_stat.st_mode);
                printf(" %d ", file_stat.st_nlink);

                struct passwd *pw = getpwuid(file_stat.st_uid);
                struct group  *gr = getgrgid(file_stat.st_gid);
                if (pw != NULL)
                {
                    printf("%s ", pw->pw_name);
                }
                else
                {
                    printf("%d ", file_stat.st_uid);
                }
                if (gr != NULL)
                {
                    printf("%s ", gr->gr_name);
                }
                else
                {
                    printf("%d ", file_stat.st_gid);
                }

                printf("%lld ", (long long)file_stat.st_size);

                struct tm *time_info = gmtime(&file_stat.st_mtime);
                char time_string[64];
                strftime(time_string, sizeof(time_string), "%b %e %H:%M", time_info);
                printf("%s ", time_string);

                printf("%s%s\033[0m\n", color, namelist[i]->d_name);
            }
            else
            {
                perror(fullpath);
            }
            free(namelist[i]);
        }
        free(namelist);
    }
    return 0;
}