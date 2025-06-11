/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "debug.h"
#include "connection/client.h"
#include "connection/connection_handler.h"
#include "connection/socket.h"
#include <stdlib.h>
#include <string.h>

void process_client_message(struct pollfd *fds, int client_index, bool debug)
{
    char *message = read_socket(fds[client_index].fd);

    if (!message) {
        remove_client(fds, client_index, debug);
        return;
    }
    debug_cmd(debug, "Client %d: '%s'\n",
        fds[client_index].fd, message);
    free(message);
    return;
}
