#include <stdio.h>

int			main(int argc, char **argv)
{
	char	buf[66];

	FILE *fs = fopen("/home/user/end/.pass", "r");
	memset(buf, 0, 33);
	if (fs != 0 && argc == 2)
	{
		fread(buf, 1, 66, fs);
		int a = atoi(argv[1]);
		buf[a + 1] = '\0';
		fread(buf + 66, 1, 65, fs);
		fclose(fs);
		if (strcmp(buf, argv[1]) == 0)
		{
			execl("/bin/sh" , "sh", NULL);
		}
		puts(buf + 66);
	}
	else
		return (255);
	return (0);
}
