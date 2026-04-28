/*****************************************************************************
 *  xpsh — cmd_exit.c
 *  Builtin: exit
 *****************************************************************************/

#include <stdio.h>

#include "builtins.h"
#include "jobs.h"

int cmd_exit(char **args)
{
    (void)args;

    /* Warn about stopped jobs */
    for (job_t *j = job_list; j; j = j->next) {
        if (j->state == JOB_STOPPED) {
            fprintf(stderr, "xpsh: there are stopped jobs\n");
            return 1;
        }
    }

    return 0; /* signal REPL to quit */
}
