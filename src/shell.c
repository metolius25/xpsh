/*****************************************************************************
 *  xpsh — shell.c
 *  REPL loop, prompt, input reading, and shell initialization.
 *****************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "parser.h"
#include "exec.h"
#include "jobs.h"
#include "signals.h"

/* Global shell state */
pid_t shell_pgid;
int   shell_terminal;
int   shell_is_interactive;

void xpsh_init(void)
{
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive) {
        /* Loop until we are in the foreground */
        while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
            kill(-shell_pgid, SIGTTIN);

        /* Put ourselves in our own process group */
        shell_pgid = getpid();
        if (setpgid(shell_pgid, shell_pgid) < 0) {
            perror("xpsh: setpgid");
            exit(1);
        }

        /* Grab control of the terminal */
        tcsetpgrp(shell_terminal, shell_pgid);

        /* Install signal handlers */
        signals_init();
    }
}

char *xpsh_read_line(void)
{
    char  *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            free(line);
            return NULL; /* EOF */
        }
        perror("xpsh: getline");
        exit(EXIT_FAILURE);
    }

    /* Strip trailing newline */
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';

    return line;
}

void xpsh_loop(void)
{
    char      *line;
    command_t *cmd;
    int        status = 1;

    while (status) {
        /* Reap finished background jobs */
        job_reap();

        if (shell_is_interactive)
            printf("xpsh> ");

        line = xpsh_read_line();
        if (!line) {
            if (shell_is_interactive)
                printf("\n");
            break; /* EOF */
        }

        if (line[0] == '\0') {
            free(line);
            continue;
        }

        cmd = xpsh_parse_line(line);
        if (cmd) {
            status = xpsh_execute(cmd);
            command_free(cmd);
        }

        free(line);
    }
}

void xpsh_cleanup(void)
{
    job_free_all();
}
