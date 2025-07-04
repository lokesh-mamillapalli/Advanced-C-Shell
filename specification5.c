#include "header.h"
extern int prev_prom;
extern char prev_prom_msg[255];
extern char homeDirectory[4096];
char histo_link[255];

void add_to_file(char *input)
{
    char file_path[1024];
    strcpy(file_path, homeDirectory);
    strcat(file_path, "/example.txt");
    FILE *file1 = fopen(file_path, "r");
    int no_of_lines = 0;
    char externalbuffer[15360];
    char buffer[1024];
    char last[1024];
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
    {
        no_of_lines++;
        strcpy(last, buffer);
    }

    fclose(file1);
    if (strcmp(input, last) == 0)
    {
    }
    else
    {
        if (no_of_lines >= 15)
        {
            FILE* file2 = fopen(file_path, "r");
            char ch;
            int count = 0;
            int flag = 0;
            while ((ch = fgetc(file2)) != EOF)
            {
                if (ch == '\n' && flag == 0)
                {
                    flag = 1;
                }
                else
                {
                    if (flag == 1)
                    {
                        externalbuffer[count] = ch;
                        count++;
                    }
                }
            }
            fclose(file2);
            strcat(externalbuffer, input);
            FILE * file3 = fopen(file_path, "w");
            fprintf(file3, "%s", externalbuffer);
            fclose(file3);
        }
        else
        {
            FILE * file4 = fopen(file_path, "a+");
            if (file4 == NULL) {
            }

            fprintf(file4, "%s", input);
            fclose(file4);
        }
    }
}

void ClearFile()
{
     char file_path[1024];
    strcpy(file_path, homeDirectory);
    strcat(file_path, "/example.txt");
    FILE *file = fopen(file_path, "w");
    fclose(file);
}

void PrintFile()
{
     char file_path[1024];
    strcpy(file_path, homeDirectory);
    strcat(file_path, "/example.txt");
    FILE *file = fopen(file_path, "r");
    int no_of_lines = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }
}

void savelog(char *input)
{
    int n = strlen(input);
    int flag = 0;
    for (int i = 0; i < n - 3; i++)
    {
        if (input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g' && (input[i + 3] == ' ' || input[i + 3] == '\n' || input[i + 3] == '\0' || input[i + 3] == '&' || input[i + 3] == ';'))
        {
            flag = 1;
        }
    }
    char file_path[1024];
    strcpy(file_path, homeDirectory);
    strcat(file_path, "/history.txt");
    FILE *file = fopen(file_path, "a+");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    if (flag == 0)
    {   
        add_to_file(input);
    }
    fclose(file);
}

void executelog(char *Command)
{
    char file_path[1024];
    strcpy(file_path, homeDirectory);
    strcat(file_path, "/history.txt");
    char Output[4096];
    removeExtraSpaces(Command, Output);
    char *command[100];
    int counter = 0;
    command[counter] = strtok(Output, " ");
    while (command[counter] != NULL)
    {
        counter++;
        command[counter] = strtok(NULL, " ");
    }
    if (counter == 1)
    {
        PrintFile();
    }
    else if (counter == 2 && strcmp(command[1], "purge") == 0)
    {
        ClearFile();
    }
    else if (counter == 3 && strcmp(command[1], "execute") == 0)
    {
        int executeNo = atoi(command[2]);
        FILE *file = fopen(file_path, "r");
        if (file == NULL)
        {
            printf("Error opening file\n");
            return;
        }
        int no_of_lines = 0;
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            no_of_lines++;
        }
        fclose(file);
        FILE* file2 = fopen(file_path, "r");
        char line[1024];
        int count = no_of_lines - executeNo + 1;
        int actual_count = 0;
        while (fgets(line, sizeof(line), file2) != NULL)
        {
            actual_count = actual_count + 1;
            if (actual_count == count)
            {
                break;
            }
        }
        printf("line %s\n", line);
        fclose(file2);
        tokenize(line, &prev_prom, prev_prom_msg);
    }
    else
    {
    }
}