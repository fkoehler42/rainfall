#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int			main(int ac, char **av) {
	char	*sh_path;
	gid_t	egid;
	uid_t	euid;

	if (atoi(av[1]) != 423)
		fwrite("No !\n", 1, 5, stderr);
	else {
		sh_path = strdup("/bin/sh");
		egid = getegid();
		euid = geteuid();
		setresgid(egid, egid, egid);
		setresuid(euid, euid, euid);
		execv("/bin/sh", &sh_path);
	}
	return (0);
}