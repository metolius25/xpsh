/*****************************************************************************
 *  xpsh — cmd_echo.c
 *  Builtin: echo [args...]
 *****************************************************************************/

#include <stdio.h>

#include "builtins.h"

int cmd_echo(char **args)
{
    for (int i = 1; args[i]; i++) {
        printf("%s%s", args[i], args[i+1] ? " " : "");
    }
    printf("\n");
    return 1;
}
