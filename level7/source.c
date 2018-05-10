#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct		s_struct {
	int		priority;
	char	*str;
};

static char	buf[68];

void		m() {
	printf("%s - %d\n", buf, (int)time(NULL));
}

int			main(int ac, char **av) {
	struct s_struct	*first, *second, *third;
	FILE	*fs;

	first = malloc(sizeof(*first));
	first->priority = 1;
	first->str = malloc(8);

	second = malloc(sizeof(*second));
	second->priority = 2;
	second->str = malloc(8);

	strcpy(first->str, av[1]);
	strcpy(second->str, av[2]);

	fs = fopen("/home/user/level8/.pass", "r");
	fgets(buf, 68, fs);
	puts("~~");
	return (0);
}