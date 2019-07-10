#include <stdio.h>
#include <string.h>
int main(int argc, char *args[])
{

    /**
     * 如果不给 main 传入参数,则 argc=1, args 是程序完整路径.
   */
    printf("%d\n", argc);
    for (int i = 0; i < sizeof(args) / sizeof(char *); ++i) {
        printf("%s", args[i]);
    }
}