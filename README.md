# RainFall

## Ressources

- 🇬🇧 [Buffer Overflow EIP Offset String Generator](http://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/)
- 🇬🇧 [Smash stack](http://insecure.org/stf/smashstack.html)
- 🇫🇷 [Introduction aux buffer overflows](https://zestedesavoir.com/articles/100/introduction-aux-buffer-overflows/)
- 🇫🇷 [Introduction à la rétroingénierie de binaires](https://zestedesavoir.com/articles/97/introduction-a-la-retroingenierie-de-binaires/)
- 🇫🇷 [Écrivez votre premier shellcode en asm x86 !](https://zestedesavoir.com/articles/158/ecrivez-votre-premier-shellcode-en-asm-x86/)

### Shell-codes

- [Shellcodes database](http://shell-storm.org/shellcode/)

## Tips

### Shellcodes

If debugging shellcode, we can use `\xcc` (int3) to stop program and return to debugger. It's a breakpoint instruction
(used by GDB).

### Python tips

You can use `struct` to convert the integer number to a binary string easily.
[Link](https://docs.python.org/2/library/struct.html).

```python
>>> import struct
>>> struct.pack("I", 0x080483f4)
'\xf4\x83\x04\x08'
```

### Setup peda

You can use [GDB peda](https://github.com/longld/peda) to enhance your GDB.

```bash
#!/bin/bash -e
FILE="utils.py skeleton.py six.py shellcode.py nasm.py config.py"

mkdir -p /tmp/peda/lib

cd /tmp/peda
curl -k -O https://raw.githubusercontent.com/longld/peda/master/peda.py
cd lib

for i in $FILE
    do curl -k -O https://raw.githubusercontent.com/longld/peda/master/lib/$i
done

cd /tmp/peda
chmod -R +rx .
cd $HOME

echo "\033[31mNE PAS OUBLIER l'ALIAS GDB :\033[0m"
echo 'alias gdb="/usr/bin/gdb -x /tmp/peda/peda.py -q "'
```
