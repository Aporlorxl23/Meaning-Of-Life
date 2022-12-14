#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "options.h"

struct elf_header {
    char elf[5];
    short abi;
    short machine;
    short endian;
} elf_header;

FILE *fp;
size_t filelen;
int *buffer;
dict_t **machine_dict, **abi_dict, **endianness_dict;

void prepare_dicts();
void destroy_dicts();
void read_elf();
void print_magic();
void check_valid_elf_file();
void set_structure();
void print_all();
void initialize();

void print_all2();
void overwrite_initialize();
void overwrite();

void prepare_dicts() {
    machine_dict = dictAlloc();
    abi_dict = dictAlloc();
    endianness_dict = dictAlloc();
    addItem(machine_dict, 0x3e, "Advanced Micro Devices X86-64");
    addItem(machine_dict, 0x03, "Intel 80386");
    addItem(machine_dict, 0x28, "ARM");
    addItem(machine_dict, 0xb7, "AArch64");

    addItem(abi_dict, 0x00, "UNIX - System V");
    addItem(abi_dict, 0x01, "HP-UX");
    addItem(abi_dict, 0x02, "NetBSD");
    addItem(abi_dict, 0x03, "Linux");
    addItem(abi_dict, 0x06, "Solaris");
    addItem(abi_dict, 0x09, "FreeBSD");
    addItem(abi_dict, 0x0C, "OpenBSD");

    addItem(endianness_dict, 1, "Little endian");
    addItem(endianness_dict, 2, "Big endian");
}

void destroy_dicts() {
    dictDealloc(machine_dict);
    dictDealloc(abi_dict);
    dictDealloc(endianness_dict);
}

void read_elf(){
    // 0x40 to filelen for read all elf data
    for(int idx = 0; idx <= filelen; ++idx) {
        fread((buffer+idx) ,1, 1, fp);
    } 
    if (!change_flag)
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
    if (change_flag)
        fclose(fp);       
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
    printf("  OS/ABI:       %s\n", (char *)getItem(*abi_dict, elf_header.abi));
    printf("  Endianness:   %s\n", (char *)getItem(*endianness_dict, elf_header.endian));
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

void print_all2() {
    printf("New ELF Header:\n");
    printf("  Output:       %s\n", output);
    printf("  Class:        ELF%d\n", elf_arch);
    printf("  Machine:      %s\n", (char *)getItem(*machine_dict, machine));
    printf("  OS/ABI:       %s\n", (char *)getItem(*abi_dict, abi));
    printf("  Endianness:   %s\n", (char *)getItem(*endianness_dict, endianness));
}

void overwrite_initialize(char *fname) {
    fp = fopen(fname, "wb");
    if (fp == NULL) {
        fprintf(stderr, "File not found!\n");
        exit(EXIT_FAILURE);
    }
}

void overwrite() {
    /*
    ELFXX buffer[0x4];
    elf_header.endian = buffer[0x5];
    elf_header.abi = buffer[0x7];
    elf_header.machine = buffer[0x12];
    */
    
    if (!endianness_flag)
        endianness = buffer[0x5];
    if (!abi_flag)
        abi = buffer[0x7];
    if (!machine_flag)
        machine = buffer[0x12];
    if (!elf_arch_flag)
        elf_arch = buffer[0x4];

    if (elf_arch == 64 || elf_arch == 2) {
        buffer[0x4] = 2;
        elf_arch = 64;
    } else if (elf_arch == 32 || elf_arch == 1) {
        buffer[0x4] = 1;
        elf_arch = 32;
    } else {
        fprintf(stderr, "[-] -E architecture paramter must be 32 or 64!\n");
        exit(EXIT_FAILURE);
    }

    if (machine) {
        buffer[0x12] = machine;
    } 

    if (abi) {
        buffer[0x7] = abi;
    }

    if (endianness) {
        buffer[0x5] = endianness;
    }

    for (int idx = 0; idx <= filelen; ++idx) {
        fputc(buffer[idx], fp);
    }
    fclose(fp);
}

int main(int argc, char **argv) { 
    
    set_arguments(argc, argv);
    prepare_dicts();
    initialize(argv[optind]);
    read_elf();
    check_valid_elf_file();
    set_structure();
    print_all();
    if (change_flag) {
        overwrite_initialize(output);
        read_elf();
        overwrite();
        print_all2();
    }
    destroy_dicts();

    return 0;
}
