#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLEN 1024


int GetPaths(FILE **file, char *argv[]);
int matches(const char *line, const char *pattern);
int check_normal_match (const char *line, const char *pattern, int patternlen);
int match_here(const char *line, const char *pattern);

int main (int argc,char *argv[]) {
    FILE *file;
    if (GetPaths(&file, argv)==1) return 1;

    char *pattern = malloc(strlen(argv[2]) + 1);
    if (pattern == NULL) {
        return 1;
    }
    strcpy(pattern, argv[2]);
    char line[MAXLEN];

    int found_match = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (matches(line,pattern)){
            found_match = 1;
            fprintf (stdout,"%s",line);
            if (line[strlen(line) - 1] != '\n') {
              fprintf(stdout, "\n");
            }
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
                fprintf(stderr, "Error occurred\n");
                return 1;
            }
        }
    return 0;
}


int matches(const char *line, const char *pattern){
    int patternlen = strlen (pattern);
        if (pattern[0] =='^'){
            return match_here(line, pattern + 1);
        }else{
            return check_normal_match (line,pattern,patternlen);
        }
    return 0;
}

int check_normal_match (const char *line, const char *pattern, int patternlen){
    for (int i = 0; i <= strlen(line); i++) {
        if (match_here(line + i, pattern)) {
            return 1;
        }
    }
    return 0;
}

int match_here(const char *line, const char *pattern){
    if (*pattern == '\0') {
    return 1;
    }

    if (*pattern == '$' && pattern[1] == '\0') {
        return *line == '\n' || *line == '\0';
    }

    if (pattern[1] == '*') {
        if (match_here(line, pattern + 2)) {
            return 1;
        }
        if (*line != '\0' && *line != '\n' &&
    (*line == *pattern || *pattern == '.')) {
            return match_here(line + 1, pattern);
        }

        return 0;
    }

    if (*line != '\0' && *line != '\n' &&
        (*line == *pattern || *pattern == '.')) {
        return match_here(line + 1, pattern + 1);
    }

    return 0;
}