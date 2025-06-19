/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Forward Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "command_handler/ai_commands.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

void forward_command(client_t *client, command_t *command)
{
    (void)command;
    move_player_forward(client->player, client->server->game->map);
    client->player->tick_cooldown = FORWARD_COMMAND_COOLDOWN;
    write(client->sockfd, "ok\n", 3);
    debug_player(client->server->options->debug, "Player %zu moved forward\n",
        client->player->id);
    debug_map(client->server->options->debug,
        "Player %zu position: (%d, %d)\n", client->player->id,
        client->player->pos.x, client->player->pos.y);
}
