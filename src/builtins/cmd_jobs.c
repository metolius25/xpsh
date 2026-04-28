/*****************************************************************************
 *  xpsh — cmd_jobs.c
 *  Builtin: jobs
 *****************************************************************************/

#include "builtins.h"
#include "jobs.h"

int cmd_jobs(char **args)
{
    (void)args;
    job_print_all();
    return 1;
}
