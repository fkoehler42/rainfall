## WIP ##

#include <string.h>
#include <stdio.h>
#include <sys/types.h>

int	main(int ac, char **av) {
	int	value = atoi(av[1]);

	if (value != 423)
		fwrite(); ## add args
	else {
		strdup(); ## add args
		geteuid(); ## add args
		getegid(); ## add args
		setresuid(); ## add args
		setresid(); ## add args
		execv("/bin/sh");
	}
	return (0);
}