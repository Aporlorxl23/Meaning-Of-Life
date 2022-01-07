section .text
    global _start

_IDK:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov ecx, 3
    mul ecx
    leave
    ret

_start:
    push ebp
    mov ebp, esp
    sub esp, 4

    push 23
    call _IDK
    add esp, 4

    mov DWORD [num], eax
    add DWORD [num+1], 0xA

    mov eax, 4
    mov ebx, 1
    mov ecx, num
    mov edx, 2
    int 0x80

    leave
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .bss
    num: resd 2
