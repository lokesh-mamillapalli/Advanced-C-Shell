#include "header.h"
extern char homeDirectory[4096];
extern int processnumber;

void innerChild(char *command) {
    int pid = getpid();
    int rc = fork();
    if (rc == 0) {
        char *args[] = {"/bin/sh", "-c", command, NULL};
        execv("/bin/sh", args);
        printf("error in child process\n");
        exit(1);
    }
    else if(rc > 0){
        char file_path[8192];
        int status;
        waitpid(rc, &status, 0);
        snprintf(file_path, sizeof(file_path), "%s/buffer.txt", homeDirectory);
        FILE *f = fopen(file_path, "a+");
        if (f == NULL) {
            fprintf(stderr, "Error opening file\n");
            exit(1);
        }
        fprintf(f,"%s ",command);
        if(status == 0){
            fprintf(f, "Process with ID %d has exited Normally\n", pid);
        }else{
            fprintf(f,"Process with ID %d has exited Abnormally\n",pid);
        }
        fclose(f);
    }else{
        fprintf(stderr, "Fork failed\n");
        return;
    }
}

void execute_command(char *Command, int background, int *prev_prom, char *prev_prom_msg)
{

    if (strncmp("hop", Command, 3) == 0)
    {
        executehop(Command);
    }
    else if (strncmp("proclore", Command, 8) == 0)
    {
        proclore(Command);
    }
    else if (strncmp("reveal", Command, 6) == 0)
    {
        executereveal(Command);
    }
    else if (strncmp("seek", Command, 4) == 0)
    {
        executeseek(Command);
    }
    else if (strncmp("log", Command, 3) == 0)
    {
        executelog(Command);
    }
    else
    {
        int rc = fork();
        if (rc == 0)
        {
            if(!background){
                char *argv[] = {"/bin/sh", "-c", Command, NULL};
                execv("/bin/sh", argv);
                perror("execv");
                exit(EXIT_FAILURE);
            }else{
                innerChild(Command);
                exit(0);
            }
        }
        else if (rc > 0)
        {
            if (!background)
            {
                time_t start_time, end_time;
                time(&start_time);
                waitpid(rc, NULL, WUNTRACED);
                time(&end_time);
                int time_taken = difftime(end_time, start_time);

                if (time_taken > 2)
                {
                    *prev_prom = 1;
                    int k = 0;
                    while (Command[k] != ' ')
                    {
                        k++;
                    }
                    Command[k] = '\0';
                    strcpy(prev_prom_msg, Command);
                    strcat(prev_prom_msg, " : ");
                    char time_took[32];
                    sprintf(time_took, "%d", time_taken);
                    strcat(prev_prom_msg, time_took);
                }
            }
            else
            {
                printf("[%d] %d\n", ++processnumber, rc);
            }
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
}

void tokenize(char *input, int *prev_prom, char *prev_prom_msg)
{
    int n = strlen(input);
    for(int i=0;i<n;i++){
        if(input[i] == '\t'){
            input[i] = ' ';
        }
    }
    char *Total_entry_parts[100];
    int counter = 0;
    Total_entry_parts[counter] = strtok(input, ";");
    while (Total_entry_parts[counter] != NULL)
    {
        counter++;
        Total_entry_parts[counter] = strtok(NULL, ";");
    }
    for (int i = 0; i < counter; i++)
    {
        char *Total_commands[200];
        int count = 0;
        Total_commands[count] = strtok(Total_entry_parts[i], "&");
        while (Total_commands[count] != NULL)
        {
            count++;
            Total_commands[count] = strtok(NULL, "&");
        }
        if (count == 1)
        {
            char *command = Total_commands[0];
            if (command != NULL)
            {
                lstrip(&command);
                rstrip(command);
                execute_command(command, 0, prev_prom, prev_prom_msg);
            }
        }
        else
        {
            char *command = Total_commands[count - 1];
            if (command != NULL)
            {
                lstrip(&command);
                rstrip(command);
                execute_command(command, 0, prev_prom, prev_prom_msg);
            }
            for (int j = 0; j < count - 1; j++)
            {
                char *command = Total_commands[j];
                if (command != NULL)
                {
                    lstrip(&command);
                    rstrip(command);
                    if (strlen(command) > 0)
                    {
                        execute_command(command, 1, prev_prom, prev_prom_msg);
                    }
                }
            }
        }
    }
}
