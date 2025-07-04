#include "header.h"
extern char homeDirectory[4096];
extern char pres_dir[200];
extern char prev_dir[200];

void executehop(char *Command)
{
    char Output[4096];
    removeExtraSpaces(Command, Output);
    int n = strlen(Output);
    getcwd(pres_dir, sizeof(pres_dir));
    if (n == 3)
    {
        getcwd(prev_dir, sizeof(prev_dir));
        printf("%s\n", homeDirectory);
        chdir(homeDirectory);
    }
    else if (n == 5 && (Output[4] == '~' || Output[4] == '.' || Output[4] == '-'))
    {
        if (Output[4] == '~')
        {
            getcwd(prev_dir, sizeof(prev_dir));
            printf("%s\n", homeDirectory);
            chdir(homeDirectory);
        }
        else if (Output[4] == '.')
        {
            getcwd(prev_dir, sizeof(prev_dir));
            printf("%s\n", pres_dir);
        }
        else if (Output[4] == '-')
        {
            if (prev_dir == NULL)
            {
                strcpy(prev_dir, homeDirectory);
            }
            chdir(prev_dir);
            printf("%s\n", prev_dir);
            strcpy(prev_dir, pres_dir);
        }
    }
    else if (n == 6 && Output[4] == '.' && Output[5] == '.')
    {
        getcwd(prev_dir, sizeof(prev_dir));
        char *temp = strrchr(pres_dir, '/');
        *temp = '\0';
        printf("%s\n", pres_dir);
        chdir(pres_dir);
    }
    else
    {
        char *command[100];
        int counter = 0;
        command[counter] = strtok(Command, " ");
        while (command[counter] != NULL)
        {
            counter++;
            command[counter] = strtok(NULL, " ");
        }
        if (counter == 2 && (command[1][0] == '~') && (command[1][1] == '/'))
        {
            getcwd(prev_dir, sizeof(prev_dir));
            int original_len = strlen(command[1]);
            chdir(homeDirectory);
            char new_string[original_len];
            strcpy(new_string, command[1] + 2);
            if (chdir(new_string))
            {
                printf("No such folder\n");
            }
            else
            {
                getcwd(pres_dir, sizeof(pres_dir));
                printf("%s\n", pres_dir);
            }
        }
        else if (counter == 2)
        {
            if (chdir(command[1]) == 0)
            {
                getcwd(prev_dir, sizeof(prev_dir));
                getcwd(pres_dir, sizeof(pres_dir));
                printf("%s\n", pres_dir);
            }
            else
            {
                printf("No such folder: %s\n", command[1]);
                perror("chdir failed");
            }
        }
        else if (counter > 2)
        {
            for (int i = 1; i < counter; i++)
            {
                getcwd(prev_dir, sizeof(prev_dir));
                if (chdir(command[i]) == 0)
                {
                    getcwd(pres_dir, sizeof(pres_dir));
                    printf("%s", pres_dir);
                    printf("\n");
                }
                else if ((command[i][0] == '.') && (command[i][1] == '.'))
                {
                    getcwd(prev_dir, sizeof(prev_dir));
                    char *temp = strrchr(pres_dir, '/');
                    *temp = '\0';
                    printf("%s", pres_dir);
                    printf("\n");
                    chdir(pres_dir);
                }
                else if ((command[i][0] == '~') && (command[i][1] == '/'))
                {
                    chdir(homeDirectory);
                    int original_len = strlen(command[1]);
                    char new_string[original_len];
                    strcpy(new_string, command[1] + 2);
                    if (chdir(new_string) == 0)
                    {
                        printf("No such folder\n");
                    }
                    else
                    {
                        getcwd(pres_dir, sizeof(pres_dir));
                        printf("%s\n", pres_dir);
                    }
                }
                else
                {
                    printf("No such folder\n");
                    break;
                }
            }
        }
        else
        {
            printf("%d\n", counter);
            printf("%d\n", chdir(command[1]));
        }
    }
}