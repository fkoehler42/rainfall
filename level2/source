#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	p(void) {
	char			buffer[64];
	unsigned int	ptr;

	ptr = __builtin_return_address(0);
	fflush(stdout);
	gets(buffer);
	if ((ptr & 0xb0000000) == 0xb0000000) {
		printf("(%p)\n", ptr);
		exit(1);
	}
	puts(buffer);
	strdup(buffer);
}

int		main(void) {
	p();
}