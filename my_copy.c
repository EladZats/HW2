#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PrintErr ();
int GetPaths(FILE **file_in, FILE **file_out, char *argv[]);

int main (int argc,char *argv[]) {
    FILE *file_in;
    FILE *file_out;
    if (GetPaths(&file_in, &file_out, argv)==1) return 1;
    int ch;
    while ((ch = fgetc(file_in)) != EOF) {
        
        if (fputc(ch, file_out) == EOF)
        {
            PrintErr();
            return 1;
        }
    }
    fclose(file_in);
    fclose(file_out);
    return 0;
}

void PrintErr (){
    fprintf (stderr, "Error occureed");
    return;
}

int GetPaths(FILE **file_in, FILE **file_out, char *argv[]){
    if (strcmp(argv[1], "-") == 0){
            *file_in =stdin;
        }else {
            *file_in = fopen(argv[1], "r");
            if (*file_in == NULL){
                PrintErr ();
                return 1;
            }
        }
    if (strcmp(argv[2], "-") == 0){
            *file_out =stdout;
        }else {
            *file_out = fopen(argv[2], "w");
            if (*file_out == NULL){
                PrintErr ();
                fclose(*file_in);
                return 1;
            }
        }
    return 0;
}