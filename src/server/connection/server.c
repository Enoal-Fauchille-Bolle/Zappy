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
#include "debug_categories.h"
#include "options_parser/options.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Destroys and frees memory allocated for client teams array
 *
 * This function safely deallocates memory for an array of client team strings.
 * It iterates through all possible client slots and frees each individual
 * team string before freeing the main array pointer.
 *
 * @param client_teams Array of strings representing client teams to be
 * destroyed. Can be NULL (function will return early if so).
 */
static void destroy_client_teams(char **client_teams)
{
    if (!client_teams)
        return;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        free(client_teams[i]);
    }
    free(client_teams);
}

/**
 * @brief Initialize an array to store client team assignments
 *
 * Allocates memory for an array of string pointers to track which team
 * each client belongs to. All pointers are initialized to NULL.
 *
 * @return char** Pointer to the allocated array on success, NULL on failure
 */
static char **init_client_teams(void)
{
    char **client_teams = malloc(sizeof(char *) * (MAX_CLIENTS));

    if (!client_teams)
        return NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_teams[i] = NULL;
    }
    return client_teams;
}

/**
 * @brief Destroys a server instance and cleans up all associated resources
 *
 * This function safely deallocates a server structure by closing all client
 * connections, freeing client memory, closing the server socket, destroying
 * server options, and finally freeing the server structure itself.
 *
 * @param server Pointer to the server structure to destroy. If NULL, the
 * function returns immediately.
 */
void destroy_server(server_t *server)
{
    if (!server) {
        return;
    }
    destroy_server_options(server->options);
    destroy_client_teams(server->client_teams);
    close(server->sockfd);
    for (size_t i = 0; i < MAX_CLIENTS; i++) {
        if (server->fds[i + 1].fd >= 0) {
            close(server->fds[i + 1].fd);
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

    if (!server)
        return NULL;
    server->client_teams = init_client_teams();
    if (setup_socket(server, options->port) == FAILURE ||
        !server->client_teams) {
        free(server);
        destroy_server_options(options);
        return NULL;
    }
    server->options = options;
    return server;
}

/**
 * @brief Runs the server and starts listening for client connections
 *
 * This function initializes the server listening process, optionally prints
 * debug information about the port being used, and then enters the main
 * connection processing loop to handle incoming client connections.
 *
 * @param server Pointer to the server structure containing configuration and
 * state
 */
void run_server(server_t *server)
{
    debug_server(
        server->options->debug, "Listening on port %u\n", server->port);
    puts("Waiting for connections...");
    process_connections(server);
}
