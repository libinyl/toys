#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include <elf.h>

void fatal(char *s)
{
    printf("%s\n", s);
    exit(1);
}

int main()
{
    extern int errno;
    int fd = open("/root/kernel", O_RDONLY);
    struct stat fs;
    if (fstat(fd, &fs) < 0)
        fatal("fstat error");
    //printf("%lld",fs.st_size);
    uint8_t *mem = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mem)
        fatal("mmap error.");

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *) mem;

    Elf32_Phdr *phdr = (Elf32_Phdr * ) & mem[ehdr->e_phoff];
    Elf32_Shdr *shdr = (Elf32_Shdr * ) & mem[ehdr->e_shoff];

    int bytesize = 8;

    int wordsize = sizeof(void *) * bytesize;
    int intwidth = sizeof(int) * bytesize;

    printf("目标操作系统是 %d 位架构", wordsize);
    printf("int宽度:%d\n", intwidth);
    int a = 0;
    int b = 0;
    int c = 0;
    printf("a:%p\n", &a);
    printf("b:%p\n", &b);
    printf("c:%p\n", &c);


    printf("考察 ELF 结构体:\n");
    printf("结构地址:%p\n", (void *) mem);
    printf("结构地址:%x\n", mem);

    printf("e_ident:%s", ehdr->e_ident);

}