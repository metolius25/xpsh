#ifndef PARSER_H
#define PARSER_H

/* Parsed command representation */
typedef struct {
    char **args;       /* NULL-terminated argument array */
    char  *infile;     /* stdin redirect  (<)            */
    char  *outfile;    /* stdout redirect (> or >>)      */
    char  *errfile;    /* stderr redirect (2>)           */
    int    append;     /* 1 if >>                        */
    int    background; /* 1 if &                         */
} command_t;

command_t *xpsh_parse_line(char *line);
void       command_free(command_t *cmd);

#endif
