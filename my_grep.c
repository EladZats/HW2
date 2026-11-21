#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { unsigned char ch; int star; } Token;

/* Add every state reachable without consuming a character. */
static void closure(unsigned char *states, const Token *tokens, size_t count)
{
    for (size_t i = 0; i < count; ++i)
        if (states[i] && tokens[i].star) states[i + 1] = 1;
}

static int matches(const char *line, size_t length, const Token *tokens,
                   size_t count, int begin, int end,
                   unsigned char *current, unsigned char *next)
{
    memset(current, 0, count + 1);
    for (size_t pos = 0;; ++pos) {
        if (!begin || pos == 0) current[0] = 1;
        closure(current, tokens, count);
        if (current[count] && (!end || pos == length)) return 1;
        if (pos == length) break;
        memset(next, 0, count + 1);
        for (size_t i = 0; i < count; ++i) {
            if (current[i] && (tokens[i].ch == '.' || tokens[i].ch == (unsigned char)line[pos]))
                next[tokens[i].star ? i : i + 1] = 1;
        }
        unsigned char *tmp = current; current = next; next = tmp;
    }
    return 0;
}

int main(int argc, char **argv)
{
    FILE *in = stdin;
    char *line = NULL;
    size_t capacity = 0, count = 0;
    ssize_t length;
    int result = 0, found = 0;
    if (argc != 3) return 1;
    if (strcmp(argv[1], "-") != 0 && !(in = fopen(argv[1], "r"))) return 1;
    const char *pattern = argv[2];
    size_t plen = strlen(pattern), first = 0, last = plen;
    int begin = plen > 0 && pattern[0] == '^';
    int end = plen > (size_t)begin && pattern[plen - 1] == '$';
    if (begin) ++first;
    if (end) --last;
    Token *tokens = malloc((plen + 1) * sizeof *tokens);
    unsigned char *current = calloc(plen + 1, 1), *next = calloc(plen + 1, 1);
    if (!tokens || !current || !next) { result = 1; goto cleanup; }
    for (size_t i = first; i < last; ++i) {
        tokens[count].ch = (unsigned char)pattern[i];
        tokens[count].star = i + 1 < last && pattern[i + 1] == '*';
        if (tokens[count].star) ++i;
        ++count;
    }
    while ((length = getline(&line, &capacity, in)) != -1) {
        size_t content = (size_t)length;
        if (content && line[content - 1] == '\n') --content;
        if (matches(line, content, tokens, count, begin, end, current, next)) {
            found = 1;
            if (fwrite(line, 1, (size_t)length, stdout) != (size_t)length ||
                (content == (size_t)length && fputc('\n', stdout) == EOF)) {
                result = 1;
                break;
            }
        }
    }
    if (ferror(in) || fflush(stdout) != 0) result = 1;
cleanup:
    free(line); free(tokens); free(current); free(next);
    if (in != stdin && fclose(in) != 0) result = 1;
    return result || !found;
}
