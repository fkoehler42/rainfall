#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct	s_struct {
	char	*auth;
	char	*service;
};

struct	s_struct	*credentials;

int		main(int ac, char **av) {
	char	buf[128];

	while (1) {
		printf("%p, %p\n", credentials->auth, credentials->service);
		fgets(buf, 128, stdin);
		
	}
}