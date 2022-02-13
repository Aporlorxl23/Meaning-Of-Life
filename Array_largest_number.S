.intel_syntax noprefix
.global _start

_start:
    dec r9
    largest_loop:
        cmp byte ptr [array+r9], 0x0
        je exit_largest
        cmp r10b, byte ptr [array+r9]
        jg jump
        mov r10b, byte ptr [array+r9]
        jump:
            nop
        inc r9
        jmp largest_loop

    exit_largest:
        nop

    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push r10
    push rsp
    pop rsi
    syscall

    mov rax, 0x3c
    xor edi, edi
    syscall
array:
.byte 0x61,0x62,0x63,0x64,0x65,0x66,0x78,0x68,0x69,0x70,0x71,0x72

//The largest element of the array is 0x78 = x
