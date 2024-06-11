#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINES 7
#define MAX_LINE_LENGTH 256

void print_usage() {
    printf("Usage: head_new [-h] [-n N] [file.txt]\n");
    printf("Options:\n");
    printf("  -h        Display this help message and exit\n");
    printf("  -n N      Print the first N lines (default is 7 lines)\n");
}

int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_LINES; // Default number of lines
    char *filename = NULL;
    FILE *file = stdin;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        } else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                num_lines = atoi(argv[++i]);
                if (num_lines <= 0) {
                    fprintf(stderr, "Error: Number of lines must be a positive integer.\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "Error: -n requires a number\n");
                return 1;
            }
        } else {
            filename = argv[i];
        }
    }

    // Open file if provided
    if (filename != NULL) {
        file = fopen(filename, "r");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
    }

    // Read and print lines
    char line[MAX_LINE_LENGTH];
    for (int i = 0; i < num_lines && fgets(line, sizeof(line), file); i++) {
        printf("%s", line);
    }

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}
