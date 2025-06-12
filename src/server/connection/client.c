/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include "connection/server.h"
#include "connection/connection_handler.h"
#include "connection/socket.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void process_client_message(server_t *server, int client_index)
{
    char *message = read_socket(server->fds[client_index].fd);

    if (!message) {
        remove_client(server, client_index);
        return;
    }
    debug_cmd(server->options->debug, "Client %d: '%s'\n",
        server->fds[client_index].fd, message);
    free(message);
}
