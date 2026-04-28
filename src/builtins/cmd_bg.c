/*****************************************************************************
 *  xpsh — cmd_bg.c
 *  Builtin: bg [%N]
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "builtins.h"
#include "jobs.h"

int cmd_bg(char **args)
{
    job_t *j = NULL;

    if (args[1]) {
        int id = atoi(args[1][0] == '%' ? args[1] + 1 : args[1]);
        j = job_find_by_id(id);
    } else {
        /* Default: most recent stopped job */
        for (job_t *p = job_list; p; p = p->next) {
            if (p->state == JOB_STOPPED) {
                j = p;
                break;
            }
        }
    }

    if (!j) {
        fprintf(stderr, "xpsh: bg: no such job\n");
        return 1;
    }

    printf("[%d]+ %s &\n", j->id, j->cmdline);

    j->state = JOB_RUNNING;
    kill(-j->pgid, SIGCONT);

    return 1;
}
