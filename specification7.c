#include "header.h"
extern char homeDirectory[4096];
void proclore(char *command)
{
    int pid = 0;
    printf("%d\n", pid);
    int n = strlen(command);
    char output[n];
    removeExtraSpaces(command, output);
    char *Command[100];
    int counter = 0;
    Command[counter] = strtok(output, " ");
    while (Command[counter] != NULL){
        counter++;
        Command[counter] = strtok(NULL, " ");
    }

    if (counter != 1){
        pid = atoi(Command[1]);
    }
    else
    {
        pid = getpid();
    }
    char virtual_path[256];
    snprintf(virtual_path, sizeof(virtual_path), "/proc/%d/statm", pid);
    unsigned long virtual_size;
    FILE *vir_file = fopen(virtual_path, "r");
    if (vir_file != NULL){
        fscanf(vir_file, "%lu", &virtual_size);
        fclose(vir_file);
    }
    char status[8];
    char status_path[256];
    char pid_str[32];
    sprintf(pid_str, "%d", pid);
    strcpy(status_path, "/proc/");
    strcat(status_path, pid_str);
    strcat(status_path, "/status");
    FILE *status_file = fopen(status_path, "r");
    if (status_file != NULL){
        char line[280];
        while (fgets(line, sizeof(line), status_file))
        {
            if (strncmp(line, "State:", 6) != 0){
            }
            else{
                sscanf(line + 7, "%s", status);
                break;
            }
        }
        fclose(status_file);
    }

    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    char home_dir[4096];
    readlink(proc_path, proc_path, sizeof(proc_path) - 1);
    strcpy(home_dir, homeDirectory);
    if (strlen(proc_path) < strlen(home_dir)){
    }
    else{
        snprintf(proc_path, sizeof(proc_path), "~%s", proc_path + strlen(home_dir));
    }
    pid_t group_pid = tcgetpgrp(STDIN_FILENO);
    ;
    char plus = '\0';
    if (pid != group_pid){
        plus = '\0';
    }
    if (pid == group_pid){
        plus = '+';
    }

    printf("pid : %d\n", pid);
    printf("Process Status : %s%c\n", status, plus);
    printf("Process Group : %d\n", getpgid(pid));
    printf("Virtual Memory : %ld\n", virtual_size);
    printf("Executable path : %s\n", proc_path);
}
