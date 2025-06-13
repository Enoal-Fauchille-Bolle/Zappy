/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Structure Header
*/

#ifndef COMMAND_H_
    #define COMMAND_H_

typedef struct command_s {
    char *name;
    int argc;
    char **argv;
} command_t;

#endif /* !COMMAND_H_ */
