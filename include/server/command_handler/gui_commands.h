/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI Commands Header
*/

#ifndef GUI_COMMANDS_H_
    #define GUI_COMMANDS_H_

    #include "command_handler/command.h"
    #include "connection/client.h"
    #include "team/player/player.h"

void msz_command(client_t *client, command_t *command); // Single Command
void ppo_command(client_t *client, command_t *command); // Single Command
void pnw_command(player_t *player); // Global Event
void bct_command(client_t *client, command_t *command); // Single Command

#endif /* !GUI_COMMANDS_H_ */
