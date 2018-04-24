#include <stdio.h>
#include <stdlib.h>

void	o() {
	char	str[] = "/bin/sh";

	system(str);
	exit(1);
}

void	n() {
	char	buf[512];

	fgets(buf, 512, stdin);
	printf("%s", buf);
	exit(1);
}

int		main() {
	n();
}