# RainFall

## Ressources

- [Buffer Overflow EIP Offset String Generator](http://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/)
- [Introduction aux buffer overflows](https://zestedesavoir.com/articles/100/introduction-aux-buffer-overflows/)
- [Introduction à la rétroingénierie de binaires](https://zestedesavoir.com/articles/97/introduction-a-la-retroingenierie-de-binaires/)
- [Smash stack](http://insecure.org/stf/smashstack.html)

## Installer peda

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


