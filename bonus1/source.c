#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int         main(int argc, char **argv)
{
    char    buf[40];
    int     first_arg;

    first_arg = atoi(argv[1]);
    if (first_arg > 9)
        return (1);
    memcpy(buf, argv[2], ((size_t)first_arg * 4));
    if (first_arg == 0x574f4c46)
        execl("/bin/sh", "sh", NULL);
    return (0);
}