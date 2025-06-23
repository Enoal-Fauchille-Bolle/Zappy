/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Eject Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/gui_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

void eject_command(client_t *client, command_t *command)
{
    (void)command;
    pex_command(client->player);
    if (!eject(client->player, client->server->game->map)) {
        write(client->sockfd, "ko\n", 3);
        return;
    }
    client->player->tick_cooldown = EJECT_COMMAND_COOLDOWN;
    write(client->sockfd, "ok\n", 3);
    debug_map(client->server->options->debug, "Player %zu ejected his tile\n",
        client->player->id);
}
