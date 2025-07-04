#include "header.h"
extern char pres_dir[200];
extern char prev_dir[200];
extern char homeDirectory[4096];

void executereveal(char *Command)
{
    char Output[4096];
    removeExtraSpaces(Command, Output);
    int n = strlen(Output);
    int counter = 0;
    char *command[100];
    command[counter] = strtok(Output, " ");
    while (command[counter] != NULL)
    {
        counter++;
        command[counter] = strtok(NULL, " ");
    }
    int a = 0;
    int i;
    int l = 0;
    if (counter == 1)
    {
        show(".", 0, 0);
    }
    else
    {
        for (i = 1; i < counter; i++)
        {
            if (command[i][0] != '-')
            {
                break;
            }
            else
            {
                int templen = strlen(command[i]);
                for (int j = 1; j < templen; j++)
                {
                    if (command[i][j] == 'l')
                    {
                        l = 1;
                    }
                    else if (command[i][j] == 'a')
                    {
                        a = 1;
                    }
                    else
                    {
                        printf("Invalid flag: %c\n", command[i][j]);
                        return;
                    }
                }
            }
        }
        if (i == counter)
        {
            show(".", a, l);
        }
        if (i != counter)
        {
            char path[4096];
            if (i == counter - 1)
            {
                if (command[counter - 1][0] == '~')
                {
                    strcpy(path, homeDirectory);
                    strcat(path, ((command[counter - 1]) + 1));
                }
                else
                {
                    strcpy(path, command[counter - 1]);
                }
            }
            else
            {
                if (command[i][0] == '~')
                {
                    strcpy(path, homeDirectory);
                    strcat(path, ((command[i]) + 1));
                }
                else
                {
                    strcpy(path, command[i]);
                }
                for (int j = i + 1; j < counter; j++)
                {
                    strcat(path, "/");
                    strcat(path, command[j]);
                }
            }
            show(path, a, l);
        }
    }
}

void show(char *path, int a, int l)
{
    struct dirent **namelist;
    int n;
    n = scandir(path, &namelist, NULL, alphasort);
    char abs_path[1000];
    if (!(path[0] == '/'))
    {
        getcwd(abs_path, sizeof(abs_path));
        strcat(abs_path, "/");
        strcat(abs_path, path);
    }
    else
    {
        strcpy(abs_path, path);
    }
    if (n < 0)
    {
        perror("scannamelist");
        return;
    }
    else if (n == 0)
    {
        printf("No of entities: %d\n", n);
    }
    else
    {
        for (int k = 0; k < n; k++)
        {
            if (namelist[k]->d_name[0] == '.' && a == 0)
            {
            }
            else
            {
                char path[1000];
                strcpy(path, abs_path);
                strcat(path, "/");
                struct stat space;
                strcat(path, namelist[k]->d_name);
                if (stat(path, &space) != -1)
                {
                    char perms[15];
                    perms[0] = '\0';
                    perms[0] = (S_ISDIR(space.st_mode)) ? 'd' : '-';
                    perms[1] = (space.st_mode & S_IRUSR) ? 'r' : '-';
                    perms[2] = (space.st_mode & S_IWUSR) ? 'w' : '-';
                    perms[3] = (space.st_mode & S_IXUSR) ? 'x' : '-';
                    perms[4] = (space.st_mode & S_IRGRP) ? 'r' : '-';
                    perms[5] = (space.st_mode & S_IWGRP) ? 'w' : '-';
                    perms[6] = (space.st_mode & S_IXGRP) ? 'x' : '-';
                    perms[7] = (space.st_mode & S_IROTH) ? 'r' : '-';
                    perms[8] = (space.st_mode & S_IWOTH) ? 'w' : '-';
                    perms[9] = (space.st_mode & S_IXOTH) ? 'x' : '-';
                    perms[10] = '\0';

                    char date_time[100];

                    struct passwd *pw = getpwuid(space.st_uid);
                    struct tm *time_info = localtime(&space.st_mtime);
                    struct group *gr = getgrgid(space.st_gid);
                    strftime(date_time, sizeof(date_time), "%b %d %H:%M", time_info);
                    if (S_ISDIR(space.st_mode))
                    {
                        if (l != 1)
                        {
                            printf("\e[34m%s\e[m\n", namelist[k]->d_name);
                        }
                        else
                        {
                            printf("\e[34m%s %3ld %s %s %8ld %s %s\e[m\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size, date_time, namelist[k]->d_name);
                        }
                    }
                    else if (S_IXUSR & space.st_mode)
                    {
                        if (l != 1)
                        {
                            printf("\e[32m%s\e[m\n", namelist[k]->d_name);
                        }
                        else
                        {
                            printf("\e[32m%s %3ld %s %s %8ld %s %s\e[m\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size, date_time, namelist[k]->d_name);
                        }
                    }
                    else
                    {
                        if (l != 1)
                        {
                            printf("%s\n", namelist[k]->d_name);
                        }
                        else
                        {
                            printf("%s %3ld %s %s %8ld %s %s\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size, date_time, namelist[k]->d_name);
                        }
                    }
                }
            }
            free(namelist[k]);
        }
        free(namelist);
        return;
    }
}