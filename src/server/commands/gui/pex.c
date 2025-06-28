/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pex Command
*/

#include "connection/client.h"
#include "connection/message_sender.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Sends the pex command to all GUI clients.
 *
 * This function constructs a message containing the player's ID and sends it
 * to all GUI clients to indicate that the player ejected his tile.
 *
 * @param player The player who ejected his tile.
 */
void pex_event(player_t *player)
{
    if (player == NULL || player->client == NULL || player->client->server == NULL)
        return;
    send_to_all_guis(player->client->server, "pex #%zu\n", player->id);
}
