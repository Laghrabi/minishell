#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    char *cmds[3][4] = {
        {"ls", NULL},            // command 1
        {"grep", ".c", NULL},    // command 2
        {"wc", "-l", NULL}       // command 3
    };

    int i, pipes[2][2];  // Only 2 pipes needed (toggle)
    pid_t pid;
    int prev_pipe = -1;

    for (i = 0; i < 3; i++)
    {
        // Create pipe if not last command
        if (i < 2)
        {
            if (pipe(pipes[i % 2]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // CHILD PROCESS

            // If not first command, set stdin to prev_pipe read end
            if (i > 0)
            {
                dup2(prev_pipe, STDIN_FILENO);
            }

            // If not last command, set stdout to current pipe write end
            if (i < 2)
            {
                dup2(pipes[i % 2][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            if (i > 0)
                close(prev_pipe);
            if (i < 2)
            {
                close(pipes[i % 2][0]);
                close(pipes[i % 2][1]);
            }

            // Execute command
            execvp(cmds[i][0], cmds[i]);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }

        // PARENT PROCESS

        // Close previous pipe's read end (no longer needed)
        if (i > 0)
            close(prev_pipe);

        // Save current pipe's read end for next command
        if (i < 2)
        {
            close(pipes[i % 2][1]);       // close write end (only child needs it)
            prev_pipe = pipes[i % 2][0];  // save read end
        }
    }

    // Wait for all children
    for (i = 0; i < 3; i++)
        wait(NULL);

    return 0;
}
