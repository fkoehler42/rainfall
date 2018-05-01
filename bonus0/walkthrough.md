# bonus0

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:     push   ebp
   0x080485a5 <+1>:     mov    ebp,esp
   0x080485a7 <+3>:     and    esp,0xfffffff0
   0x080485aa <+6>:     sub    esp,0x40                             ; add 64 bytes of stack space
   0x080485ad <+9>:     lea    eax,[esp+0x16]                       ; eax = buffer address
   0x080485b1 <+13>:    mov    DWORD PTR [esp],eax                  ; load 1st arg of <pp> (buffer)
   0x080485b4 <+16>:    call   0x804851e <pp>                       ; pp(buffer)
   0x080485b9 <+21>:    lea    eax,[esp+0x16]                       ; eax = buffer address
   0x080485bd <+25>:    mov    DWORD PTR [esp],eax                  ; load 1st arg of <puts> (buffer)
   0x080485c0 <+28>:    call   0x80483b0 <puts@plt>                 ; puts(buffer)
   0x080485c5 <+33>:    mov    eax,0x0                              ; eax = 0 (function return value)
   0x080485ca <+38>:    leave
   0x080485cb <+39>:    ret
End of assembler dump.
```

```gdb
gdb-peda$ disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>:      push   ebp
   0x0804851f <+1>:      mov    ebp,esp
   0x08048521 <+3>:      push   edi
   0x08048522 <+4>:      push   ebx
   0x08048523 <+5>:      sub    esp,0x50                            ; add 80 bytes of stack space
   0x08048526 <+8>:      mov    DWORD PTR [esp+0x4],0x80486a0       ; load 2nd arg of <p> (" - ")
   0x0804852e <+16>:     lea    eax,[ebp-0x30]                      ; eax = buffer2 (20 bytes, on stack)
   0x08048531 <+19>:     mov    DWORD PTR [esp],eax                 ; load 1st arg of <p> (buffer2)
   0x08048534 <+22>:     call   0x80484b4 <p>                       ; p(buffer2, " - ")
   0x08048539 <+27>:     mov    DWORD PTR [esp+0x4],0x80486a0       ; load 2nd arg of <p> (" - ")
   0x08048541 <+35>:     lea    eax,[ebp-0x1c]                      ; eax = buffer3 (20 bytes, on stack)
   0x08048544 <+38>:     mov    DWORD PTR [esp],eax                 ; load 1st arg of <p> (buffer3)
   0x08048547 <+41>:     call   0x80484b4 <p>                       ; p(buffer3, " - ")
   0x0804854c <+46>:     lea    eax,[ebp-0x30]                      ; eax = buffer2
   0x0804854f <+49>:     mov    DWORD PTR [esp+0x4],eax             ; load 2nd arg of <strcpy> (buffer2)
   0x08048553 <+53>:     mov    eax,DWORD PTR [ebp+0x8]             ; eax = buffer from main
   0x08048556 <+56>:     mov    DWORD PTR [esp],eax                 ; load 1st arg of <strcpy> (buffer)
   0x08048559 <+59>:     call   0x80483a0 <strcpy@plt>              ; strcpy(buffer, buffer2)
   0x0804855e <+64>:     mov    ebx,0x80486a4                       ; ebx = 0x80486a4 (' ')
   0x08048563 <+69>:     mov    eax,DWORD PTR [ebp+0x8]             ; eax = buffer (main)
   0x08048566 <+72>:     mov    DWORD PTR [ebp-0x3c],0xffffffff     ; prepare counter register (= -1)
   0x0804856d <+79>:     mov    edx,eax                             ; edx = eax (buffer)
   0x0804856f <+81>:     mov    eax,0x0                             ; eax = 0
   0x08048574 <+86>:     mov    ecx,DWORD PTR [ebp-0x3c]            ; ecx = -1
   0x08048577 <+89>:     mov    edi,edx                             ; edi = edx (buffer)
   0x08048579 <+91>:     repnz scas al,BYTE PTR es:[edi]            ; strlen(buffer)
   0x0804857b <+93>:     mov    eax,ecx                             ; eax = ecx (buffer length)
   0x0804857d <+95>:     not    eax                                 ; reverse eax value
   0x0804857f <+97>:     sub    eax,0x1                             ; sub 1 to eax -> final buffer length
   0x08048582 <+100>:    add    eax,DWORD PTR [ebp+0x8]             ; buffer += buffer_length
   0x08048585 <+103>:    movzx  edx,WORD PTR [ebx]                  ; edx = ebx (' ')
   0x08048588 <+106>:    mov    WORD PTR [eax],dx                   ; *buffer = ' '
   0x0804858b <+109>:    lea    eax,[ebp-0x1c]                      ; eax = buffer3
   0x0804858e <+112>:    mov    DWORD PTR [esp+0x4],eax             ; load 2nd arg of <strcat> (buffer3)
   0x08048592 <+116>:    mov    eax,DWORD PTR [ebp+0x8]             ; eax = buffer
   0x08048595 <+119>:    mov    DWORD PTR [esp],eax                 ; load 1st arg of <strcat> (buffer)
   0x08048598 <+122>:    call   0x8048390 <strcat@plt>              ; strcat(buffer, buffer3)
   0x0804859d <+127>:    add    esp,0x50                            ; restore stack ptr to its previous value
   0x080485a0 <+130>:    pop    ebx
   0x080485a1 <+131>:    pop    edi
   0x080485a2 <+132>:    pop    ebp
   0x080485a3 <+133>:    ret
