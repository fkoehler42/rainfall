#include <stdio.h>
#include <stdlib.h>

int         main(int argc, char **argv)
{
    if (argc > 1)
    {
        int a = atoi(argv[1]);
        printf("int:\t%d\nsize_t:\t%zu\nhex:\t%x\n", a, a, a);
    }
    return (0);
}
