/*****************************************************************************
 *  xpsh — cmd_fg.c
 *  Builtin: fg [%N]
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"
#include "jobs.h"
#include "shell.h"

int cmd_fg(char **args)
{
    job_t *j = NULL;

    if (args[1]) {
        int id = atoi(args[1][0] == '%' ? args[1] + 1 : args[1]);
        j = job_find_by_id(id);
    } else {
        /* Default: most recent job */
        j = job_list;
    }

    if (!j) {
        fprintf(stderr, "xpsh: fg: no such job\n");
        return 1;
    }

    printf("%s\n", j->cmdline);

    /* Give the terminal to the job's process group */
    if (shell_is_interactive)
        tcsetpgrp(shell_terminal, j->pgid);

    /* Send SIGCONT if stopped */
    if (j->state == JOB_STOPPED)
        kill(-j->pgid, SIGCONT);

    j->state = JOB_RUNNING;

    /* Wait for the foreground job */
    int status;
    do {
        waitpid(j->pgid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status) &&
             !WIFSTOPPED(status));

    if (WIFSTOPPED(status)) {
        j->state = JOB_STOPPED;
        printf("\n[%d]+ Stopped\t\t%s\n", j->id, j->cmdline);
    } else {
        job_remove(j->id);
    }

    /* Take terminal back */
    if (shell_is_interactive)
        tcsetpgrp(shell_terminal, shell_pgid);

    return 1;
}
