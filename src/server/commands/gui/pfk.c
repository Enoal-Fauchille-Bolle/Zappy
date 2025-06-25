/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pfk Command
*/

#include "connection/client.h"
#include "connection/message_sender.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Sends the ebo command to all GUI clients
 *
 * This function sends the "ebo" command with the egg ID to all GUI clients
 * connected to the server. It is used to notify GUIs about an egg's existence.
 *
 * @param egg Pointer to the egg structure containing the egg ID and team
 * information. If NULL, the function does nothing.
 */
void pfk_event(player_t *player)
{
    if (player == NULL)
        return;
    send_to_all_guis(player->team->game->server, "pfk #%zu\n", player->id);
}
