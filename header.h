#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>
#ifndef TOKENIZE_H

#define TOKENIZE_H
extern int processnumber;
extern char homeDirectory[4096];

void printing(int *prev_prom, char *prev_prom_msg);
void lstrip(char** str_ptr);
void rstrip(char *str);
void Clearbuffer();
void removeExtraSpaces(const char *input, char *output);
void execute_simple_command(char *command);
void innerChild(char *command);
void execute_command(char *Command, int background,int *prev_prom, char *prev_prom_msg);
void tokenize(char *input,int *prev_prom, char *prev_prom_msg);
void executehop(char *Command);
void proclore(char *command);
void executereveal(char *Command);
void show(char *path, int a, int l);
void executeseek(char* Command);
int is_executable(char *file_path);
int has_directory_access(char *dir_path);
void recursive_search(char *curr_path, char *prefix, char *path_to_be_stored, int *cnt, int *type, int d, int f, int *file_type);
void searchingDirectory(char *target, char *search_path, int d, int e, int f);
void add_to_file(char *input);
void ClearFile();
void PrintFile();
void executelog(char *Command);
void savelog(char *input);

#endif
