# bonus3

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

```gdb
gdb-peda$ pdisas main
Dump of assembler code for function main:
   0x080484f4 <+0>:     push   ebp                                                                                      
   0x080484f5 <+1>:     mov    ebp,esp                                                                                  
   0x080484f7 <+3>:     push   edi                                                                                      
   0x080484f8 <+4>:     push   ebx                                                                                      
   0x080484f9 <+5>:     and    esp,0xfffffff0                                                                           
   0x080484fc <+8>:     sub    esp,0xa0                                                                                 
   0x08048502 <+14>:    mov    edx,0x80486f0                    ; put "r" in edx                                      ⎤ 
   0x08048507 <+19>:    mov    eax,0x80486f2                    ; put "/home/user/end/.pass" in eax                   ⎥ 
   0x0804850c <+24>:    mov    DWORD PTR [esp+0x4],edx                                                                ⎥ 
   0x08048510 <+28>:    mov    DWORD PTR [esp],eax                                                                    ⎥ 
   0x08048513 <+31>:    call   0x8048410 <fopen@plt>            ; fopen("/home/user/end/.pass", "r")                  ⎦ 
   0x08048518 <+36>:    mov    DWORD PTR [esp+0x9c],eax         ; fopen return stored at esp+0x9c                       
   0x0804851f <+43>:    lea    ebx,[esp+0x18]                   ;                                                     ⎤ 
   0x08048523 <+47>:    mov    eax,0x0                          ;                                                     ⎥ 
   0x08048528 <+52>:    mov    edx,0x21                         ;                                                     ⎥ 
   0x0804852d <+57>:    mov    edi,ebx                          ;                                                     ⎥ 
   0x0804852f <+59>:    mov    ecx,edx                          ;                                                     ⎥ 
   0x08048531 <+61>:    rep stos DWORD PTR es:[edi],eax         ; memset(esp+0x18, 0, 33)                             ⎦ 
   0x08048533 <+63>:    cmp    DWORD PTR [esp+0x9c],0x0         ;                                                     ⎤ 
   0x0804853b <+71>:    je     0x8048543 <main+79>              ; if esp+0x9c == 0: jump to <main+79>                 ⎦ 
   0x0804853d <+73>:    cmp    DWORD PTR [ebp+0x8],0x2          ;                                                       
   0x08048541 <+77>:    je     0x804854d <main+89>              ; if argc == 2: jump to <main+89>                       
   0x08048543 <+79>:    mov    eax,0xffffffff                   ; eax = INT_MAX                                         
   0x08048548 <+84>:    jmp    0x8048615 <main+289>             ; jump to the end <main+289>                            
   0x0804854d <+89>:    lea    eax,[esp+0x18]                   ;                                                       
   0x08048551 <+93>:    mov    edx,DWORD PTR [esp+0x9c]         ;                                                       
   0x08048558 <+100>:   mov    DWORD PTR [esp+0xc],edx          ;                                                       
   0x0804855c <+104>:   mov    DWORD PTR [esp+0x8],0x42         ;                                                       
   0x08048564 <+112>:   mov    DWORD PTR [esp+0x4],0x1          ;                                                       
   0x0804856c <+120>:   mov    DWORD PTR [esp],eax              ;                                                       
   0x0804856f <+123>:   call   0x80483d0 <fread@plt>            ;                                                       
   0x08048574 <+128>:   mov    BYTE PTR [esp+0x59],0x0          ; add '\0' at the end of fread buffer                   
   0x08048579 <+133>:   mov    eax,DWORD PTR [ebp+0xc]          ; argv                                                  
   0x0804857c <+136>:   add    eax,0x4                          ; argv[1]                                               
   0x0804857f <+139>:   mov    eax,DWORD PTR [eax]                                                                      
   0x08048581 <+141>:   mov    DWORD PTR [esp],eax                                                                      
   0x08048584 <+144>:   call   0x8048430 <atoi@plt>             ; atoi(argv[1])                                         
   0x08048589 <+149>:   mov    BYTE PTR [esp+eax*1+0x18],0x0    ; put '\0' at buf + argv[1]                             
   0x0804858e <+154>:   lea    eax,[esp+0x18]                   ; eax = buf                                             
   0x08048592 <+158>:   lea    edx,[eax+0x42]                   ; edx = end of buf                                      
   0x08048595 <+161>:   mov    eax,DWORD PTR [esp+0x9c]         ; eax = file stream pointer                             
   0x0804859c <+168>:   mov    DWORD PTR [esp+0xc],eax          ;                                                       
   0x080485a0 <+172>:   mov    DWORD PTR [esp+0x8],0x41         ;                                                       
   0x080485a8 <+180>:   mov    DWORD PTR [esp+0x4],0x1          ;                                                       
   0x080485b0 <+188>:   mov    DWORD PTR [esp],edx              ;                                                       
   0x080485b3 <+191>:   call   0x80483d0 <fread@plt>            ; fread(buf + 0x42, 1, 65, filestream)                  
   0x080485b8 <+196>:   mov    eax,DWORD PTR [esp+0x9c]                                                                 
   0x080485bf <+203>:   mov    DWORD PTR [esp],eax                                                                      
   0x080485c2 <+206>:   call   0x80483c0 <fclose@plt>           ; fclose(filestream)                                    
   0x080485c7 <+211>:   mov    eax,DWORD PTR [ebp+0xc]                                                                  
   0x080485ca <+214>:   add    eax,0x4                          ; argv[1]                                               
   0x080485cd <+217>:   mov    eax,DWORD PTR [eax]                                                                      
   0x080485cf <+219>:   mov    DWORD PTR [esp+0x4],eax                                                                  
   0x080485d3 <+223>:   lea    eax,[esp+0x18]                                                                           
   0x080485d7 <+227>:   mov    DWORD PTR [esp],eax                                                                      
   0x080485da <+230>:   call   0x80483b0 <strcmp@plt>           ; strcmp(buf, argv[1])                                  
   0x080485df <+235>:   test   eax,eax                                                                                  
   0x080485e1 <+237>:   jne    0x8048601 <main+269>             ; if not equal: jump to <main+269>                      
   0x080485e3 <+239>:   mov    DWORD PTR [esp+0x8],0x0                                                                  
   0x080485eb <+247>:   mov    DWORD PTR [esp+0x4],0x8048707                                                            
   0x080485f3 <+255>:   mov    DWORD PTR [esp],0x804870a                                                                
   0x080485fa <+262>:   call   0x8048420 <execl@plt>            ; execl("/bin/sh" , "sh", NULL)                         
   0x080485ff <+267>:   jmp    0x8048610 <main+284>                                                                     
   0x08048601 <+269>:   lea    eax,[esp+0x18]                                                                           
   0x08048605 <+273>:   add    eax,0x42                                                                                 
   0x08048608 <+276>:   mov    DWORD PTR [esp],eax                                                                      
   0x0804860b <+279>:   call   0x80483e0 <puts@plt>             ; puts(buf + 0x42)                                      
   0x08048610 <+284>:   mov    eax,0x0                          ; return(0)                                             
   0x08048615 <+289>:   lea    esp,[ebp-0x8]                                                                            
   0x08048618 <+292>:   pop    ebx                                                                                      
   0x08048619 <+293>:   pop    edi                                                                                      
   0x0804861a <+294>:   pop    ebp                                                                                      
   0x0804861b <+295>:   ret                                                                                             
End of assembler dump.
```

This level is straightforward, after analysing the code we know that we have to focus on the `strcmp(buffer, [argv1])` instruction to make the program execute `execl("/bin/sh", "sh", NULL)`. The `fopen/memset/fread` process is absolutely useless, no investigation needed here. We look at how `argv[1]` is used along the code before the comparison, and see that it is converted to an integer thanks to `atoi`. This integer is then used as an index of the buffer to set it to '0'.

## Exploit

Since we have no idea of what the buffer contains, we try to set the '\0' to its first character (`buf[0] = '\0'`). By doing so, `argv[1]` is equal to the character '0', not the value '\0'. Obviously, in this case, `strcmp` does not return '0' and we do not fulfill the condition to call `execl`.

The solution is to send an argument which , once converted by `atoi`, will return 0; and on the same time the first character of this argument must be equal to '\0'. After some trials, we find the proper way to do this : to send an empty string.

```console
bonus3@RainFall:~$ ./bonus3 ""
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
