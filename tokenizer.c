#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOK_BUFFER_SIZE 64
#define TOK_DELIM " \t\r\n\a;&|<>"
char **tokenizer(char *line)
{
    int buffer_size = TOK_BUFFER_SIZE;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        if (position >= buffer_size) {
            buffer_size += TOK_BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
// strtok() splits the lines based on special chars
// stores pointer to token in the array
// reallocates