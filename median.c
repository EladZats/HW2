#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_GRADE 100

void SortWithHist (int count, int* grades, int* sort_grades);
int Median (int count, int* grades);

int main (int argc, char *argv[]) {
    int capacity = 100;
    FILE *file;

    int *grades = malloc(capacity * sizeof(int));
    if (grades == NULL){
        return 1;
    }

    if (argc == 1 || strcmp(argv[1], "-") == 0){
        file =stdin;
    }else {
        file = fopen(argv[1], "r");
        if (file == NULL){
            free (grades);
            return 1;
        }
    }


    char line[100];
    int i=0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (i >= capacity){
            int *temp = realloc(grades, capacity * 2 * sizeof(int));
            if (temp == NULL) {
                free(grades);
                return 1;
            }
            grades = temp;
            capacity = capacity * 2;
        }
        char *end;
        long grade = strtol(line, &end, 10);
        if (end == line || (*end != '\n' && *end != '\0') ||
        grade > 100 || grade < 0)
        {
            fprintf (stderr,"Error at line %d: invalid input %s",i+1,line);
            return 1;
        }
    
        grades[i++] = (int)grade;
    }
    int* sort_grades = malloc(i * sizeof(int));
    if (sort_grades == NULL){
        return 1;
    }
    SortWithHist (i,  grades, sort_grades);
    fprintf (stdout,"%d",Median (i, sort_grades));
    free (grades);
    free(sort_grades);
    fclose(file);
    return 0;

}

void SortWithHist (int count, int* grades, int* sort_grades){
    int hist [101] = {0};
    for (int i=0; i<count; i++ ){
        hist[grades[i]] ++;
    }
    int j=0;
    for (int i=0; i<count;){
        if (hist[j] > 0){
            sort_grades[i++] = j;
            hist[j]--;
        }else if (hist[j] ==0) j++;
    }
}

int Median (int count, int* grades){
    int MedianCount = (count+1)/2;
    if (MedianCount == 0) return grades[0];
    return grades[MedianCount-1];
}