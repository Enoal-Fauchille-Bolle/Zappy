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
#include "game/game.h"
#include "game/game_state.h"
#include "options_parser/options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>

/**
 * @brief Destroys and frees memory allocated for client teams array
 *
 * This function safely deallocates memory for an array of client team strings.
 * It iterates through all possible client slots and frees each individual
 * team string before freeing the main array pointer.
 *
 * @param clients_team Array of strings representing client teams to be
 * destroyed. Can be NULL (function will return early if so).
 */
static void destroy_clients(server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] == NULL) {
            continue;
        }
        if (server->clients[i]->team_name != NULL) {
            free(server->clients[i]->team_name);
        }
        free(server->clients[i]);
    }
}

/**
 * @brief Initialize an array to store client team assignments
 *
 * Allocates memory for an array of string pointers to track which team
 * each client belongs to. All pointers are initialized to NULL.
 *
 * @return char** Pointer to the allocated array on success, NULL on failure
 */
static void init_clients(server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }
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
    if (server->game) {
        destroy_game(server->game);
    }
    for (size_t i = 0; i < MAX_CLIENTS + 2; i++) {
        if (server->fds[i].fd >= 0) {
            close(server->fds[i].fd);
        }
    }
    destroy_server_options(server->options);
    destroy_clients(server);
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
 * @brief Sets up and initializes the server with the given options
 *
 * This function initializes a server structure by setting up signal handling,
 * initializing client teams, configuring poll file descriptors, and
 * establishing the server socket on the specified port.
 *
 * @param server Pointer to the server structure to be initialized
 * @param options Pointer to server options containing configuration (e.g.,
 * port)
 *
 * @return true on successful setup, false on failure
 *
 * @note On failure, the function cleans up allocated resources
 * @warning The function will return NULL on socket setup failure
 */
static bool setup_server(server_t *server, server_options_t *options)
{
    int signal_fd = setup_signal_handler();

    if (signal_fd == -1) {
        perror("Failed to set up signal handler");
        return FAILURE;
    }
    memset(server, 0, sizeof(server_t));
    init_clients(server);
    init_poll_fds(server->fds, setup_socket_fd(), signal_fd);
    if (setup_socket(server, options->port) == FAILURE) {
        free(server);
        destroy_server_options(options);
        return NULL;
    }
    server->options = options;
    return SUCCESS;
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
    if (setup_server(server, options) == FAILURE) {
        return NULL;
    }
    server->game = create_game(server->options);
    if (!server->game) {
        destroy_server(server);
        return NULL;
    }
    return server;
}

/**
 * @brief Runs the main server loop to handle incoming connections
 *
 * This function starts the server listening process and continuously accepts
 * and processes new client connections while the game is in running state.
 * The server will stop when the game state changes or a connection processing
 * failure occurs.
 *
 * @param server Pointer to the server structure containing configuration and
 * game state
 */
// TODO: game_tick(server);
void run_server(server_t *server)
{
    debug_server(server->options->debug, "Listening on port %u\n",
        server->options->port);
    puts("Waiting for connections...");
    while (server->game->game_state == GAME_RUNNING) {
        if (process_connection(server) == FAILURE)
            break;
    }
    debug_server(server->options->debug, "Server stopped\n");
}
