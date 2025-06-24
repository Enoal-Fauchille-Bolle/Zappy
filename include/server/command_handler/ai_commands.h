/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AI Commands Header
*/

#ifndef AI_COMMANDS_H_
    #define AI_COMMANDS_H_

    #include "command_handler/command.h"
    #include "connection/client.h"

void forward_command(client_t *client, command_t *command);
void left_command(client_t *client, command_t *command);
void right_command(client_t *client, command_t *command);
void look_command(client_t *client, command_t *command);
void inventory_command(client_t *client, command_t *command);
void connect_nbr_command(client_t *client, command_t *command);
void fork_command(client_t *client, command_t *command);
void take_command(client_t *client, command_t *command);
void set_command(client_t *client, command_t *command);
void eject_command(client_t *client, command_t *command);
void incantation_command(client_t *client, command_t *command);

#endif /* !AI_COMMANDS_H_ */
