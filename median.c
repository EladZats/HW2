#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *in = stdin;
    unsigned long long counts[101] = {0}, total = 0, line_number = 0;
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    if (argc > 1 && strcmp(argv[1], "-") != 0) {
        in = fopen(argv[1], "r");
        if (!in) { perror(argv[1]); return 1; }
    }
    while ((length = getline(&line, &capacity, in)) != -1) {
        char *end;
        long value;
        ++line_number;
        errno = 0;
        value = strtol(line, &end, 10);
        if (end != line) while (*end && isspace((unsigned char)*end)) ++end;
        if (end == line || *end || errno == ERANGE || value < 0 || value > 100 ||
            memchr(line, '\0', (size_t)length) != NULL) {
            fprintf(stderr, "Error at line %llu: invalid input ", line_number);
            fwrite(line, 1, (size_t)length, stderr);
            if (length == 0 || line[length - 1] != '\n') fputc('\n', stderr);
            free(line);
            if (in != stdin) fclose(in);
            return 1;
        }
        ++counts[value];
        ++total;
    }
    free(line);
    if (ferror(in)) { if (in != stdin) fclose(in); return 1; }
    if (in != stdin && fclose(in) != 0) return 1;
    if (!total) return 1;
    unsigned long long target = total / 2 + total % 2, seen = 0;
    for (int i = 0; i <= 100; ++i) {
        seen += counts[i];
        if (seen >= target) { printf("%d\n", i); return 0; }
    }
    return 1;
}
