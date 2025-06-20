/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Fork Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

void fork_command(client_t *client, command_t *command)
{
    (void)command;
    client->player->tick_cooldown = FORK_COMMAND_COOLDOWN;
    lay_egg(client->player, client->server->game->map);
    write(client->sockfd, "ok\n", 3);
    debug_player(client->server->options->debug,
        "Player %zu laid an egg at position (%d, %d)\n", client->player->id,
        client->player->pos.x, client->player->pos.y);
}
