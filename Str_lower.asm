global _start

section .text
_start:
    push rbp
    mov rbp, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, msgl
    mov rsi, msg
    syscall
    stack:
        cmp r9, rdx
        je exit

        mov r11b, byte [msg+r9]
        add r11b, 0x20
        mov rax, 1
        mov rdi, 1
        mov rdx, msgl
        dec rdx
        push r11
        push rsp
        pop rsi
        syscall
        inc r9
        jmp stack

    exit:
        mov rax, 1
        mov rdi, 1
        mov rdx,1
        push 0xA
        push rsp
        pop rsi
        syscall
        mov rax, 0x3c
        xor edi, edi
        leave
        syscall

section .data
    msg: db "APORLOX",0xA
    msgl: equ $-msg
; aporlox
