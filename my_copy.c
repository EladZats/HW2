#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    FILE *in = stdin, *out = stdout;
    unsigned char buffer[65536];
    size_t n;
    int failed = 0;
    struct stat source, destination;
    if (argc != 3) goto error;
    if (strcmp(argv[1], "-") != 0 && !(in = fopen(argv[1], "rb"))) goto error;
    if (strcmp(argv[2], "-") != 0) {
        if (fstat(fileno(in), &source) == 0 && stat(argv[2], &destination) == 0 &&
            S_ISREG(source.st_mode) && source.st_dev == destination.st_dev && source.st_ino == destination.st_ino) {
            if (in != stdin && fclose(in) != 0) goto error;
            return 0;
        }
        out = fopen(argv[2], "wb");
        if (!out) { if (in != stdin) fclose(in); goto error; }
    }
    while ((n = fread(buffer, 1, sizeof buffer, in)) != 0)
        if (fwrite(buffer, 1, n, out) != n) { failed = 1; break; }
    if (ferror(in)) failed = 1;
    if (in != stdin && fclose(in) != 0) failed = 1;
    if (out == stdout) { if (fflush(out) != 0) failed = 1; }
    else if (fclose(out) != 0) failed = 1;
    if (failed) goto error;
    return 0;
error:
    fputs("Error occurred\n", stderr);
    return 1;
}
