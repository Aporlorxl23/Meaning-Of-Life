section .text
    global _start

_start:
    mov r12, 1
    cmp r12, 0
    jne else_if
    if:
        push '0'
        push rsp
        pop rsi
        mov rax, 1
        mov rdi, 1
        mov rdx, 1
        syscall
        jmp exit
    else_if:
        cmp r12, 1
        jne else_if2
        push '1'
        push rsp
        pop rsi
        mov rax, 1
        mov rdi, 1
        mov rdx, 1
        syscall
        jmp exit
    else_if2:
         cmp r12, 2
         jne else
         push '2'
         push rsp
         pop rsi
         mov rax, 1
         mov rdi, 1
         mov rdx, 1
         syscall
         jmp exit
    else:
        mov rax, 1
        mov rdi, 1
        lea rsi, [Error]
        mov rdx, 10
        syscall
        jmp exit
    exit:
        mov rax, 0x3c
        xor edi, edi
        syscall
section .data
Error: db "Undifened", 0xA
