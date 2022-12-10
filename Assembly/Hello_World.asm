section .data 
	Message db "Hello World",10
	Messagel equ $-Message

section .text
	global _start

_start:
	mov rax,1
	mov rdi,1
	mov rsi,Message
	mov rdx,Messagel
	syscall
	
	mov rax,60
	mov rdi,0
	syscall
; Eren Şimşek <Aporlorxl23>
