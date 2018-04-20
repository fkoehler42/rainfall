#include <stdio.h>
#include <stdlib.h>

static int	to_override = 0;

void	p(char *str) {
	printf("%s", str);
}

void	n() {
	char	buf[512];

	fgets(buf, 512, stdin);
	p(buf);
	if (to_override == ???) {
		system("/bin/cat /home/user/level5/.pass");
	}
}

int		main() {
	n();
}