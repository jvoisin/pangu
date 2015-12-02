#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

// killgdb.c - prevent an elf from being loaded by gdb.
// Jeffrey Crowell <crowell [at] bu [dot] edu>
//
// $ objcopy --only-keep-debug program program.debug
// $ strip program
// $ objcopy --add-gnu-debuglink=program.debug program
// $ ./killgdb program
//
// GDB can't handle debuglink sections of size 0, there's a divide by 0 error.
// We can exploit this to make gdb crash on load of elfs.

int filesize(int fd) { return (lseek(fd, 0, SEEK_END)); }

void print_section(Elf64_Shdr *shdr, char *strTab, int shNum,
                        uint8_t *data) {
  int i;
  for (i = 0; i < shNum; i++) {
    size_t k;
    if (!strcmp(".gnu_debuglink", &strTab[shdr[i].sh_name])) {
      printf("%02d: %s Offset %lx\n", i, &strTab[shdr[i].sh_name],
             shdr[i].sh_offset);
      printf("Setting size to zero.\n");
      shdr[i].sh_size = 0;
    }
  }
}

int main(int ac, char **av) {
  void *data;
  Elf64_Ehdr *elf;
  Elf64_Shdr *shdr;
  int fd;
  char *strtab;

  fd = open(av[1], O_RDWR);
  data = mmap(NULL, filesize(fd), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  elf = (Elf64_Ehdr *)data;
  shdr = (Elf64_Shdr *)(data + elf->e_shoff);
  strtab = (char *)(data + shdr[elf->e_shstrndx].sh_offset);
  print_section(shdr, strtab, elf->e_shnum, (uint8_t*)data);
  close(fd);
  return 0;
}
