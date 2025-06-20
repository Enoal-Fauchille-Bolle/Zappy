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

/**
 * @brief Sends the player new connection message to all GUI clients.
 *
 * This function constructs a message containing the player's ID, position,
 * orientation, level, and team name, and sends it to all GUI clients.
 *
 * @param player The player whose information is being sent.
 */
void pnw_command(player_t *player)
{
    if (player == NULL)
        return;
    send_to_all_guis(player->client->server, "pnw %zu %d %d %d %u %s\n",
        player->id, player->pos.x, player->pos.y, player->orientation + 1,
        player->level, player->team->name);
}
