section .text
    global _start

_start:
    push 10
    push '0'
    pop rax
    pop r12

    loop:
        cmp r12, 0
        je exit
        push rax     ; <---|
        push rsp     ;     |
        pop r11      ;     |
        mov rax, 1   ;     |
        mov rdi, 1   ;     |
        mov rsi, r11 ;     |
        mov dl, 1    ;     |
        syscall      ;     |
        pop rax      ; <---|
        sub rax, '0'
        inc rax
        add rax, '0'
        dec r12
        jmp loop

    exit:
        mov rax, 0x3c
        xor edi, edi
        syscall
