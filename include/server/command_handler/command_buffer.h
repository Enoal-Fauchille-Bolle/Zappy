/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Buffer Management Header
*/

#ifndef COMMAND_BUFFER_H_
    #define COMMAND_BUFFER_H_

    #include "command_handler/command.h"
    #include "connection/client.h"

void add_command_to_buffer(client_t *client, command_t *command);
command_t *pop_command_from_buffer(client_t *client);

#endif /* !COMMAND_BUFFER_H_ */
