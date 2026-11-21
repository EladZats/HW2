#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char ch;
    int any;
    int star;
} Token;

int main(int argc, char **argv) {
    if (argc != 3) return 1;
    FILE *input = strcmp(argv[1], "-") == 0 ? stdin : fopen(argv[1], "r");
    if (!input) return 1;

    const char *p = argv[2];
    int anchored_start = (*p == '^');
    if (anchored_start) ++p;
    size_t plen = strlen(p);
    int anchored_end = plen > 0 && p[plen - 1] == '$';
    if (anchored_end) --plen;

    Token *tokens = malloc((plen + 1) * sizeof(*tokens));
    unsigned char *active = calloc(plen + 1, 1);
    unsigned char *next = calloc(plen + 1, 1);
    if (!tokens || !active || !next) {
        free(tokens); free(active); free(next);
        if (input != stdin) fclose(input);
        return 1;
    }
    size_t count = 0;
    for (size_t i = 0; i < plen; ++i) {
        tokens[count].ch = (unsigned char)p[i];
        tokens[count].any = (p[i] == '.');
        tokens[count].star = (i + 1 < plen && p[i + 1] == '*');
        if (tokens[count].star) ++i;
        ++count;
    }

    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;
    int error = 0;
    while ((length = getline(&line, &capacity, input)) >= 0) {
        size_t n = (size_t)length;
        if (n && line[n - 1] == '\n') --n;
        memset(active, 0, count + 1);
        int matched = 0;
        for (size_t pos = 0; pos <= n; ++pos) {
            if (!anchored_start || pos == 0) active[0] = 1;
            for (size_t j = 0; j < count; ++j)
                if (active[j] && tokens[j].star) active[j + 1] = 1;
            if (active[count] && (!anchored_end || pos == n)) {
                matched = 1;
                break;
            }
            if (pos == n) break;
            memset(next, 0, count + 1);
            for (size_t j = 0; j < count; ++j) {
                if (active[j] && (tokens[j].any || tokens[j].ch == (unsigned char)line[pos]))
                    next[tokens[j].star ? j : j + 1] = 1;
            }
            unsigned char *tmp = active; active = next; next = tmp;
        }
        if (matched && fwrite(line, 1, (size_t)length, stdout) != (size_t)length) {
            error = 1;
            break;
        }
    }
    if (ferror(input) || fflush(stdout) == EOF) error = 1;
    free(line); free(tokens); free(active); free(next);
    if (input != stdin && fclose(input) == EOF) error = 1;
    return error ? 1 : 0;
}
