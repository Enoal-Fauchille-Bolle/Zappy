/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include "connection/connection_handler.h"
#include "connection/server.h"
#include "connection/socket.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Validates if a team name exists in the server's team list
 *
 * Checks if the provided team name matches any of the teams defined in the
 * server's options. This is used to ensure clients can only join valid teams.
 *
 * @param server Pointer to the server structure containing team options
 * @param team_name The team name string to validate
 * @return true if the team name is found in the server's team list
 * @return false if the team name is not found
 */
static bool is_team_name_valid(server_t *server, const char *team_name)
{
    for (size_t i = 0; server->options->teams[i] != NULL; i++) {
        if (strcmp(server->options->teams[i], team_name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Parses and validates a team name from a client connection message
 *
 * Validates the team name against server rules and assigns the client to the
 * team if valid. Sends appropriate response messages to the client including
 * team capacity and world dimensions.
 *
 * @param server Pointer to the server structure
 * @param message The team name message received from the client
 * @param client_index Index of the client in the server's file descriptor
 * array
 *
 * @return true if team name is valid and client was successfully assigned
 * @return false if team name is invalid
 *
 * @note Currently sends "0" as team capacity placeholder - needs
 * implementation to send actual available slots for the team
 */
// TODO: Server should send the real number of clients that can join a team
//       instead of 0
static bool parse_team_name(
    server_t *server, const char *message, int client_index)
{
    if (!is_team_name_valid(server, message)) {
        debug_conn(server->options->debug,
            "Client %d tried to join invalid team '%s'\n",
            server->fds[client_index].fd, message);
        write(server->fds[client_index].fd, "ko\n", 3);
        return false;
    }
    server->client_teams[client_index] = strdup(message);
    debug_conn(server->options->debug, "Client %d assigned to team '%s'\n",
        server->fds[client_index].fd, server->client_teams[client_index]);
    dprintf(server->fds[client_index].fd, "0\n");
    dprintf(server->fds[client_index].fd, "%ld %ld\n", server->options->width,
        server->options->height);
    return true;
}

static bool parse_command(
    server_t *server, const char *message, int client_index)
{
    (void)server;
    (void)message;
    (void)client_index;
    return true;
}

/**
 * @brief Processes a message received from a client
 *
 * Reads a message from the specified client socket and handles it
 * appropriately. If no team is assigned to the client, the message is parsed
 * as a team name. Otherwise, the message is parsed as a command. Removes the
 * client if no message is received.
 *
 * @param server Pointer to the server structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
void process_client_message(server_t *server, int client_index)
{
    char *message = read_socket(server->fds[client_index].fd);

    if (!message) {
        remove_client(server, client_index);
        return;
    }
    if (strlen(message) == 0) {
        free(message);
        return;
    }
    if (server->client_teams[client_index] == NULL) {
        parse_team_name(server, message, client_index);
    } else {
        debug_cmd(server->options->debug, "Client %d: '%s'\n",
            server->fds[client_index].fd, message);
        parse_command(server, message, client_index);
    }
    free(message);
}
