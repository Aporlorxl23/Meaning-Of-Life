#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "options.h"

#define ELF_HEADER_BYTE 0x4
#define ELF_HEADER_ENDIAN 0x5
#define ELF_HEADER_ABI 0x7
#define ELF_HEADER_MACHINE 0x12

#define ELF_PIE 0x10

struct elf_header {
    char elf[5];
    short abi;
    short machine;
    short endian;

    short pie;
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
void print_pie();

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
    for (int jdx = 1; jdx < ELF_HEADER_BYTE; ++jdx) {
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
    elf_header.endian = buffer[ELF_HEADER_ENDIAN];
    elf_header.abi = buffer[ELF_HEADER_ABI];
    elf_header.machine = buffer[ELF_HEADER_MACHINE];
    elf_header.pie = buffer[ELF_PIE];

    //buffer[0x4] == 2 ? memcpy(elf_header.elf, "ELF64", sizeof(elf_header.elf)) : memcpy(elf_header.elf, "ELF32", sizeof(elf_header.elf));
    if (buffer[ELF_HEADER_BYTE] == 2)
        memcpy(elf_header.elf, "ELF64", sizeof(elf_header.elf));
    else
        memcpy(elf_header.elf, "ELF32", sizeof(elf_header.elf));
}

void print_pie() {
    if (elf_header.pie == 0x3)
        printf("  PIE:          Enabled\n");
    else
        printf("  PIE:          Disabled\n");
}

void print_all() {
    printf("ELF Header:\n");
    print_magic();
    printf("  Class:        %s\n", elf_header.elf);
    printf("  Machine:      %s\n", (char *)getItem(*machine_dict, elf_header.machine));
    printf("  OS/ABI:       %s\n", (char *)getItem(*abi_dict, elf_header.abi));
    printf("  Endianness:   %s\n", (char *)getItem(*endianness_dict, elf_header.endian));
    print_pie();
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
    print_magic();
    printf("  Output:       %s\n", output);
    printf("  Class:        ELF%d\n", elf_arch);
    printf("  Machine:      %s\n", (char *)getItem(*machine_dict, machine));
    printf("  OS/ABI:       %s\n", (char *)getItem(*abi_dict, abi));
    printf("  Endianness:   %s\n", (char *)getItem(*endianness_dict, endianness));
    print_pie();
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
        endianness = buffer[ELF_HEADER_ENDIAN];
    if (!abi_flag)
        abi = buffer[ELF_HEADER_ABI];
    if (!machine_flag)
        machine = buffer[ELF_HEADER_MACHINE];
    if (!elf_arch_flag)
        elf_arch = buffer[ELF_HEADER_BYTE];

    if (elf_arch == 64 || elf_arch == 2) {
        buffer[ELF_HEADER_BYTE] = 2;
        elf_arch = 64;
    } else if (elf_arch == 32 || elf_arch == 1) {
        buffer[ELF_HEADER_BYTE] = 1;
        elf_arch = 32;
    } else {
        fprintf(stderr, "[-] -E architecture paramter must be 32 or 64!\n");
        exit(EXIT_FAILURE);
    }

    if (machine) {
        buffer[ELF_HEADER_MACHINE] = machine;
    }

    if (abi) {
        buffer[ELF_HEADER_ABI] = abi;
    }

    if (endianness) {
        buffer[ELF_HEADER_ENDIAN] = endianness;
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
    /*
    for (int ndx = 0; ndx < 0x100; ++ndx) {
        printf("%.2x ", buffer[ndx]);
    } putchar('\n');
    */
    destroy_dicts();

    return 0;
}
