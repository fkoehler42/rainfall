#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	p(char *buf, char *str) {
	puts(str);
	read(0, buf, 4096);
	strchr(buf, '\n');
	strncpy(buf, buf, 20);
}

void	pp(char *buf1) {
	char	hyphen[] = " - ";
	
	p(buf1, hyphen);
	p(buf1 + 20, hyphen);
	strcpy();
	strcat();
}

int		main() {
	char	buf1[42];

	pp(buf1);
	puts(buf1);
	return (0);
}