#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024


char *msh_read_line()
{
    int bufsize = BUFSIZE;
    int position = 0;
    int c;
    char *buffer = malloc(sizeof(char) * bufsize);
    if (!buffer) {
        fprintf(stderr, "msh:alloction error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();
        //如果读到 EOF,就把 buffer 填为 null 值并返回
        if (c == EOF || c == '\n') {
            buffer[position] = 0;
            return buffer;
        }
        else {
            buffer[position] = c;
        }
        ++position;

        // 如果输入长度大于缓冲区,需要重新分配
        if (position >= bufsize) {
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "msh: allocation error.\n");
                exit(EXIT_FAILURE);
            }
        }

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