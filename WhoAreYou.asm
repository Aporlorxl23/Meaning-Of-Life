section .data
	Sor db "Kendini Tanıt: ",10
	Sorl equ $-Sor
	
	Cevap db "Merhaba Can Dostum "
	Cevapl equ $-Cevap

section .bss
	Name resb 16

section .text
	global _start

_start:
	call _WhoAreYou
	call _WhoAmI
	call _YouAreXXX
	call _MeIXXX

	mov rax,60
	mov rdi,0
	syscall

_MeIXXX:
	mov rax,1
	mov rdi,1
	mov rsi,Name
	mov rdx,16
	syscall
	ret

_YouAreXXX:
	mov rax,1
	mov rdi,1
	mov rsi,Cevap
	mov rdx,Cevapl
	syscall
	ret

_WhoAreYou:
	mov rax,1
	mov rdi,1
	mov rsi,Sor
	mov rdx,Sorl
	syscall
	ret

_WhoAmI:
	mov rax,0
	mov rdi,0
	mov rsi,Name
	mov rdx,16
	syscall
	ret
; Eren Şimşek <Aporlorxl23>
