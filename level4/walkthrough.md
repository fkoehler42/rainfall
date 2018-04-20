# level4

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

As in the previous level, `main` does nothing but call a function `n`.

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
   0x08048492 <+59>:    cmp    eax,0x1025544                 ; compare eax to ???????????
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

## Exploit

Here everything concerning the exploit description.
