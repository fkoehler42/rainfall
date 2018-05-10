#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int		language;

int		greetuser(char *buf1) {
	char	buf3[72];

	// not working, just to explain the process
	switch (language) {
		case 1 :
			buf3 = "Hyvää päivää ";
			break;
		case 2 :
			buf3 = "Goedemiddag! ";
			break;
		default :
			buf3 = "Hello ";
	}
	strcat(buf3, buf1);
	return (puts(buf3));
}

int		main(int argc, char **argv) {
	char	*lang;
	char	buf1[40];
	char	buf2[32];

	if (argc != 3)
		return (1);

	strncpy(buf1, argv[1], 40);
	strncpy(buf2, argv[2], 32);

	lang = getenv("LANG");
	if (lang != NULL) {
		if (memcmp(lang, "fi", 2) == 0)
			language = 1;
		else if (memcmp(lang, "nl", 2) == 0)
			language = 2;
	}
	return (greetuser(buf1));
}