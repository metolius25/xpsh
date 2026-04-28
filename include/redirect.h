#ifndef REDIRECT_H
#define REDIRECT_H

#include "parser.h"

/* Apply I/O redirections described in cmd. Call in child before execvp.
   Returns 0 on success, -1 on failure. */
int apply_redirects(command_t *cmd);

#endif
