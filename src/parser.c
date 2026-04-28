/*****************************************************************************
 *  xpsh — parser.c
 *  Tokenizer with I/O redirection and background (&) parsing.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define TOK_BUFSIZE 64
#define TOK_DELIM   " \t\r\n\a"

command_t *xpsh_parse_line(char *line)
{
    int    bufsize = TOK_BUFSIZE;
    int    pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char  *tok;
    command_t *cmd;

    if (!tokens) {
        fprintf(stderr, "xpsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    cmd = calloc(1, sizeof(command_t));
    if (!cmd) {
        fprintf(stderr, "xpsh: allocation error\n");
        free(tokens);
        exit(EXIT_FAILURE);
    }

    tok = strtok(line, TOK_DELIM);
    while (tok) {
        /* I/O redirect: input */
        if (strcmp(tok, "<") == 0) {
            tok = strtok(NULL, TOK_DELIM);
            if (tok) cmd->infile = strdup(tok);
            tok = strtok(NULL, TOK_DELIM);
            continue;
        }
        /* I/O redirect: stderr */
        if (strcmp(tok, "2>") == 0) {
            tok = strtok(NULL, TOK_DELIM);
            if (tok) cmd->errfile = strdup(tok);
            tok = strtok(NULL, TOK_DELIM);
            continue;
        }
        /* I/O redirect: append stdout */
        if (strcmp(tok, ">>") == 0) {
            tok = strtok(NULL, TOK_DELIM);
            if (tok) {
                cmd->outfile = strdup(tok);
                cmd->append = 1;
            }
            tok = strtok(NULL, TOK_DELIM);
            continue;
        }
        /* I/O redirect: stdout */
        if (strcmp(tok, ">") == 0) {
            tok = strtok(NULL, TOK_DELIM);
            if (tok) {
                cmd->outfile = strdup(tok);
                cmd->append = 0;
            }
            tok = strtok(NULL, TOK_DELIM);
            continue;
        }
        /* Background */
        if (strcmp(tok, "&") == 0) {
            cmd->background = 1;
            tok = strtok(NULL, TOK_DELIM);
            continue;
        }

        /* Regular argument */
        tokens[pos++] = strdup(tok);

        if (pos >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "xpsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        tok = strtok(NULL, TOK_DELIM);
    }

    tokens[pos] = NULL;

    if (pos == 0) {
        free(tokens);
        command_free(cmd);
        return NULL;
    }

    cmd->args = tokens;
    return cmd;
}

void command_free(command_t *cmd)
{
    if (!cmd) return;

    if (cmd->args) {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd->errfile);
    free(cmd);
}
