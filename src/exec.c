/*****************************************************************************
 *  xpsh — exec.c
 *  Command execution with process groups and job control.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "exec.h"
#include "builtins.h"
#include "jobs.h"
#include "signals.h"
#include "redirect.h"
#include "shell.h"

/* Reconstruct cmdline string from args for job display */
static char *args_to_string(char **args)
{
    size_t len = 0;
    for (int i = 0; args[i]; i++)
        len += strlen(args[i]) + 1;

    char *s = malloc(len + 1);
    if (!s) return NULL;
    s[0] = '\0';

    for (int i = 0; args[i]; i++) {
        if (i > 0) strcat(s, " ");
        strcat(s, args[i]);
    }
    return s;
}

int xpsh_launch(command_t *cmd)
{
    pid_t pid;
    int   status;

    pid = fork();
    if (pid == 0) {
        /* Child: new process group */
        setpgid(0, 0);

        if (!cmd->background && shell_is_interactive)
            tcsetpgrp(shell_terminal, getpid());

        /* Reset signals to default */
        signals_default();

        /* Apply redirections */
        if (apply_redirects(cmd) < 0)
            exit(EXIT_FAILURE);

        /* Exec */
        if (execvp(cmd->args[0], cmd->args) == -1)
            perror("xpsh");
        exit(EXIT_FAILURE);

    } else if (pid < 0) {
        perror("xpsh: fork");
        return 1;
    }

    /* Parent */
    setpgid(pid, pid);

    if (cmd->background) {
        char *cmdline = args_to_string(cmd->args);
        job_t *j = job_add(pid, cmdline ? cmdline : "?", JOB_RUNNING);
        printf("[%d] %d\n", j->id, pid);
        free(cmdline);
    } else {
        /* Foreground: give terminal and wait */
        if (shell_is_interactive)
            tcsetpgrp(shell_terminal, pid);

        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status) &&
                 !WIFSTOPPED(status));

        if (WIFSTOPPED(status)) {
            /* Job was stopped (Ctrl-Z) */
            char *cmdline = args_to_string(cmd->args);
            job_t *j = job_add(pid, cmdline ? cmdline : "?", JOB_STOPPED);
            printf("\n[%d]+ Stopped\t\t%s\n", j->id, j->cmdline);
            free(cmdline);
        }

        /* Take back terminal */
        if (shell_is_interactive)
            tcsetpgrp(shell_terminal, shell_pgid);
    }

    return 1;
}

int xpsh_execute(command_t *cmd)
{
    if (!cmd->args[0])
        return 1;

    /* Check builtins first */
    builtin_fn fn = builtin_lookup(cmd->args[0]);
    if (fn)
        return fn(cmd->args);

    return xpsh_launch(cmd);
}
