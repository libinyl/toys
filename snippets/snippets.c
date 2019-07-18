#include <stdio.h>
#include <unistd.h>
int main()
{
    if(fork()==0)
    {
        execv("ls")
    }
}ls /proc/8301/fd/ | wc -l