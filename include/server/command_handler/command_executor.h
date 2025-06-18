/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Executor Header
*/

#ifndef COMMAND_EXECUTOR_H_
    #define COMMAND_EXECUTOR_H_

    #include "command_handler/command.h"
    #include "connection/client.h"
    #include <stdbool.h>

typedef struct command_registry_entry_s {
    char *command_name;
    void (*handler)(client_t *client, command_t *command);
} command_registry_entry_t;

void execute_command(client_t *client, command_t *command);

#endif /* !COMMAND_EXECUTOR_H_ */
