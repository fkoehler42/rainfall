# level0

> We use [GDB peda](https://github.com/longld/peda) in order to get a better GDB visualization.

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

We first look at the level0 executable which belongs to level1 user (setuid bit active) and we run it.

```console
level0@RainFall:~$ ./level0
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 toto
No !
```

It seems like the executable expects to receive an argument to work properly.

We use gdb to disassemble it and rewrite the source code in C language.

```gdb
gdb-peda$ disas main
Dump of assembler code for function main:
   0x08048ec0 <+0>:      push   ebp
   0x08048ec1 <+1>:      mov    ebp,esp
   0x08048ec3 <+3>:      and    esp,0xfffffff0
   0x08048ec6 <+6>:      sub    esp,0x20
   0x08048ec9 <+9>:      mov    eax,DWORD PTR [ebp+0xc]         ; argv
   0x08048ecc <+12>:     add    eax,0x4                         ; argv[1]
   0x08048ecf <+15>:     mov    eax,DWORD PTR [eax]             ; eax = argv[1]
   0x08048ed1 <+17>:     mov    DWORD PTR [esp],eax             ; push <atoi> arg to stack
   0x08048ed4 <+20>:     call   0x8049710 <atoi>                ; atoi(argv[1])
   0x08048ed9 <+25>:     cmp    eax,0x1a7                       ; compare atoi result to 0x1a7 (423 in decimal)
   0x08048ede <+30>:     jne    0x8048f58 <main+152>            ; if not equal, jump to <main+152>
   0x08048ee0 <+32>:     mov    DWORD PTR [esp],0x80c5348       ; push <strdup> arg ("/bin/sh")
   0x08048ee7 <+39>:     call   0x8050bf0 <strdup>              ; strdup("/bin/sh")
   0x08048eec <+44>:     mov    DWORD PTR [esp+0x10],eax        ; store result to stack variable (sh_path)
   0x08048ef0 <+48>:     mov    DWORD PTR [esp+0x14],0x0
   0x08048ef8 <+56>:     call   0x8054680 <getegid>             ; getegid()
   0x08048efd <+61>:     mov    DWORD PTR [esp+0x1c],eax        ; store result to stack variable (egid)
   0x08048f01 <+65>:     call   0x8054670 <geteuid>             ; geteuid()
   0x08048f06 <+70>:     mov    DWORD PTR [esp+0x18],eax        ; store result to stack variable (euid)
   0x08048f0a <+74>:     mov    eax,DWORD PTR [esp+0x1c]        ; eax = egid
   0x08048f0e <+78>:     mov    DWORD PTR [esp+0x8],eax         ; push <setresgid> 3rd arg to stack (egid)
   0x08048f12 <+82>:     mov    eax,DWORD PTR [esp+0x1c]        ; eax = egid
   0x08048f16 <+86>:     mov    DWORD PTR [esp+0x4],eax         ; push <setresgid> 2nd arg to stack (egid)
   0x08048f1a <+90>:     mov    eax,DWORD PTR [esp+0x1c]        ; eax = egid
   0x08048f1e <+94>:     mov    DWORD PTR [esp],eax             ; push <setresgid> 1st arg to stack (egid)
   0x08048f21 <+97>:     call   0x8054700 <setresgid>           ; setresgid(egid, egid, egid)
   0x08048f26 <+102>:    mov    eax,DWORD PTR [esp+0x18]        ; eax = euid
   0x08048f2a <+106>:    mov    DWORD PTR [esp+0x8],eax         ; push <setresuid> 3rd arg to stack (euid)
   0x08048f2e <+110>:    mov    eax,DWORD PTR [esp+0x18]        ; eax = euid
   0x08048f32 <+114>:    mov    DWORD PTR [esp+0x4],eax         ; push <setresuid> 2nd arg to stack (euid)
   0x08048f36 <+118>:    mov    eax,DWORD PTR [esp+0x18]        ; eax = euid
   0x08048f3a <+122>:    mov    DWORD PTR [esp],eax             ; push <setresuid> 1st arg to stack (euid)
   0x08048f3d <+125>:    call   0x8054690 <setresuid>           ; setresuid(euid, euid, euid)
   0x08048f42 <+130>:    lea    eax,[esp+0x10]                  ; eax = sh_path variable (from strdup)
   0x08048f46 <+134>:    mov    DWORD PTR [esp+0x4],eax         ; push <execv> 2nd arg (sh_path)
   0x08048f4a <+138>:    mov    DWORD PTR [esp],0x80c5348       ; push <execv> 1st arg (sh_path address)
   0x08048f51 <+145>:    call   0x8054640 <execv>               ; execv("/bin/sh", sh_path)
   0x08048f56 <+150>:    jmp    0x8048f80 <main+192>            ; jump to <main+192> (end of function)
   0x08048f58 <+152>:    mov    eax,ds:0x80ee170                ; eax = stderr
   0x08048f5d <+157>:    mov    edx,eax                         ; edx = eax
   0x08048f5f <+159>:    mov    eax,0x80c5350                   ; eax = "No !\n"
   0x08048f64 <+164>:    mov    DWORD PTR [esp+0xc],edx         ; push <fwrite> 4th arg to stack (stderr)
   0x08048f68 <+168>:    mov    DWORD PTR [esp+0x8],0x5         ; push <fwrite> 3rd arg to stack (5)
   0x08048f70 <+176>:    mov    DWORD PTR [esp+0x4],0x1         ; push <fwrite> 2nd arg to stack (1)
   0x08048f78 <+184>:    mov    DWORD PTR [esp],eax             ; push <fwrite> 1st arg to stack ("No !\n")
   0x08048f7b <+187>:    call   0x804a230 <fwrite>              ; fwrite("No !\n", 1, 5, stderr)
   0x08048f80 <+192>:    mov    eax,0x0                         ; set main return value to 0
   0x08048f85 <+197>:    leave
   0x08048f86 <+198>:    ret
End of assembler dump.
```

After analysing the source code, we easily understand that the program expects an integer as first argument and compare it to the value '423' in order to spawn a shell via `execv`.

## Exploit

All that we have to do here is to provide the good argument to the executable, then use the shell spawned with the level1 rights to display the content of the '.pass' file. 

```console
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```