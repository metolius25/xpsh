/*****************************************************************************
 *  xpsh — main.c
 *  Entry point.
 *****************************************************************************/

#include "shell.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    xpsh_init();
    xpsh_loop();
    xpsh_cleanup();

    return 0;
}
