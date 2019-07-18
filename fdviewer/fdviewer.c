#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void print_head()
{
    const char *line = "-------------------------------------------------------------";
    printf("%s\n", line);
    printf("%-15s\t %-15s\t %-15s\n", "File Descriptor", "File Pointer", "Description");
    printf("%s\n", line);
}

int main()
{

    printf("process's file descriptor table are:\n");
    print_head();
    printf("%-15d\t %-15p\t %-15s\n", STDIN_FILENO, stdin, "Terminal Input Device");
    printf("%-15d\t %-15p\t %-15s\n", STDOUT_FILENO, stdout, "Terminal Output Device");
    printf("%-15d\t %-15p\t %-15s\n", STDERR_FILENO, stderr, "Terminal Error Device");

    printf("\n\n");

    printf("new file descriptor table are:\n");
    print_head();
    int ifd = open("/root/in.txt", O_RDONLY);
    if (ifd == -1) {
        perror("open");
        exit(-1);
    }
    int ofd = open("/root/out.txt", O_WRONLY);
    printf("%-15d\t %-15p\t %-15s\n", ifd, fdopen(ifd, "r"), "file in.txt");
    printf("%-15d\t %-15p\t %-15s\n", ofd, fdopen(ofd, "w"), "file out.txt");

    int pipefd[2];
    pipe(pipefd);
    printf("%-15d\t %-15p\t %-15s\n", pipefd[0], fdopen(pipefd[0], "r"), "pipe's read end");
    printf("%-15d\t %-15p\t %-15s\n", pipefd[1], fdopen(pipefd[1], "w"), "pipe's write end");

}