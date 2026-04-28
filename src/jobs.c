/*****************************************************************************
 *  xpsh — jobs.c
 *  Job list management (linked list).
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "jobs.h"

job_t *job_list = NULL;

static const char *state_str(int state)
{
    switch (state) {
    case JOB_RUNNING: return "Running";
    case JOB_STOPPED: return "Stopped";
    case JOB_DONE:    return "Done";
    default:          return "Unknown";
    }
}

int job_next_id(void)
{
    int max_id = 0;
    for (job_t *j = job_list; j; j = j->next)
        if (j->id > max_id)
            max_id = j->id;
    return max_id + 1;
}

job_t *job_add(pid_t pgid, const char *cmdline, int state)
{
    job_t *j = malloc(sizeof(job_t));
    if (!j) {
        perror("xpsh: malloc");
        return NULL;
    }

    j->id      = job_next_id();
    j->pgid    = pgid;
    j->cmdline = strdup(cmdline);
    j->state   = state;
    j->next    = job_list;
    job_list   = j;

    return j;
}

void job_remove(int id)
{
    job_t **pp = &job_list;
    while (*pp) {
        if ((*pp)->id == id) {
            job_t *tmp = *pp;
            *pp = tmp->next;
            free(tmp->cmdline);
            free(tmp);
            return;
        }
        pp = &(*pp)->next;
    }
}

job_t *job_find_by_id(int id)
{
    for (job_t *j = job_list; j; j = j->next)
        if (j->id == id)
            return j;
    return NULL;
}

job_t *job_find_by_pgid(pid_t pgid)
{
    for (job_t *j = job_list; j; j = j->next)
        if (j->pgid == pgid)
            return j;
    return NULL;
}

void job_set_state(job_t *j, int state)
{
    if (j) j->state = state;
}

void job_print_all(void)
{
    for (job_t *j = job_list; j; j = j->next)
        printf("[%d]%c %s\t\t%s\n", j->id,
               (j == job_list) ? '+' : '-',
               state_str(j->state), j->cmdline);
}

void job_reap(void)
{
    int   status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        job_t *j = job_find_by_pgid(pid);
        if (!j) continue;

        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            j->state = JOB_DONE;
            printf("[%d]+ Done\t\t%s\n", j->id, j->cmdline);
            job_remove(j->id);
        } else if (WIFSTOPPED(status)) {
            j->state = JOB_STOPPED;
        } else if (WIFCONTINUED(status)) {
            j->state = JOB_RUNNING;
        }
    }
}

void job_free_all(void)
{
    job_t *j = job_list;
    while (j) {
        job_t *next = j->next;
        free(j->cmdline);
        free(j);
        j = next;
    }
    job_list = NULL;
}
