#include "header.h"
extern int prev_prom;
extern char prev_prom_msg[255];
extern char homeDirectory[4096];

void execute_simple_command(char *command)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        char *argv[] = {command, NULL};
        if (execvp(command, argv) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                printf("Command exited with non-zero status: %d\n", exit_status);
            }
        }
        else if (WIFSIGNALED(status))
        {
            printf("Command terminated by signal: %d\n", WTERMSIG(status));
        }
    }
}

int is_executable(char *file_path)
{
    struct stat file_stat;
    if (stat(file_path, &file_stat) == -1)
    {
        printf("stat error");
        return 0;
    }
    if ((file_stat.st_mode & S_IXUSR) || (file_stat.st_mode & S_IXGRP) || (file_stat.st_mode & S_IXOTH))
    {
        return 1;
    }
    return 0;
}

int has_directory_access(char *dir_path)
{
    struct stat dir_stat;
    if (stat(dir_path, &dir_stat) == -1)
    {
        printf("stat error\n");
        return 0;
    }
    if (!S_ISDIR(dir_stat.st_mode))
    {
        return 0;
    }
    if (access(dir_path, R_OK | X_OK) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void recursive_search(char *curr_path, char *prefix, char *path_to_be_stored, int *cnt, int *type, int d, int f, int *file_type)
{
    struct dirent *entry;
    DIR *dp = opendir(curr_path);
    if (dp == NULL)
    {
        printf("No such directory: %s (Wait a minute)\n", curr_path);
        return;
    }
    char new_path[1024];
    size_t prefix_len = strlen(prefix);

    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        sprintf(new_path, "%s/%s", curr_path, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            if (f == 0 && strcmp(entry->d_name, prefix) == 0)
            {
                printf("\033[34m%s\033[0m\n", new_path);
                (*cnt)++;
                if ((*cnt) == 1)
                {
                    strcpy(path_to_be_stored, new_path);
                    *(file_type) = 0;
                    (*type) = has_directory_access(new_path);
                }
            }
            recursive_search(new_path, prefix, path_to_be_stored, cnt, type, d, f, file_type);
        }
        else
        {
            if (d == 0 && (strcmp(entry->d_name, prefix) == 0 ||
                           (strncmp(entry->d_name, prefix, prefix_len) == 0 && entry->d_name[prefix_len] == '.')))
            {
                (*cnt)++;
                sprintf(new_path, "%s/%s", curr_path, entry->d_name);
                printf("\033[32m%s\033[0m\n", new_path);
                if ((*cnt) == 1)
                {
                    *(file_type) = 1;
                    strcpy(path_to_be_stored, new_path);
                    (*type) = is_executable(new_path);
                }
            }
        }
    }
    closedir(dp);
}

void searchingDirectory(char target[], char search_path[], int d, int e, int f)
{
    struct dirent *entry;
    DIR *dp = opendir(search_path);

    if (dp == NULL)
    {
        printf("No such directory: %s\n", search_path);
        return;
    }
    closedir(dp);
    int cnt = 0;
    char path[1024];
    int type = -1;
    int file_type = -1;
    recursive_search(search_path, target, path, &cnt, &type, d, f, &file_type);
    if (cnt == 1 && e == 1)
    {
        if (file_type == -1)
        {
            printf("No matching files found (unusual)\n");
        }
        else if (file_type == 0)
        {
            if (type == 1)
            {
                char command[1030];
                snprintf(command, sizeof(command), "hop %s\n", path);
                printf("hopped to : ");
                tokenize(command, &prev_prom, prev_prom_msg);
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
        else
        {
            if (type == 1)
            {
                char command[1030];
                snprintf(command, sizeof(command), "./%s", path);
                execute_simple_command(command);
            }
            else
            {
                FILE *file = fopen(path, "r"); 
                if (file == NULL)
                {
                    printf("Error opening file: %s\n", path);
                    return;
                }

                char line[1024];

                while (fgets(line, sizeof(line), file) != NULL)
                {
                    printf("%s", line);
                }
                fclose(file);
            }
        }
    }
}

void executeseek(char *Command)
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
    int d = 0;
    int e = 0;
    int f = 0;
    int i = 1;
    if (counter == 1)
    {
        printf("no target is given\n");
        return;
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
                if (command[i][1] == 'd')
                {
                    d = 1;
                }
                else if (command[i][1] == 'e')
                {
                    e = 1;
                }
                else if (command[i][1] == 'f')
                {
                    f = 1;
                }
                else
                {
                    printf("Invalid flag:-%c\n", command[i][1]);
                    return;
                }
            }
        }
        if (d == 1 && f == 1)
        {
            printf("Invalid flags!\n");
            return;
        }
        if (i == counter)
        {
            printf("no target is given\n");
            return;
        }
        if (i == counter - 1)
        {
            searchingDirectory(command[counter - 1], ".", d, e, f);
        }
        else
        {
            char target[4096];
            strcpy(target, command[i]);
            i++;
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
            searchingDirectory(target, path, d, e, f);
        }
    }
}