/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** seg Command
*/

#include "connection/client.h"
#include "connection/client_message.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Sends the seg event to all GUI clients.
 *
 * This function formats the seg event message and sends it to all GUI clients
 * connected to the server. The message format is "seg <team_name>\n".
 *
 * @param team The team that has been segmented.
 * @param server The server structure containing the list of GUI clients.
 */
void seg_event(team_t *team, server_t *server)
{
    send_to_all_guis(server, "seg %s\n", team->name);
}
