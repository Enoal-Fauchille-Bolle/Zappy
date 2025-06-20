/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pnw Command
*/

#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void pnw_command(player_t *player)
{
    if (player == NULL)
        return;
    for (int i = 2; i < MAX_CLIENTS + 2; i++) {
        if (player->client->server->clients[i - 2] == NULL ||
            !player->client->server->clients[i - 2]->is_gui)
            continue;
        dprintf(player->client->server->clients[i - 2]->sockfd,
            "pnw %zu %d %d %d %u %s\n", player->id, player->pos.x,
            player->pos.y, player->orientation + 1, player->level,
            player->team->name);
    }
}
