/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pdi Command
*/

#include "connection/client.h"
#include "connection/client_message.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Sends the player disconnect message to all GUI clients.
 *
 * This function constructs a message containing the player's ID and sends it
 * to all GUI clients to indicate that the player has disconnected.
 *
 * @param player The player who has disconnected.
 */
void pdi_event(player_t *player)
{
    if (player == NULL)
        return;
    send_to_all_guis(player->client->server, "pdi #%zu\n", player->id);
}
