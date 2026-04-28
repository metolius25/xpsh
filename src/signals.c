/*****************************************************************************
 *  xpsh — signals.c
 *  Signal handler installation and reset.
 *****************************************************************************/

#include <stddef.h>
#include <signal.h>
#include <sys/wait.h>

#include "signals.h"
#include "jobs.h"

static void sigchld_handler(int sig)
{
    (void)sig;
    /* Handled synchronously in xpsh_loop via job_reap() */
}

void signals_init(void)
{
    struct sigaction sa;

    /* Ignore interactive signals in the shell process */
    signal(SIGINT,  SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    /* SIGCHLD: just need to interrupt waitpid in main loop */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

void signals_default(void)
{
    signal(SIGINT,  SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
}
