#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // load config files

    // run command loop
    void lsh_loop(void)
    {
        char *line;
        char **args;
        int status;
        do {
            printf("> ");
            line = read_line();
            args = tokenizer(line);
            status = lsh_execute(args);

            free(line);
            free(args);
        } while (status);

    }
    // Perform any actions to shutdown and clean memory

    // Return

}
// Initialize: Read and executve config files
// Interpret: Reads commands from stdin and executes
// Terminate: After commands execute, shuts down, frees up memory, terminates

char *read_line(void)
{
    char *line = NULL;
    ssize_t buffer_size = 0;

    if (getline(&line, &buffer_size, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    // start with a block of space, and realloc if more space is used
}

int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
// fork creates a child process that is a copy of the current
// 0 = child process
    // child process is the command given through args
// > 0 = parent
    // waits for chld process to finish
// < 0 = failed