/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pic Command
*/

#include "connection/client.h"
#include "connection/message_receiver.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Sends the incantation event to all GUI clients.
 *
 * This function formats the incantation event message and sends it to all GUI
 * clients connected to the server. The message format is "pic <x> <y> <level>
 * <player_id1> <player_id2> ...\n".
 *
 * @param incantation The incantation structure containing the position, level,
 * and players involved.
 * @param server The server structure containing the list of GUI clients.
 */
void pic_event(incantation_t *incantation, server_t *server)
{
    char message[1024];
    player_t *player = NULL;

    if (incantation == NULL || server == NULL) {
        fprintf(stderr, "Invalid incantation or server pointer\n");
        return;
    }
    snprintf(message, sizeof(message), "pic %d %d %d", incantation->pos.x,
        incantation->pos.y, incantation->level);
    for (size_t i = 0; incantation->players[i] != NULL; i++) {
        player = incantation->players[i];
        if (!player || !player->client)
            continue;
        snprintf(message + strlen(message), sizeof(message) - strlen(message),
            " %lu", player->id);
    }
    strncat(message, "\n", sizeof(message) - strlen(message) - 1);
    send_to_all_guis(server, "%s", message);
}
