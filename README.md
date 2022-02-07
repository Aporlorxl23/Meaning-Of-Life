# Compile

Add your $SHELL profile

```bash
function asm64 () {
    name=$(echo $1 | cut -d '.' -f 1)
    nasm -f elf64 $1 -o $name.o; ld -o $name $name.o
}

function asm32 () {
    name=$(echo $1 | cut -d '.' -f 1)
    nasm -f elf32 $1 -o $name.o; ld -m elf_i386 -o $name $name.o
}

function shellcode() {
    name=$(echo $1 | cut -d '.' -f 1)
    gcc -nostdlib -static -o $name $1
    objcopy --dump-section .text=$name-raw $name
 }
```

asm64 Hello_World.asm; ./Hello_World

shellcode Binsh.S; ./Binsh
