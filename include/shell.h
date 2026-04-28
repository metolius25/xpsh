#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

/* Shell state */
extern pid_t shell_pgid;
extern int   shell_terminal;
extern int   shell_is_interactive;

/* Core functions */
void xpsh_init(void);
void xpsh_loop(void);
void xpsh_cleanup(void);

/* Input */
char *xpsh_read_line(void);

#endif
