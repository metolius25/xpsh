/*****************************************************************************
 *  xpsh — cmd_help.c
 *  Builtin: help
 *****************************************************************************/

#include <stdio.h>

#include "builtins.h"

int cmd_help(char **args)
{
    (void)args;

    printf("xpsh — a minimal POSIX shell\n");
    printf("Built-in commands:\n");

    for (int i = 0; i < num_builtins; i++)
        printf("  %-10s %s\n", builtins[i].name, builtins[i].help);

    printf("\nFeatures: job control (fg/bg), I/O redirection (<, >, >>, 2>)\n");
    return 1;
}
