#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct		s_str {
	char	str[64];
};

struct		s_fptr {
	void	(*fptr)();
};

void	n() {
	system("/bin/cat /home/user/level7/.pass");
}

void	m() {
	puts("Nope");
}

int		main(int ac, char **av) {
	struct s_str	*str_struct;
	struct s_fptr	*fptr_struct;
	
	str_struct = malloc(sizeof(*str_struct));
	fptr_struct = malloc(sizeof(*fptr_struct));

	fptr_struct->fptr = &m;
	strcpy(str_struct->str, av[1]);
	fptr_struct->fptr();
}