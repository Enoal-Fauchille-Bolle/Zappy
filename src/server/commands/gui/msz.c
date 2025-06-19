/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** msz Command
*/

#include "command_handler/command.h"
#include "connection/server.h"
#include "connection/client.h"
#include <stdbool.h>
#include <stdio.h>

void msz_command(client_t *client, command_t *command)
{
    (void)command;
    dprintf(client->sockfd, "msz %zu %zu\n", client->server->game->map->width,
        client->server->game->map->height);
}
