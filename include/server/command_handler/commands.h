/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Commands Function Header
*/

#ifndef COMMANDS_H_
    #define COMMANDS_H_

    #include "command_handler/command.h"
    #include "command_handler/command_status.h"
    #include "connection/server.h"
    #include <stdbool.h>

command_status_t dummy_command(
    server_t *server, command_t *command, int client_sockfd);

#endif /* !COMMANDS_H_ */
