/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Executor Header
*/

#ifndef COMMAND_EXECUTOR_H_
    #define COMMAND_EXECUTOR_H_

    #include "command_handler/command.h"
    #include "command_handler/command_status.h"
    #include "connection/server.h"
    #include <stdbool.h>

typedef struct command_registry_entry_s {
    char *command_name;
    command_status_t (*handler)(server_t *server, command_t *command,
        int client_sockfd);
} command_registry_entry_t;

command_status_t execute_command(
    server_t *server, command_t *command, int client_sockfd);

#endif /* !COMMAND_EXECUTOR_H_ */
