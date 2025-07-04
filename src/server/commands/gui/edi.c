/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** edi Command
*/

#include "connection/client.h"
#include "connection/message_sender.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Sends the edi command to all GUI clients
 *
 * This function sends the "edi" command with the egg ID to all GUI clients
 * connected to the server. It is used to notify GUIs about an egg's death.
 *
 * @param egg Pointer to the egg structure containing the egg ID and team
 * information. If NULL, the function does nothing.
 */
void edi_event(egg_t *egg)
{
    if (egg == NULL)
        return;
    send_to_all_guis(egg->team->game->server, "edi #%zu\n", egg->id);
}
