#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.c"

struct elf_header {
    char elf[5];
    short abi;
    short machine;
    short endian;
} elf_header;

FILE *fp;
size_t filelen;
int *buffer;
dict_t **machine_dict, **abi, **endianness;

void prepare_dicts() {
    machine_dict = dictAlloc();
    abi = dictAlloc();
    endianness = dictAlloc();
    addItem(machine_dict, 0x3e, "Advanced Micro Devices X86-64");
    addItem(machine_dict, 0x03, "Intel 80386");
    addItem(machine_dict, 0x28, "ARM");
    addItem(machine_dict, 0xb7, "AArch64");

    addItem(abi, 0x00, "UNIX - System V");
    addItem(abi, 0x01, "HP-UX");
    addItem(abi, 0x02, "NetBSD");
    addItem(abi, 0x03, "Linux");
    addItem(abi, 0x06, "Solaris");
    addItem(abi, 0x09, "FreeBSD");
    addItem(abi, 0x0C, "OpenBSD");

    addItem(endianness, 1, "Little endian");
    addItem(endianness, 2, "Big endian");
}

void destroy_dicts() {
    dictDealloc(machine_dict);
    dictDealloc(abi);
    dictDealloc(endianness);
}

void read_elf_header(){
    for(int idx = 0; idx < 0x40; ++idx) {
        fread((buffer+idx) ,1, 1, fp);
    }
    fclose(fp);
}

void print_magic(){
    printf("  Magic:        ");
    for (int ndx = 0; ndx < 0x10; ++ndx) {
        printf("%.2x ", buffer[ndx]);
    } putchar('\n');
}

void check_valid_elf_file() {
    for (int jdx = 1; jdx < 4; ++jdx) {
        elf_header.elf[jdx-1] = (char)buffer[jdx];
    }
    if(strcmp("ELF", elf_header.elf)) {
        fprintf(stderr, "This is not valid ELF file!\n");
        exit(EXIT_FAILURE);
    }
}

void set_structure() {
    elf_header.endian = buffer[0x5];
    elf_header.abi = buffer[0x7];
    elf_header.machine = buffer[0x12];
    
    //buffer[0x4] == 2 ? memcpy(elf_header.elf, "ELF64", sizeof(elf_header.elf)) : memcpy(elf_header.elf, "ELF32", sizeof(elf_header.elf));
    if (buffer[0x4] == 2)
        memcpy(elf_header.elf, "ELF64", sizeof(elf_header.elf));
    else
        memcpy(elf_header.elf, "ELF32", sizeof(elf_header.elf));
}

void print_all() {
    printf("ELF Header:\n");
    print_magic();
    printf("  Class:        %s\n", elf_header.elf);
    printf("  Machine:      %s\n", (char *)getItem(*machine_dict, elf_header.machine));
    printf("  OS/ABI:       %s\n", (char *)getItem(*abi, elf_header.abi));
    printf("  Endianness:   %s\n", (char *)getItem(*endianness, elf_header.endian));
}

void initialize(char *fname) {
    fp = fopen(fname, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File not found!\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_END);
    filelen = ftell(fp);
    rewind(fp);
    buffer = (int *)malloc((filelen+1)*sizeof(int)); 
    memset(buffer, '\0', filelen);
}

int main(int argc, char **argv) { 

    initialize(argv[1]);
    prepare_dicts();
    read_elf_header();
    check_valid_elf_file();
    set_structure();
    print_all();
    destroy_dicts();

    return 0;
}
