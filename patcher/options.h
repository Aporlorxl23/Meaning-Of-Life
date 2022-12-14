#include <getopt.h>
#include "helper.h"

#define OUTPUT_FILE_NAME "patcher.out"
#define DEFAULT_ARCH 64
#define DEFAULT_MACHINE 0x3e
#define DEFAULT_ABI 0
#define DEFAULT_ENDIANNESS 1

int output_flag, change_flag, machine_flag, abi_flag, endianness_flag, elf_arch_flag, err_flag, result;

int elf_arch, machine, abi, endianness; 
char *output;

/*
struct option options[] = {
    {"output", optional_argument, NULL, 1},  
    {"change", no_argument, &change_flag, 2},
    {"machine", optional_argument, NULL, 3},
    {"abi", optional_argument, NULL, 4},
    {"endianness", optional_argument, NULL, 5},
    {0, 0, 0, 0}
};
*/

void set_arguments(int argc, char *argv[]) {
    output_flag = change_flag = machine_flag = abi_flag = endianness_flag = err_flag = 0;
    opterr = 0; // disable getopt error handle, we handle manuel

    output = OUTPUT_FILE_NAME;

    //while ((result = getopt_long(argc, argv, "o:cm:a:e:", options, NULL)) != -1 ) {
    while ((result = getopt(argc, argv, "o:cm:a:e:E:h")) != -1 ) {
        if (result == 'h') {
            helper();
        }
        else if (result == 'o' /*|| result == 1*/){
            output = optarg;
            output_flag++;
        } else if (result == 'E') {
            elf_arch = atoi(optarg);
            elf_arch_flag++;
        } 
        else if (result == 'c') {
            change_flag++;
        } else if (result == 'm' /*|| result == 3*/) {
            machine = strtol(optarg, NULL, 16);
            machine_flag++;
        } else if (result == 'a' /*|| result == 4*/) {
            abi = strtol(optarg, NULL, 16);
            abi_flag++;
        } else if (result == 'e' /*|| result == 5*/) {
            endianness = strtol(optarg, NULL, 16);
            endianness_flag++;
        } else if (result == '?') {
            if (optopt != 0) {
                fprintf(stderr, "-%c unknown argument!\n", optopt);
            } /*else {
                fprintf(stderr, "--??? unknown argument!\n");
            } */err_flag = 1;
        }
    }

    if (err_flag) exit(EXIT_FAILURE);

    if (output_flag > 1 || elf_arch_flag > 1 || machine_flag > 1 || abi_flag > 1 || endianness_flag > 1) {
        fprintf(stderr, "Can't use same parameter twice!\n");
        exit(EXIT_FAILURE);
    }

    if (machine < 0 || abi < 0 || endianness < 0) {
        fprintf(stderr, "Enter number greater than 0 or 0!\n");
        exit(EXIT_FAILURE);
    } 
}
