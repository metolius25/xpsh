#ifndef EXEC_H
#define EXEC_H

#include "parser.h"

/* Execute a parsed command (builtin check + fork/exec) */
int xpsh_execute(command_t *cmd);

/* Launch an external program with process group + job control */
int xpsh_launch(command_t *cmd);

#endif
