#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

/* Job states */
#define JOB_RUNNING 0
#define JOB_STOPPED 1
#define JOB_DONE    2

typedef struct job {
    int         id;      /* Job number [1], [2], ... */
    pid_t       pgid;    /* Process group ID         */
    char       *cmdline; /* Original command string  */
    int         state;   /* RUNNING, STOPPED, DONE   */
    struct job *next;
} job_t;

/* Global job list head */
extern job_t *job_list;

job_t *job_add(pid_t pgid, const char *cmdline, int state);
void   job_remove(int id);
job_t *job_find_by_id(int id);
job_t *job_find_by_pgid(pid_t pgid);
void   job_set_state(job_t *j, int state);
void   job_print_all(void);
void   job_reap(void);
void   job_free_all(void);
int    job_next_id(void);

#endif
