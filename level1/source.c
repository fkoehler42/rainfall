#include <stdio.h>
#include <stdlib.h>

void run(void) {
	char	str[] = "Good... Wait what?";

	fwrite((void*)str, sizeof(str), 1, 1);
	system("/bin/sh");
	return;
}

int	main(int ac, char **av) {
	char	buffer[64];

	gets(buffer);
	return (0);
}