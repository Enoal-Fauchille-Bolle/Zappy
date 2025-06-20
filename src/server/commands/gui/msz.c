/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** msz Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Handles the msz command to send the map size to the client.
 *
 * This function retrieves the map dimensions from the server's game and sends
 * them to the client in the format "msz width height\n".
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void msz_command(client_t *client, command_t *command)
{
    (void)command;
    dprintf(client->sockfd, "msz %zu %zu\n", client->server->game->map->width,
        client->server->game->map->height);
}
