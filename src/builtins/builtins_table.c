/*****************************************************************************
 *  xpsh — builtins_table.c
 *  Builtin command registry: name → function → help string.
 *****************************************************************************/

#include <string.h>

#include "builtins.h"

builtin_entry_t builtins[] = {
    {"cd",   cmd_cd,   "Change directory"},
    {"help", cmd_help, "Show this help"},
    {"exit", cmd_exit, "Exit the shell"},
    {"jobs", cmd_jobs, "List background jobs"},
    {"fg",   cmd_fg,   "Bring job to foreground"},
    {"bg",   cmd_bg,   "Resume job in background"},
    {"echo", cmd_echo, "Print arguments"},
};

int num_builtins = sizeof(builtins) / sizeof(builtins[0]);

builtin_fn builtin_lookup(const char *name)
{
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(name, builtins[i].name) == 0)
            return builtins[i].func;
    }
    return NULL;
}
