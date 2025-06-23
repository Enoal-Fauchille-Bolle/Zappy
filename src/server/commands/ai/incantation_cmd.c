/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/incantation.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

void incantation_command(client_t *client, command_t *command)
{
    (void)command;
    if (!check_incantation_requirements(client->server->game->map,
            client->player->pos, client->player->level)) {
        write(client->sockfd, "ko\n", 3);
        return;
    }
    client->player->tick_cooldown = INCANTATION_COMMAND_COOLDOWN;
    create_incantation(
        client->server->game, client->player->pos, client->player->level);
    debug_game(client->server->options->debug,
        "Player %zu started an "
        "incantation at (%d, %d) for level %u\n",
        client->player->id, client->player->pos.x, client->player->pos.y,
        client->player->level);
}
