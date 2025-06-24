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
    #include "game/incantation.h"
    #include "map/coordinates.h"
    #include "map/resources.h"
    #include "map/tile.h"
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
void ppo_event(player_t *player);
void bct_event(tile_t *tile, pos_t pos, server_t *server);
void pnw_event(player_t *player);
void pdi_event(player_t *player);
void enw_event(egg_t *egg);
void ebo_event(egg_t *egg);
void pfk_event(player_t *player);
void pdr_event(player_t *player, resource_t resource);
void pgt_event(player_t *player, resource_t resource);
void edi_event(egg_t *egg);
void pex_event(player_t *player);
void plv_event(player_t *player);
void pic_event(incantation_t *incantation, server_t *server);
void pie_event(pos_t pos, bool success, server_t *server);

#endif /* !GUI_COMMANDS_H_ */
