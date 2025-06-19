/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Commands Function Header
*/

#ifndef COMMANDS_H_
    #define COMMANDS_H_

    #include "command_handler/command.h"
    #include "connection/client.h"
    #include <stdbool.h>

    #define FORWARD_COMMAND_COOLDOWN 7
    #define LEFT_COMMAND_COOLDOWN 7
    #define RIGHT_COMMAND_COOLDOWN 7
    #define LOOK_COMMAND_COOLDOWN 7
    #define INVENTORY_COMMAND_COOLDOWN 1
    #define BROADCAST_COMMAND_COOLDOWN 7
    #define CONNECT_NBR_COMMAND_COOLDOWN 0
    #define FORK_COMMAND_COOLDOWN 42
    #define EJECT_COMMAND_COOLDOWN 7
    #define TAKE_COMMAND_COOLDOWN 7
    #define SET_COMMAND_COOLDOWN 7
    #define INCANTATION_COMMAND_COOLDOWN 300

void dummy_command(client_t *client, command_t *command);
void forward_command(client_t *client, command_t *command);
void left_command(client_t *client, command_t *command);
void right_command(client_t *client, command_t *command);

#endif /* !COMMANDS_H_ */
