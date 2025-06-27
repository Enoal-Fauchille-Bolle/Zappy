/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Eject Command
*/

#include "command_handler/command.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include <stdbool.h>

void eject_command(client_t *client, command_t *command)
{
    (void)command;
    pex_event(client->player);
    if (!eject(client->player, client->server->game->map)) {
        send_to_client(client, "ko\n");
        return;
    }
    client->player->tick_cooldown = EJECT_COMMAND_COOLDOWN;
    send_to_client(client, "ok\n");
    debug_map(client->server->options->debug, "Player %zu ejected his tile\n",
        client->player->id);
}
