# Compile

function asm64 () {
    name=$(echo $1 | cut -d '.' -f 1)
    nasm -f elf64 $name.asm -o $name.o; ld -o $name $name.o
}

function asm32 () {
    name=$(echo $1 | cut -d '.' -f 1)
    nasm -f elf32 $name.asm -o $name.o; ld -m elf_i386 -o $name $name.o
}

asm64 Hello_World.asm; ./Hello_World
