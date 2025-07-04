/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Management
*/

#include "connection/server.h"
#include "connection/client.h"
#include "connection/connection_handler.h"
#include "connection/signal_handler.h"
#include "connection/socket.h"
#include "constants.h"
#include "debug_categories.h"
#include "game/game.h"
#include "options_parser/options.h"
#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Destroys and frees memory allocated for client teams array
 *
 * This function safely deallocates memory for an array of client team strings.
 * It iterates through all possible client slots and frees each individual
 * client structure and closes their file descriptors.
 *
 * @param server Pointer to the server structure containing the clients array
 */
static void destroy_clients(server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] != NULL) {
            remove_client(server, i + 2);
        }
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
    destroy_clients(server);
    if (server->fds[0].fd >= 0) {
        close(server->fds[0].fd);
    }
    if (server->fds[1].fd >= 0) {
        close(server->fds[1].fd);
    }
    if (server->game) {
        destroy_game(server->game);
    }
    debug_server(server->options->debug, "Server stopped\n");
    destroy_server_options(server->options);
    free(server);
    server = NULL;
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
    server->game = create_game(server);
    if (!server->game) {
        destroy_server(server);
        return NULL;
    }
    return server;
}

/**
 * @brief Gets the current time in milliseconds since the epoch
 *
 * This function retrieves the current time using CLOCK_MONOTONIC and converts
 * it to milliseconds.
 *
 * @return long long Current time in milliseconds
 */
static long long get_current_time_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}

/**
 * @brief Calculates the timeout for poll() based on next tick timing
 *
 * This function determines how long poll() should wait before timing out,
 * ensuring the game tick executes at the correct time while keeping the
 * timeout within reasonable bounds.
 *
 * @param next_tick_time Absolute time when the next game tick should occur
 * @param tick_duration_ms Duration of one game tick in milliseconds
 * @return int Timeout value in milliseconds for poll()
 */
static int calculate_poll_timeout(
    long long next_tick_time, long long tick_duration_ms)
{
    long long current_time = get_current_time_ms();
    int timeout_ms = (int)(next_tick_time - current_time);

    if (timeout_ms < 1)
        timeout_ms = 1;
    if (timeout_ms > tick_duration_ms)
        timeout_ms = tick_duration_ms;
    return timeout_ms;
}

/**
 * @brief Processes a single server loop iteration
 *
 * This function handles one iteration of the server loop, including timing
 * calculations, game tick execution, and connection processing.
 *
 * @param server Pointer to the server structure
 * @param next_tick_time Pointer to the next tick time (modified)
 * @param tick_duration_ms Duration of one game tick in milliseconds
 * @return true to continue the loop, false to break
 */
static bool process_server_iteration(
    server_t *server, long long *next_tick_time, long long tick_duration_ms)
{
    long long current_time = get_current_time_ms();
    int timeout_ms = 0;
    bool poll_timed_out = false;

    if (current_time >= *next_tick_time) {
        game_tick(server->game, server->options);
        *next_tick_time += tick_duration_ms;
        current_time = get_current_time_ms();
        if (current_time >= *next_tick_time) {
            *next_tick_time = current_time + tick_duration_ms;
        }
    }
    timeout_ms = calculate_poll_timeout(*next_tick_time, tick_duration_ms);
    if (process_connection(server, timeout_ms, &poll_timed_out) == FAILURE)
        return false;
    return true;
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
void run_server(server_t *server)
{
    long long tick_duration_ms = 1000 / (long long)server->game->tick_rate;
    long long next_tick_time = 0;

    debug_server(server->options->debug, "Listening on port %u\n",
        server->options->port);
    puts("Waiting for connections...");
    debug_game(server->options->debug, "1 tick = %lld ms\n", tick_duration_ms);
    next_tick_time = get_current_time_ms() + tick_duration_ms;
    while (process_server_iteration(server, &next_tick_time, tick_duration_ms))
        ;
}