End of assembler dump.
```

Interesting functions here : `strcpy` which is vulnerable to buffer overflow attack, and `strcat` which is vulnerable too, especially if the first parameter (char *s1) does not have enough space to hold the second parameter (char *s2) appended to it.

```gdb
gdb-peda$ disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:      push   ebp
   0x080484b5 <+1>:      mov    ebp,esp
   0x080484b7 <+3>:      sub    esp,0x1018                          ; 4120 bytes of stack space
   0x080484bd <+9>:      mov    eax,DWORD PTR [ebp+0xc]             ; eax = " - "
   0x080484c0 <+12>:     mov    DWORD PTR [esp],eax                 ; load <puts> arg
   0x080484c3 <+15>:     call   0x80483b0 <puts@plt>                ; puts(" - ")
   0x080484c8 <+20>:     mov    DWORD PTR [esp+0x8],0x1000          ; load read 3rd arg (4096)
   0x080484d0 <+28>:     lea    eax,[ebp-0x1008]                    ; eax = read_buffer (declared on stack)
   0x080484d6 <+34>:     mov    DWORD PTR [esp+0x4],eax             ; load 2nd arg of <read> (read_buffer)
   0x080484da <+38>:     mov    DWORD PTR [esp],0x0                 ; load 1st arg of <read> (0 aka stdin)
   0x080484e1 <+45>:     call   0x8048380 <read@plt>                ; read(0, read_buffer, 4096)
   0x080484e6 <+50>:     mov    DWORD PTR [esp+0x4],0xa             ; load 2nd arg of <strchr> ('\n')
   0x080484ee <+58>:     lea    eax,[ebp-0x1008]                    ; eax = read_buffer
   0x080484f4 <+64>:     mov    DWORD PTR [esp],eax                 ; load <strchr> 1st arg (read_buffer)
   0x080484f7 <+67>:     call   0x80483d0 <strchr@plt>              ; strchr(read_buffer, '\n')
   0x080484fc <+72>:     mov    BYTE PTR [eax],0x0                  ; content of strchr returned ptr = '\0'
   0x080484ff <+75>:     lea    eax,[ebp-0x1008]                    ; eax = read_buffer
   0x08048505 <+81>:     mov    DWORD PTR [esp+0x8],0x14            ; load 3rd arg of <strncpy> (20) 
   0x0804850d <+89>:     mov    DWORD PTR [esp+0x4],eax             ; load 2nd arg of <strncpy> (read_buffer)
   0x08048511 <+93>:     mov    eax,DWORD PTR [ebp+0x8]             ; eax = buffer from <pp> function
   0x08048514 <+96>:     mov    DWORD PTR [esp],eax                 ; load 1st arg of <strncpy> (buffer)
   0x08048517 <+99>:     call   0x80483f0 <strncpy@plt>             ; strncpy(buffer, read_buffer, 20)
   0x0804851c <+104>:    leave
   0x0804851d <+105>:    ret
End of assembler dump.
```

Notice here the call to a 4096 bytes long `read`, followed by the usage of the first 20th characters read only. `strncpy` seems to protect the copy but we should probably investigate on this part of the code too.

We first take some time to understand what does this program do instruction by instruction, what do the addresses contain, etc..Thanks to this deep analysis we now have a good overview of the different pieces of code on which we have to look for a vulnerability.

Since the program waits twice for an user input and stores the result in two buffers of 20 bytes, we try to send 20+ characters on both `read` calls and a crash occurs. From here, we try to understand why !

To be continued.... ;-)

## Exploit

print 4095 char + '\n' + 9 char + address of shellcode + 7 char (sry I forgot to write the cmd here :-/)
