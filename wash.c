#include <stdio.h> // Standard I/O functions
#include <stdlib.h>
#include <unistd.h> // POSIX operating system API
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>  // Character type functions

#define MAX_LINE 256
#define MAX_ARGS 100

void parse_input(char *input, char **args, int *background) {
    *background = 0;  // Initializes the background flag to 0 (false).

    // Trim leading and trailing whitespace
    while (isspace((unsigned char)*input)) input++;
    if (*input == 0) return; // All spaces

    char *end = input + strlen(input) - 1;
    while (end > input && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    // Tokenize the input string
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Check if the command should be run in the background
    if (i > 0 && strcmp(args[i-1], "&") == 0) {
        *background = 1;
        args[i-1] = NULL; // Remove '&' from args
    }
}

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        // Error forking
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process Executes the command using execvp. If execvp fails, prints an error message and exits.
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else {
        // Parent process Waits for the child process to complete using waitpid
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_LINE]; //Buffer to store user input.

    char *args[MAX_ARGS]; //Array of strings to store command arguments.

    int background; //Flag to indicate if the command should run in the background.

    while (1) { //main loop
        printf("wash> ");
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            // Handle end of input 
            printf("\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        parse_input(input, args, &background);

        // Check for built-in commands and handle them
        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;


        } else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; args[i] != NULL; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");


        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[MAX_LINE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd");
            }


        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                args[1] = getenv("HOME");
            }
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        } else if (strcmp(args[0], "setpath") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "setpath: missing argument\n");
            } else {
                char path[MAX_LINE] = "";
                for (int i = 1; args[i] != NULL; i++) {
                    strcat(path, args[i]);
                    if (args[i + 1] != NULL) {
                        strcat(path, ":");
                    }
                }
                setenv("PATH", path, 1);
            }
        } else if (strcmp(args[0], "help") == 0) {
            printf("Built-in commands:\n");
            printf("exit: Exit the shell\n");
            printf("echo [text]: Print the provided text\n");
            printf("pwd: Print the current working directory\n");
            printf("cd [dir]: Change the current directory\n");
            printf("setpath <dir> [dir ...]: Set the PATH for executable programs\n");
            printf("help: List all built-in commands\n");
        } else {
            execute_command(args);
        }

        if (!background) {
            wait(NULL);
        }
    }
    return 0;
}
