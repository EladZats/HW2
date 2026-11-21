#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int GetPaths(FILE **file, char *argv[]);
int matches(const char *line, const char *pattern);


int main (int argc,char *argv[]) {
    FILE *file;
    if (GetPaths(&file, argv)==1) return 1;

    char *pattern = malloc(strlen(argv[2]) + 1);
    if (pattern == NULL) {
        return 1;
    }
    strcpy(pattern, argv[2]);
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (matches(line,pattern)){
            fprintf (stdout,"%s",line);
        }
    }
    return 0;
}

int GetPaths(FILE **file, char *argv[]){
    if (strcmp(argv[1], "-") == 0){
            *file =stdin;
        }else {
            *file = fopen(argv[1], "r");
            if (*file == NULL){
                return 1;
            }
        }
    return 0;
}


int matches(const char *line, const char *pattern){
    int j =0;
    for (int i = 0; i <= strlen(line); i++){
        if (line[i+j] != '\0' && line[i+j] == pattern[j]){
            i--;
            j++;
            if (j == strlen (pattern)){
                return 1;
            } 
        }else{
            j=0;
        }
    }
    return 0;
}