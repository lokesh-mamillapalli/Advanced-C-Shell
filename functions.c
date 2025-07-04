#include "header.h"
extern char homeDirectory[4096];
extern char pres_dir[200];
extern char prev_dir[200];

void removeExtraSpaces(const char *input, char *output)
{
    int i = 0, j = 0;
    int length = strlen(input);
    int space_flag = 0;

    while (i < length)
    {
        while (i < length && isspace(input[i]))
        {
            i++;
        }
        while (i < length && !isspace(input[i]))
        {
            output[j++] = input[i++];
            space_flag = 1;
        }
        if (space_flag && i < length)
        {
            output[j++] = ' ';
            space_flag = 0;
        }
    }
    if (j > 0 && (output[j - 1] == ' ' || output[j - 1] == '\n'))
    {
        j--;
    }
    output[j] = '\0';
}

void lstrip(char **str_ptr)
{
    char *str = *str_ptr;
    while (isspace((unsigned char)*str))
    {
        str++;
    }
    *str_ptr = str;
}

void rstrip(char *str)
{
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';
}

void Clearbuffer() {
    char filePath[8192];
    snprintf(filePath, sizeof(filePath), "%s/buffer.txt", homeDirectory);
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }else{
        char line[4096];
        while (fgets(line, 4095, file) != NULL) {
            printf("%s", line);
        }
        fclose(file);
        FILE *file2 = fopen(filePath, "w");
        fclose(file2);
    }
}
