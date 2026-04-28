/*****************************************************************************
 *  xpsh — cmd_cd.c
 *  Builtin: cd [dir]
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"

int cmd_cd(char **args)
{
    const char *dir = args[1];

    if (!dir) {
        dir = getenv("HOME");
        if (!dir) {
            fprintf(stderr, "xpsh: cd: HOME not set\n");
            return 1;
        }
    }

    if (chdir(dir) != 0)
        perror("xpsh: cd");

    return 1;
}
