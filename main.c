#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

int shell_cd(char **args);
int shell_help(char **args);
int shell_pwd(char **args);
int shell_echo(char **args);
int shell_exit(char **args);

char *builtin_str_list[] = {
    "cd",
    "help",
    "pwd",
    "echo",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_pwd,
    &shell_echo,
    &shell_exit
};

int num_builtins() {
    return sizeof(builtin_str_list) / sizeof(char *);
}

int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
        perror("lsh");
        }
    }
    return 1;
}

int shell_help(char **args) {
    int i;
    printf("Brendan's Shell\n");
    printf("Built-in commands:\n");
    for (i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_str_list[i]);
    }
    return 1;
}

int shell_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
    return 1;
}

int shell_echo(char **args) {
    for (int i = 1; args[i]; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

int shell_exit(char **args)
{
    return 0;
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

int execute(char **args)
{
    int i;
    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str_list[i]) == 0 ) {
            return (*builtin_func[i])(args);
        }
    }
    return launch(args);
}
// checks for builtin commands else launch

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
}

void shell_loop(void)
{
        char *line;
        char **args;
        int status;
        do {
            printf("> ");
            line = read_line();
            args = tokenizer(line);
            status = execute(args);
            free(line);
            free(args);
        } while (status);
}

int main(int argc, char **argv)
{
    // run command loop
    shell_loop();
    return EXIT_SUCCESS;
}