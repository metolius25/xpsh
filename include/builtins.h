#ifndef BUILTINS_H
#define BUILTINS_H

/* Builtin function signature */
typedef int (*builtin_fn)(char **args);

/* Builtin registry entry */
typedef struct {
    const char *name;
    builtin_fn  func;
    const char *help;
} builtin_entry_t;

/* Builtin commands */
int cmd_cd(char **args);
int cmd_help(char **args);
int cmd_exit(char **args);
int cmd_jobs(char **args);
int cmd_fg(char **args);
int cmd_bg(char **args);
int cmd_echo(char **args);

/* Builtin table (defined in builtins_table.c or inline) */
extern builtin_entry_t builtins[];
extern int num_builtins;

/* Lookup a builtin by name, returns NULL if not found */
builtin_fn builtin_lookup(const char *name);

#endif
