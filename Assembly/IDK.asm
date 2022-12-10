section .text
    global _start

_IDK:
    push rbp
    mov rbp, rsp
    mov eax, [rbp+16]
    mov ecx, 3
    mul ecx
    leave
    ret

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 8

    push 23
    call _IDK
    add rsp, 8

    mov DWORD [num], eax
    add DWORD [num+1], 0xA

    mov eax, 1
    mov edi, 1
    mov esi, num
    mov edx, 2
    syscall

    leave
    mov eax, 60
    xor edi, edi
    syscall

section .bss
    num: resd 2
