/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connection Handler
*/

#include "connection/connection_handler.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Removes a client from the server and poll arrays
 *
 * This function safely disconnects a client by removing it from both the
 * server's client array and the poll file descriptor array, then destroys
 * the client structure.
 *
 * @param server Pointer to the server structure
 * @param fds Array of poll file descriptors
 * @param client_index Index of the client to remove (1-based for fds array)
 */
void remove_client(server_t *server, int client_index)
{
    if (client_index < 1 || client_index >= MAX_CLIENTS + 1)
        return;
    if (server->client_teams[client_index] != NULL) {
        debug_conn(server->options->debug,
            "Client %d removed from team '%s'\n",
            server->fds[client_index].fd, server->client_teams[client_index]);
        free(server->client_teams[client_index]);
        server->client_teams[client_index] = NULL;
    }
    debug_conn(server->options->debug, "Client %d disconnected\n",
        server->fds[client_index].fd);
    server->fds[client_index].fd = -1;
    server->fds[client_index].events = 0;
    server->fds[client_index].revents = 0;
}

/**
 * @brief Processes client events for all connected clients
 *
 * Iterates through the file descriptor array and processes incoming messages
 * from clients that have data ready to be read (POLLIN event).
 *
 * @param fds Array of poll file descriptors to monitor
 * @param max_fds Maximum number of file descriptors in the array
 * @param clients Array of client structures corresponding to the file
 * descriptors
 */
static void process_client_events(server_t *server, int max_fds)
{
    for (int i = 1; i < max_fds; i++) {
        if (server->fds[i].fd < 0)
            continue;
        if (server->fds[i].revents & POLLIN) {
            process_client_message(server, i);
        }
    }
}

/**
 * @brief Accepts a new client connection and adds it to the server's client
 * pool
 *
 * This function accepts an incoming connection on the server socket, creates a
 * new client structure, and assigns it to the first available slot in the
 * clients array and file descriptor polling array.
 *
 * @param server Pointer to the server structure containing socket and client
 * information
 * @param fds Array of pollfd structures for monitoring file descriptors
 *
 * @note If the accept fails, an error is printed via perror and the function
 * returns
 * @note Debug information is printed if server debug mode is enabled
 * @note The function searches for the first available slot (fd < 0) in the fds
 * array
 */
static void accept_new_connection(server_t *server)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(
        server->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_sockfd == -1)
        return perror("accept");
    debug_conn(server->options->debug, "Connection from %s:%d (%d)\n",
        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),
        client_sockfd);
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        if (server->fds[i].fd < 0) {
            write(client_sockfd, "WELCOME\n", 8);
            server->fds[i].fd = client_sockfd;
            server->fds[i].events = POLLIN;
            break;
        }
    }
}

/**
 * @brief Processes incoming connections and client events using poll()
 *
 * This function handles network I/O by polling file descriptors for activity.
 * It accepts new connections when available and processes events from existing
 * clients.
 *
 * @param server Pointer to the server structure containing game state and
 * client data
 * @param fds Array of pollfd structures for monitoring file descriptors
 * @return true on success, false on failure (poll error)
 *
 * @note Uses a timeout defined by POLL_TIMEOUT constant
 * @note Monitors up to MAX_CLIENTS + 1 file descriptors
 */
// TODO: if (fds[0].revents & POLLIN && server->game->game_state == GAME_START)
static bool process_connection(server_t *server)
{
    int result = poll(server->fds, MAX_CLIENTS + 1, POLL_TIMEOUT);

    if (result < 0) {
        if (errno != EINTR)
            perror("poll");
        return FAILURE;
    }
    if (server->fds[0].revents & POLLIN) {
        accept_new_connection(server);
    }
    process_client_events(server, MAX_CLIENTS + 1);
    return SUCCESS;
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
static void init_poll_fds(struct pollfd *fds, int server_sockfd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = server_sockfd;
    fds[0].events = POLLIN;
}

/**
 * @brief Processes client connections and handles communication with connected
 * clients
 *
 * This function initializes polling file descriptors and enters a main loop to
 * continuously process client connections. It handles incoming client requests
 * and manages the server's connection state until a failure occurs or the
 * server is stopped.
 *
 * @param server Pointer to the server structure containing socket information
 *               and client management data
 */
// TODO: Replace the "while (true)" with the actual game state check
// TODO:
// if (game is running)
//     game_tick(server);
void process_connections(server_t *server)
{
    init_poll_fds(server->fds, server->sockfd);
    while (true) {
        if (process_connection(server) == FAILURE)
            break;
    }
    destroy_server(server);
    debug_conn(server->options->debug, "Server stopped\n");
}
