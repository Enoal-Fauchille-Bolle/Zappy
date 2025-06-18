/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connection Handler
*/

#include "connection/connection_handler.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/signal_handler.h"
#include "constants.h"
#include "debug_categories.h"
#include "game/game_state.h"
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
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
    if (client_index < 2 || client_index >= MAX_CLIENTS + 2)
        return;
    if (server->fds[client_index].fd >= 0) {
        debug_conn(server->options->debug, "Client %d disconnected\n",
            client_index - 2);
        close(server->fds[client_index].fd);
    }
    if (server->clients[client_index - 2] != NULL) {
        debug_conn(server->options->debug,
            "Player %d (Client %d) removed from team '%s'\n",
            server->clients[client_index - 2]->player->id, client_index - 2,
            server->clients[client_index - 2]->player->team->name);
        destroy_client(server->clients[client_index - 2]);
        server->clients[client_index - 2] = NULL;
    }
    server->fds[client_index].fd = -1;
    server->fds[client_index].events = 0;
    server->fds[client_index].revents = 0;
}

/**
 * @brief Processes client events for all connected clients
 *
 * Iterates through the file descriptor array and processes incoming messages
 * from clients that have data ready to be read (POLLIN event). Also handles
 * client disconnections detected through POLLHUP events.
 *
 * @param fds Array of poll file descriptors to monitor
 * @param max_fds Maximum number of file descriptors in the array
 * @param clients Array of client structures corresponding to the file
 * descriptors
 */
static void process_client_events(server_t *server, int max_fds)
{
    for (int i = 2; i < max_fds; i++) {
        if (server->fds[i].fd < 0)
            continue;
        if (server->fds[i].revents & POLLHUP) {
            debug_conn(server->options->debug,
                "Client %d disconnected (POLLHUP)\n", i - 2);
            remove_client(server, i);
            continue;
        }
        if (server->fds[i].revents & POLLERR) {
            debug_conn(
                server->options->debug, "Client %d error (POLLERR)\n", i - 2);
            remove_client(server, i);
            continue;
        }
        if ((server->fds[i].revents & POLLIN) &&
            server->fds[i].revents & POLLOUT) {
            handle_client_message(server, i);
        }
    }
}

/**
 * @brief Initializes a new client connection for polling
 *
 * Sets up a pollfd structure for a new client socket and sends a welcome
 * message. Configures the socket to monitor for input, output, and hangup
 * events.
 *
 * @param fd Pointer to the pollfd structure to initialize
 * @param client_sockfd File descriptor of the client socket
 */
static void init_new_connection(struct pollfd *fd, int client_sockfd)
{
    fd->fd = client_sockfd;
    fd->events = POLLIN | POLLOUT | POLLHUP;
    write(client_sockfd, "WELCOME\n", 8);
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
        server->fds[0].fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_sockfd == -1)
        return perror("accept");
    for (int i = 2; i < MAX_CLIENTS + 2; i++) {
        if (server->fds[i].fd < 0) {
            debug_conn(server->options->debug,
                "Connection from %s:%d (Client %d)\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),
                i - 2);
            init_new_connection(&server->fds[i], client_sockfd);
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
 * @note Uses a timeout defined by the game tick rate to limit the
 * polling duration
 * @note Monitors up to MAX_CLIENTS + 2 file descriptors
 */
bool process_connection(server_t *server)
{
    int result = poll(server->fds, MAX_CLIENTS + 2,
        (1.0 / server->game->tick_rate) * 1000.0);

    if (result < 0) {
        if (errno != EINTR)
            perror("poll");
        return FAILURE;
    }
    if (server->fds[1].revents & POLLIN &&
        server->game->game_state == GAME_RUNNING) {
        if (!handle_signal(server->fds[1].fd, server->options->debug)) {
            return FAILURE;
        }
    }
    if (server->fds[0].revents & POLLIN) {
        accept_new_connection(server);
    }
    process_client_events(server, MAX_CLIENTS + 2);
    return SUCCESS;
}
