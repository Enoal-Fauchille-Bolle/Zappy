/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pnw Command
*/

#include "connection/client.h"
#include "connection/client_message.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void pnw_command(player_t *player)
{
    if (player == NULL)
        return;
    send_to_all_guis(player->client->server,
        "pnw %zu %d %d %d %u %s\n", player->id, player->pos.x, player->pos.y,
        player->orientation + 1, player->level, player->team->name);
}
