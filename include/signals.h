#ifndef SIGNALS_H
#define SIGNALS_H

/* Install shell signal handlers (ignore SIGINT, SIGTSTP, SIGTTOU; handle SIGCHLD) */
void signals_init(void);

/* Reset signals to default in child before execvp */
void signals_default(void);

#endif
