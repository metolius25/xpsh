/*****************************************************************************
 *  xpsh — redirect.c
 *  Apply I/O redirections via dup2.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "redirect.h"

int apply_redirects(command_t *cmd)
{
    int fd;

    /* stdin redirect: < file */
    if (cmd->infile) {
        fd = open(cmd->infile, O_RDONLY);
        if (fd < 0) {
            perror(cmd->infile);
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    /* stdout redirect: > file or >> file */
    if (cmd->outfile) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append ? O_APPEND : O_TRUNC;
        fd = open(cmd->outfile, flags, 0644);
        if (fd < 0) {
            perror(cmd->outfile);
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    /* stderr redirect: 2> file */
    if (cmd->errfile) {
        fd = open(cmd->errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror(cmd->errfile);
            return -1;
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    return 0;
}
