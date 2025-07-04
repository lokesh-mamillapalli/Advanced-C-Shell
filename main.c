#include<stdio.h>
int prev_prom = 0;
char prev_prom_msg[255];
#include "header.h"
#define MAX_CHAR 4096

int processnumber = 0;
char pres_dir[200];
char prev_dir[200];
char homeDirectory[MAX_CHAR];

int main() {
    getcwd(homeDirectory , sizeof(homeDirectory));
    while(1){
        printing(&prev_prom, prev_prom_msg);
        char input[4096] = {'\0'};
        fgets(input, 4096, stdin);
        Clearbuffer();
        savelog(input);
        tokenize(input,&prev_prom, prev_prom_msg);
    }
}