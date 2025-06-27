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
#include "debug.h"
#include "debug_categories.h"
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

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

static void refuse_connection(server_t *server, int client_sockfd)
{
    debug_conn(server->options->debug,
        "Connection refused - server at capacity (%d/%d clients)\n",
        MAX_CLIENTS, MAX_CLIENTS);
    write(client_sockfd, "ko\n", 3);
    close(client_sockfd);
}

/**
 * @brief Checks socket error status using getsockopt
 *
 * @param sockfd Socket file descriptor to check
 * @param debug Debug flag for logging
 * @return true if socket has no errors, false otherwise
 */
static bool check_socket_error(int sockfd, bool debug)
{
    socklen_t errlen = sizeof(errno);

    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &errno, &errlen) != 0) {
        debug_warning(debug, "getsockopt failed on socket %d: %s\n", sockfd,
            strerror(errno));
        return false;
    }
    if (errno) {
        debug_warning(
            debug, "Socket %d is not writable: %s\n", sockfd, strerror(errno));
        return false;
    }
    return true;
}

/**
 * @brief Polls socket for write readiness
 *
 * @param sockfd Socket file descriptor to poll
 * @param debug Debug flag for logging
 * @return true if socket is ready for writing, false otherwise
 */
static bool poll_socket_write(int sockfd, bool debug)
{
    struct pollfd pfd = {sockfd, POLLOUT, 0};

    if (poll(&pfd, 1, 0) <= 0) {
        if (errno != EINTR) {
            debug_warning(debug, "poll failed on socket %d: %s\n", sockfd,
                strerror(errno));
        }
        return false;
    }
    if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
        debug_warning(debug, "Socket %d has error events: %s\n", sockfd,
            strerror(errno));
        return false;
    }
    if (!(pfd.revents & POLLOUT)) {
        debug_warning(debug, "Socket %d is not ready for writing\n", sockfd);
        return false;
    }
    return true;
}

/**
 * @brief Checks if a socket is writable using a non-blocking write test
 *
 * Uses a non-blocking approach to test if the server can write to a client
 * socket. This is more efficient than using poll() as it doesn't block other
 * operations.
 *
 * @param sockfd Socket file descriptor to test
 * @param debug Debug flag for logging
 * @return true if socket is writable, false otherwise
 */
static bool is_socket_writable(int sockfd, bool debug)
{
    return check_socket_error(sockfd, debug) &&
        poll_socket_write(sockfd, debug);
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
    if (!is_socket_writable(client_sockfd, server->options->debug))
        return;
    for (int i = 2; i < MAX_CLIENTS + 2; i++) {
        if (server->fds[i].fd < 0) {
            debug_conn(server->options->debug,
                "Connection from %s:%d (Client %d)\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),
                i - 2);
            init_new_connection(&server->fds[i], client_sockfd);
            return;
        }
    }
    refuse_connection(server, client_sockfd);
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
 * @param timeout_ms Timeout in milliseconds for the poll() call
 * @param timed_out Pointer to bool indicating if poll() timed out (set to true
 * if timeout, false if activity detected)
 * @return true on success, false on failure (poll error)
 *
 * @note Monitors up to MAX_CLIENTS + 2 file descriptors
 */
bool process_connection(server_t *server, int timeout_ms, bool *timed_out)
{
    int result = poll(server->fds, MAX_CLIENTS + 2, timeout_ms);

    if (result < 0) {
        if (errno != EINTR)
            perror("poll");
        return FAILURE;
    }
    *timed_out = (result == 0);
    if (server->fds[1].revents & POLLIN) {
        if (!handle_signal(server->fds[1].fd, server->options->debug)) {
            return FAILURE;
        }
    }
    if ((server->fds[0].revents & POLLIN) &&
        !(server->fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
        accept_new_connection(server);
    }
    process_client_events(server, MAX_CLIENTS + 2);
    return SUCCESS;
}
