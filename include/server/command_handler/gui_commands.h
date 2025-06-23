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

// GUI Commands
void ppo_command(client_t *client, command_t *command);
void msz_command(client_t *client, command_t *command);
void bct_command(client_t *client, command_t *command);
void mct_command(client_t *client, command_t *command);
void plv_command(client_t *client, command_t *command);
void tna_command(client_t *client, command_t *command);
void pin_command(client_t *client, command_t *command);
void sgt_command(client_t *client, command_t *command);
void sst_command(client_t *client, command_t *command);

// GUI Events
void pnw_event(player_t *player);
void pdi_event(player_t *player);
void enw_event(egg_t *egg);
void ebo_event(egg_t *egg);
void pfk_event(player_t *player);
void pdr_event(player_t *player, resource_t resource);
void pgt_event(player_t *player, resource_t resource);
void edi_event(egg_t *egg);
void pex_event(player_t *player);

#endif /* !GUI_COMMANDS_H_ */
