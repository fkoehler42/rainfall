# level0

1. [Analysis](#analysis)
2. [Exploit](#exploit)

## Analysis

We first look at the level0 executable which belongs to level1 user (setuid bit active) and we run it.

```console
./level0
Segmentation fault (core dumped)
```
 
```console
./level0 toto
```

No !

We use gdb to disassemble the source code and rewrite it in C language.

After analysing the source code, we know that the program expects an integer as first argument and compare it to the value `423`.

## Exploit

```console
./level0 423
```

Then, the program sets the user and group ID to the level1 ones and executes a shell thanks to `execv` function.

We are loggued in as level1 user so we can display the content of the `.pass` file.

```console
whoami
level1
cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
