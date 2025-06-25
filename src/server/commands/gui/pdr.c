/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pdr Command
*/

#include "connection/client.h"
#include "connection/message_sender.h"
#include "map/resources.h"
#include "team/player/player.h"

/**
 * @brief Sends a pdr command to all GUI clients.
 *
 * This function sends a message to all GUI clients indicating that a player
 * has dropped a resource. The message includes the player's ID and the type of
 * resource dropped.
 *
 * @param player Pointer to the player structure whose resource is being sent.
 * @param resource The type of resource that the player has dropped.
 */
void pdr_event(player_t *player, resource_t resource)
{
    if (player == NULL)
        return;
    if (resource >= RESOURCE_COUNT)
        return;
    if (player->team == NULL || player->team->game == NULL ||
        player->team->game->server == NULL)
        return;
    send_to_all_guis(
        player->team->game->server, "pdr #%zu %zu\n", player->id, resource);
}
