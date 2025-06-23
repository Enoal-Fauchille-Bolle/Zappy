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
    #include "map/resources.h"
    #include "team/player/player.h"

void msz_command(client_t *client, command_t *command); // Command
void ppo_command(client_t *client, command_t *command); // Command
void pnw_command(player_t *player); // Global Event
void bct_command(client_t *client, command_t *command); // Command
void mct_command(client_t *client, command_t *command); // Command
void plv_command(client_t *client, command_t *command); // Command
void tna_command(client_t *client, command_t *command); // Command
void pdi_command(player_t *player); // Global Event
void pin_command(client_t *client, command_t *command); // Command
void enw_command(egg_t *egg); // Global Event
void ebo_command(egg_t *egg); // Global Event
void pfk_command(player_t *player); // Global Event
void pdr_command(player_t *player, resource_t resource); // Global Event
void pgt_command(player_t *player, resource_t resource); // Global Event
void sgt_command(client_t *client, command_t *command); // Command

#endif /* !GUI_COMMANDS_H_ */
