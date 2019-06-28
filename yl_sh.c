#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024


char *msh_read_line()
{
    int bufsize = BUFSIZE;
    char *buffer = malloc(sizeof(char) * bufsize);
    if (!buffer) {
        fprintf(stderr, "msh:alloction error\n");
        exit(EXIT_FAILURE);
    }

}

void msh_loop()
{

    char *line = NULL;
    char **args = NULL;
    int status = 0;

    do {
        printf("> ");
        line = msh_read_line();
        //args = msh_split_line(line);
        //status = msh_execute(args);
        free(line);
        free(args);
    }
    while (status);

}

int main()
{
    msh_loop();
    return EXIT_SUCCESS;
}