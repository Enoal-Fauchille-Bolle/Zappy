/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include "command_handler/command_factory.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "map/orientation_names.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Destroys a client structure and frees all associated memory
 *
 * This function safely deallocates a client structure and all its associated
 * resources, including the team name string and player object. It performs
 * null checks to prevent segmentation faults.
 *
 * @param client Pointer to the client structure to destroy. Can be NULL.
 *
 * @note After calling this function, the client pointer should be considered
 * invalid.
 * @note The function is safe to call with NULL pointer.
 */
void destroy_client(client_t *client)
{
    if (client == NULL)
        return;
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        if (client->command_buffer[i] != NULL) {
            destroy_command(client->command_buffer[i]);
            client->command_buffer[i] = NULL;
        }
    }
    if (client->player != NULL && client->server != NULL &&
        client->server->game != NULL) {
        destroy_player(client->player);
        client->player = NULL;
    } else if (client->player != NULL) {
        free(client->player);
        client->player = NULL;
    }
    free(client);
    client = NULL;
}

/**
 * @brief Closes the connection for a client and updates the server state
 *
 * This function closes the file descriptor for a client, sends a disconnection
 * message if applicable, and updates the server's client array to reflect the
 * disconnection.
 *
 * @param server Pointer to the server structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
static void close_client_connection(server_t *server, int client_index)
{
    if (server->clients[client_index - 2] != NULL) {
        if (!server->clients[client_index - 2]->is_gui) {
            write(server->fds[client_index].fd, "dead\n", 5);
        } else {
            write(server->fds[client_index].fd, "seg\n", 4);
        }
    }
    debug_conn(
        server->options->debug, "Client %d disconnected\n", client_index - 2);
    close(server->fds[client_index].fd);
    server->fds[client_index].fd = -1;
    server->fds[client_index].events = 0;
    server->fds[client_index].revents = 0;
}

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
        close_client_connection(server, client_index);
    }
    if (server->clients[client_index - 2] != NULL) {
        if (server->clients[client_index - 2]->player != NULL &&
            !server->clients[client_index - 2]->is_gui) {
            debug_conn(server->options->debug,
                "Player %d (Client %d) removed from team '%s'\n",
                server->clients[client_index - 2]->player->id,
                client_index - 2,
                server->clients[client_index - 2]->player->team->name);
        } else {
            debug_conn(server->options->debug, "GUI client %d removed\n",
                client_index - 2);
        }
        destroy_client(server->clients[client_index - 2]);
        server->clients[client_index - 2] = NULL;
    }
}

/**
 * @brief Initializes a client structure with server connection details
 *
 * @param client Pointer to the client structure to initialize
 * @param server Pointer to the server instance
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
static void setup_client(
    client_t *client, server_t *server, int client_index, bool is_gui)
{
    client->server = server;
    client->index = client_index - 2;
    client->sockfd = server->fds[client_index].fd;
    client->player = NULL;
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        client->command_buffer[i] = NULL;
    }
    client->is_gui = is_gui;
}

static void setup_player(client_t *client, server_t *server, team_t *team)
{
    client->player = hatch_player(
        team, server->game->map, server->game->next_player_id, client);
    debug_map(server->options->debug,
        "Player %zu spawned at position (%d, %d) with orientation %s\n",
        client->player->id, client->player->pos.x, client->player->pos.y,
        orientation_names[client->player->orientation]);
    server->game->next_player_id++;
}

/**
 * @brief Creates a new client and associates it with a team and player
 *
 * Allocates memory for a new client structure, duplicates the team name,
 * and creates a hatched player for the client. Updates the server's next
 * player ID counter.
 *
 * @param server Pointer to the server structure
 * @param team Pointer to the team structure to associate with the client
 * @return Pointer to the newly created client, or NULL on failure
 */
client_t *create_client(server_t *server, team_t *team, int client_index)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client) {
        debug_warning(server->options->debug,
            "Failed to allocate memory for new client\n");
        return NULL;
    }
    setup_client(client, server, client_index, team == NULL);
    if (team == NULL) {
        debug_conn(server->options->debug, "Client %d connected as GUI\n",
            client_index - 2);
        return client;
    }
    setup_player(client, server, team);
    return client;
}
