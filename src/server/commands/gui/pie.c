/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pie Command
*/

#include "connection/client.h"
#include "connection/message_receiver.h"
#include "map/coordinates.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Sends the pie event to all GUI clients.
 *
 * This function formats the pie event message and sends it to all GUI clients
 * connected to the server. The message format is "pie <x> <y> <success>\n".
 *
 * @param pos The position where the pie event occurred.
 * @param success Indicates whether the pie event was successful.
 * @param server The server structure containing the list of GUI clients.
 */
void pie_event(pos_t pos, bool success, server_t *server)
{
    send_to_all_guis(server, "pie %d %d %d\n", pos.x, pos.y, success);
}
