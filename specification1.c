#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"
#include <sys/types.h>
#include <sys/wait.h>
#define max_char 4096

extern char homeDirectory[];

void printing(int *prev_prom, char *prev_prom_msg)
{
    char systemname[max_char];
    char directory[max_char];
    char *username = getlogin();
    gethostname(systemname, sizeof(systemname));
    getcwd(directory, sizeof(directory));
    int n = strlen(homeDirectory);
    int m = strlen(directory);
    int i = 0;
    int flag = 0;
    while (i < n && i < m && directory[i] == homeDirectory[i])
    {
        i++;
    }
    if (i == n && i < m)
    {
        flag = 1;
    }
    else if (i == n && i == m)
    {
        flag = 2;
    }
    else
    {
    }
    if (flag == 0)
    {

        if (*prev_prom == 1)
        {
            printf("<%s@%s:%s %s>", username, systemname, directory, prev_prom_msg);
            *prev_prom = 0;
            return;
        }
        else
        {
            printf("<%s@%s:%s>", username, systemname, directory);
        }
    }
    else if (flag == 1)
    {

        if (*prev_prom == 1)
        {
            printf("<%s@%s:~", username, systemname);
            while (i < m)
            {
                printf("%c", directory[i]);
                i++;
            }
            printf(" %s>", prev_prom_msg);
            *prev_prom = 0;
        }
        else
        {
            printf("<%s@%s:~", username, systemname);
            while (i < m)
            {
                printf("%c", directory[i]);
                i++;
            }
            printf(">");
        }
    }
    else
    {

        if (*prev_prom == 1)
        {
            printf("<%s@%s:~", username, systemname);
            printf(" %s>", prev_prom_msg);
            *prev_prom = 0;
        }
        else
        {
            printf("<%s@%s:~>", username, systemname);
        }
    }
}