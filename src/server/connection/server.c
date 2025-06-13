/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Management
*/

#include "connection/server.h"
#include "connection/connection_handler.h"
#include "connection/signal_handler.h"
#include "connection/socket.h"
#include "constants.h"
#include "debug_categories.h"
#include "options_parser/options.h"
#include <stdio.h>
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
    destroy_client_teams(server->clients_team);
    for (size_t i = 0; i < MAX_CLIENTS + 1; i++) {
        if (server->fds[i].fd >= 0) {
            close(server->fds[i].fd);
        }
    }
    free(server);
}

/**
 * @brief Initialize polling file descriptors array for server connections
 *
 * Sets up the pollfd array by initializing all client slots to -1 (unused)
 * and configuring the server socket at index 0 to listen for incoming
 * connections.
 *
 * @param fds Pointer to array of pollfd structures to initialize
 * @param server_sockfd Server socket file descriptor to monitor for new
 * connections
 */
static void init_poll_fds(struct pollfd *fds, int server_sockfd, int signal_fd)
{
    for (int i = 0; i < MAX_CLIENTS + 2; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = server_sockfd;
    fds[0].events = POLLIN;
    fds[1].fd = signal_fd;
    fds[1].events = POLLIN;
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
    memset(server, 0, sizeof(server_t));
    if (setup_socket(server, options->port) == FAILURE ||
    init_poll_fds(server->fds, setup_socket_fd(), setup_signal_handler());
    if (setup_socket(server, options->port) == FAILURE) {
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
void run_server(server_t *server, port_t port)
{
    debug_server(
        server->options->debug, "Listening on port %u\n", port);
    puts("Waiting for connections...");
    process_connections(server);
}
