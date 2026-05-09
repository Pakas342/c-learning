#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"

#define MAX_INPUT (256)

// Calculates the length of a NULL terminated array.
int array_length(void **array) {
    if (array == NULL) {
        return 0;
    }

    int i = 0;
    while (array[i] != NULL) {
        i++;
    }
    return i;
}

// Spawns a child process to execute the given command
// Returns their pid in the parent.
// in_num and out_num are the file descriptors to map to
// stdin and stdout
// Checkout `man dup` for a hint at changing those.
pid_t spawn_child(char **command, int in_num, int out_num) {
    pid_t pid = fork();
    if (pid < 0) {
        return 0;
    } else if (pid == 0) {
        dup2(in_num, 0);
        dup2(out_num, 1);
        execvp(command[0], command);
    }
    return pid;
}

int main() {
    // An initial input buffer.
    size_t max_input_size = MAX_INPUT;
    char *input_buffer = malloc(MAX_INPUT);
    memset(input_buffer, 0, MAX_INPUT);

    // A loop to pass through the lines and
    // process them into arrays of sequential commands
    printf("SASH> ");
    fflush(stdout);
    size_t read_bytes = getline(&input_buffer, &max_input_size, stdin);
    char ***commands_array = NULL;
    int num_commands = 0;

    pid_t *children = NULL;

    while (read_bytes != -1) {
        commands_array = deconstruct_command(input_buffer);
        num_commands = array_length((void **)commands_array);

        // Remove the '\n'
        input_buffer[read_bytes - 1] = '\0';

        // Process command. See `commands.h` for a description
        // of commands_array and deconstruct_command.

        children = malloc(sizeof(pid_t *) * num_commands);
        memset(children, 0, sizeof(pid_t *) * num_commands);
        int num_children = num_commands;

        // TODO
        char ***commands = commands_array;

        if (num_commands == 1) {
            // if there's only one child, no need for pipes.
            children[0] = spawn_child(commands[0], STDIN_FILENO, STDOUT_FILENO);
        } else {
            // Spawn children,
            // one for each command in the command array.
            // With the stdout of one command piped
            // to the stdin of the next.

            int last_input_fd = STDIN_FILENO;
            int pipefd[2];
            for (int i = 0; i < num_commands - 1; i++) {
                pipe(pipefd);

                int last_output_fd = pipefd[1]; // Just created pipe input
                children[i] =
                    spawn_child(commands[i], last_input_fd, last_output_fd);
                if (last_input_fd != STDIN_FILENO) {
                    close(last_input_fd);
                }
                close(last_output_fd);
                last_input_fd = pipefd[0]; // just created pipe output
            }
            children[num_commands - 1] = spawn_child(
                commands[num_commands - 1], last_input_fd, STDOUT_FILENO);
            if (last_input_fd != STDIN_FILENO) {
                close(last_input_fd);
            }
        }

        for (int i = 0; i < num_children; i++) {
            waitpid(children[i], NULL, 0);
        }

        // Free data structures and reset values
        free_decon_command(commands_array);
        commands_array = NULL;
        num_commands = 0;

        // TODO: Free some more things, don't want any leaks.
        free(children);

        printf("SASH> ");
        fflush(stdout);
        read_bytes = getline(&input_buffer, &max_input_size, stdin);
    }

    return 0;
}
