void helper();

void helper() {
    char *help = "Usage: patcher [OPTION]... [FILE]...\n"
                 "Parses the ELF header and prints standard stdout.\n"
                 "Patch the specific file ELF header bytes, use -c parameter.\n"
                 "\n"
                 "Parameters get hex value without 0x.\n"
                 "\n"
                 "Mandatory arguments to long options are mandatory for short options too.\n"
                 "  -c                        set change ELF header\n"
                 "  -a                        set OS/ABI\n"
                 "                             0x00 = UNIX\n"
                 "                             0x01 = HP-UX\n"
                 "                             0x02 = NetBSD\n"
                 "                             0x06 = Solaris\n"
                 "                             0x09 = FreeBSD\n"
                 "                             0x0C = OpenBSD\n"
                 "  -m                        set Machine Architecture\n"
                 "                             0x3e = Advanced Micro Devices X86-64\n"
                 "                             0x03 = Intel 80386\n"
                 "                             0x28 = ARM\n"
                 "                             0xb7 = AArch64\n"
                 "  -e                        set Endianness\n"
                 "                             1 = Little endian\n"
                 "                             2 = Big endian";

    puts(help);
    exit(EXIT_SUCCESS);
}
