#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int GetPaths(FILE **file, char *argv[]);
int matches(const char *line, const char *pattern);
int check_normal_match (const char *line, const char *pattern, int patternlen);

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
    int patternlen = strlen (pattern);
        if (pattern[0] =='^'){
            if (patternlen == 1) {
                return 1;
            }
            for (int j = 0; j<= patternlen;){
                if (pattern [j+1] == '$' && (line[j] == '\0' ||
                     line[j] == '\n'))
                {
                    return 1;
                }
                if (line[j] != '\0' && (line[j] == pattern[j+1]|| 
                    (line[j] != '\n' && pattern[j+1] == '.') )){
                    j++;
                if (j == patternlen - 1){
                    return 1;
                } 
                }else{
                    return 0;
                }
            }
        }else{
            return check_normal_match (line,pattern,patternlen);
        }
    return 0;
}

int check_normal_match (const char *line, const char *pattern, int patternlen){
    int j =0;
    for (int i = 0; i <= strlen(line); i++){
        if (pattern [j] == '$' && (line[i+j] == '\0' || line[i+j] == '\n'))
        {
            return 1;
        }
        if (line[i+j] != '\0' && (line[i+j] == pattern[j]|| 
            (line[i+j] != '\n' && pattern[j] == '.') )){
            i--;
            j++;
            if (j == patternlen){
                return 1;
            } 
        }else{
            j=0;
        }
    }
    return 0;
}