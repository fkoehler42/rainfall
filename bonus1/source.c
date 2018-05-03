#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int         main(int argc, char **argv)
{
    int     first_arg;
    char    buf[40];

    first_arg = atoi(argv[1]);
    if (first_arg > 9)
        return (1);
    memcpy(buf, argv[2], (first_arg * 4));
    if (first_arg == 0x574f4c46)
        execl("/bin/sh");
    return (0);
}