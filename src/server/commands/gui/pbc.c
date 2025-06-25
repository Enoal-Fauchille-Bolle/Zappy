/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pbc Command
*/

#include "connection/client.h"
#include "connection/client_message.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Broadcast a message to all GUI clients.
 *
 * This function formats the broadcast message and sends it to all GUI clients
 * connected to the server. The message format is "pbc <player_id> <message>\n".
 *
 * @param player The player who is broadcasting the message.
 * @param message The message to broadcast.
 */
void pbc_event(player_t *player, char *message)
{
    if (player == NULL)
        return;
    send_to_all_guis(
        player->client->server, "pbc #%zu %s\n", player->id, message);
}
