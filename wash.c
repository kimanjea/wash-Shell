#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 256

void parse_input(char *input, char **args, int *background) {
    // Implementation of input parsing
}

void execute_command(char **args) {
    // Implementation of command execution using fork and exec
}

int main() {
    char input[MAX_LINE];
    char *args[MAX_LINE/2 + 1];
    int background;

    while (1) {
        printf("wash> ");
        fgets(input, MAX_LINE, stdin);

        parse_input(input, args, &background);

        // Check for built-in commands and handle them
        if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "echo") == 0) {
            // Handle echo
        } else if (strcmp(args[0], "pwd") == 0) {
            // Handle pwd
        } else if (strcmp(args[0], "cd") == 0) {
            // Handle cd
        } else if (strcmp(args[0], "setpath") == 0) {
            // Handle setpath
        } else if (strcmp(args[0], "help") == 0) {
            // Handle help
        } else {
            execute_command(args);
        }

        if (!background) {
            wait(NULL);
        }
    }
    return 0;
}

