/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Management
*/

#include "connection/server.h"
#include "connection/connection_handler.h"
#include "connection/socket.h"
#include "constants.h"
#include "options_parser/options.h"
#include <stdlib.h>
#include <unistd.h>

void destroy_server(server_t *server)
{
    if (!server) {
        return;
    }
    destroy_server_options(server->options);
    close(server->sockfd);
    for (size_t i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i]) {
            close(server->clients[i]->sockfd);
            free(server->clients[i]);
        }
    }
    free(server);
}

/**
 * @brief Creates and initializes a new server instance
 *
 * Allocates memory for a server structure and sets up the socket
 * connection using the specified port from the options.
 *
 * @param options Pointer to server options containing configuration parameters
 * @return Pointer to the created server instance on success, NULL on failure
 *
 * @note The caller is responsible for freeing the returned server instance
 * @note Returns NULL if socket setup fails or memory allocation fails
 */
server_t *create_server(server_options_t *options)
{
    server_t *server = malloc(sizeof(server_t));

    if (setup_socket(server, options->port) == FAILURE) {
        free(server);
        destroy_server_options(options);
        return NULL;
    }
    for (size_t i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }
    server->options = options;
    return server;
}

void run_server(server_t *server)
{
    if (server->options->debug) {
        printf("\nListening on port %u\n", server->port);
    }
    puts("Waiting for connections...");
    process_connections(server);
}
