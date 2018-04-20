#include <stdio.h>
#include <stdlib.h>

static char	str[] = "Wait what?!\n";
static int			to_override = 0;

void	v(void) {
	char			buf[512];


	fgets(buf, 512, stdin);
	printf("%s", buf);
	if (to_override == 64) {
		fwrite(str, 12, 1, stdout);
		system("/bin/sh");
	}
}

int		main(void) {
	v();
}