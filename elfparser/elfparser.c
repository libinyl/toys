#ifndef __linux__
#error not a linux os, abort.
#endif
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
#include <string.h>

// elf 文件格式图示: https://en.wikipedia.org/wiki/File:Elf-layout--en.svg
int main(int argc, char *args[])
{
    if (argc != 2)
        printf("usage: %s <elf-file>", args[0]);

    //args[1] = "/bin/ls";
    int fd = open(args[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }


    struct stat fs;
    if (fstat(fd, &fs) < 0) {
        perror("fstat");
        exit(-1);
    }

    /**
     *
     * mmap: 用于将一个文件从磁盘映射到内存中.与普通 IO 的区别:
     * 普通 IO: 经过 read/write 的系统调用,直接对磁盘读写
     * mmap: 将文件的磁盘映射到内存中的虚拟地址,对内存读写.内存
     * 读写引起脏页回写(page cache),保证数据更新到磁盘.
     *
     * [知乎 简解](https://www.zhihu.com/question/265246208/answer/291288299)
     * [博客园 详解](https://www.cnblogs.com/huxiao-tee/p/4660352.html)
     * [UNIX 网络编程 卷二 第 12 章]
     * 参数
     * 返回: 被映射区的指针
     *
     *
     * 动作: 把文件 fd,从 offset 个字节开始,映射到内存地址空间从 addr 开始的位置.
     * 必需: 需要告知文件的大小为 len,以及要求的访问保护选项 prot,和其他选项 flags.
     *
     * void * mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
     *
     * addr     :   指定文件描述符fd 应该映射到进程空间的起始地址,通常指定为 0 让内核自己选择
     * len      :   映射到进程地址空间的字节数
     * offset   :   文件开始的偏移量,通常设为 0
     * 返回值:   fd 映射到进程地址空间的起始地址
     *
     */
    uint8_t *mem = mmap(0, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mem) {
        perror("mmap error");
        exit(-1);
    }

    /**
     * 如何判断目标操作系统是 64-bit 还是 32-bit?
     *
     * if (sizeof(void *) == 4) => 32-bit
     * if (sizeof(void *) == 8) => 64-bit
     *
     */

    // printf("mem[0]0x%x\n",mem[0]);  should be 0x7f

    if (mem[0] != 0x7f) {
        printf("not a valid elf file.");
        exit(1);
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) mem;                  // elf header
    Elf64_Phdr *phdr = (Elf64_Phdr *) &mem[ehdr->e_phoff];  // program header
    Elf64_Shdr *shdr = (Elf64_Shdr *) &mem[ehdr->e_shoff];  // section header



    /* ugly code*/
    printf("e_ident\t:\t%s\n", ehdr->e_ident);
    printf("e_type\t:\t%hd\n", ehdr->e_type);
    printf("e_machine\t:\t%hd\n", ehdr->e_machine);
    printf("e_version\t:\t%u\n", ehdr->e_version);
    printf("e_entry\t:\t0x%lx\n", ehdr->e_entry);
    printf("e_phoff\t:\t0x%lx\n", ehdr->e_phoff);
    printf("e_shoff\t:\t0x%lx\n", ehdr->e_shoff);
    printf("e_flags\t:\t%u\n", ehdr->e_flags);
    printf("e_ehsize\t:\t%hd\n", ehdr->e_ehsize);
    printf("e_phentsize\t:\t%hd\n", ehdr->e_phentsize);
    printf("e_phnum\t:\t%hd\n", ehdr->e_phnum);
    printf("e_shentsize\t:\t%hd\n", ehdr->e_shentsize);
    printf("e_shnum\t:\t%hd\n", ehdr->e_shnum);
    printf("e_shstrndx\t:\t%hd\n", ehdr->e_shstrndx);




    /* Legal values for e_type (object file type).

     * #define ET_NONE		0		// No file type
     * #define ET_REL		1		// Relocatable file
     * #define ET_EXEC		2		// Executable file
     * #define ET_DYN		3		// Shared object file
     * #define ET_CORE		4		// Core file
     * #define	ET_NUM		5		// Number of defined types
     * #define ET_LOOS		0xfe00		// OS-specific range start
     * #define ET_HIOS		0xfeff		// OS-specific range end
     * #define ET_LOPROC	0xff00		// Processor-specific range start
     * #define ET_HIPROC	0xffff		// Processor-specific range end
    */


    /**
     * 注意,一般来说,静态链接得到可执行文件,动态链接得到共吸纳过文件
     */
    if (ehdr->e_type != ET_EXEC) {
        fprintf(stderr, "warn: %s is not executable type.\n", args[1]);
    }

    printf("program entry point:0x%lx\n", ehdr->e_entry);

    //e_shstrndx: /* Section header string table index */
    unsigned char *string_table = &mem[shdr[ehdr->e_shstrndx].sh_offset];

    printf("section header list:\n");
    for (int i = 1; i < ehdr->e_shnum; ++i) {
        printf("%s: 0x%lx\n", &string_table[shdr[i].sh_name], shdr[i].sh_addr);
    }

    printf("program header list:\n");

    char *interp;
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        switch (phdr->p_type) {
        case PT_LOAD:
            if (phdr[i].p_offset == 0)
                printf("Text segment:0x%lx\n", phdr[i].p_vaddr);
            else
                printf("Data segment:0x%lx\n", phdr[i].p_vaddr);
            break;

        case PT_INTERP:interp = strdup((char *) &mem[phdr[i].p_offset]);
            printf("Interpreter: %s\n", interp);
            break;
        case PT_NOTE:printf("Note segment: 0x%lx\n", phdr[i].p_vaddr);
            break;
        case PT_DYNAMIC:printf("Dynamic segment: 0x%lx\n", phdr[i].p_vaddr);
            break;
        case PT_PHDR:printf("Phdr segment: 0x%lx\n", phdr[i].p_vaddr);
            break;
        }

    }
    exit(0);
}