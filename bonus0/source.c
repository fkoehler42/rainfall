#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	p(char *tmp_buf, char *hyphen) {
	char	read_buf[4096];

	puts(hyphen);
	read(0, read_buf, 4096);
	*(strchr(read_buf, '\n')) = '\0';
	strncpy(tmp_buf, read_buf, 20);
}

void	pp(char *buf1) {
	char	hyphen[] = " - ";
	char	buf2[20];
	char	buf3[20];
	
	p(buf2, hyphen);
	p(buf3, hyphen);
	strcpy(buf1, buf2);
	buf1 += strlen(buf1);
	*buf1 = ' ';
	buf1++;
	strcat(buf1, buf3);
}

int		main() {
	char	buf1[42];

	pp(buf1);
	puts(buf1);
	return (0);
}