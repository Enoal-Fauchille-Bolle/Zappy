/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include "command_handler/command.h"
#include "command_handler/command_executor.h"
#include "command_handler/command_parser.h"
#include "connection/connection_handler.h"
#include "connection/server.h"
#include "connection/socket.h"
#include "debug.h"
#include "debug_categories.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "utils/string.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Destroy a command and free allocated resources.
 *
 * This function frees the memory associated with the command structure
 * and its arguments.
 *
 * @param command The command structure to destroy.
 */
static void destroy_command(command_t *command)
{
    if (command == NULL)
        return;
    if (command->tokens != NULL) {
        for (int i = 0; command->tokens[i] != NULL; i++) {
            free(command->tokens[i]);
        }
        free(command->tokens);
    }
    free(command);
    command = NULL;
}

/**
 * @brief Handles a command received from a client
 *
 * Parses the command buffer, executes the command if valid, and sends
 * appropriate response to the client. If parsing fails, sends "ko\n" to
 * client.
 *
 * @param server Pointer to the server structure
 * @param command_buffer String containing the command to be parsed and
 * executed
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @return true if command was successfully parsed and executed, false
 * otherwise
 */
static bool handle_command(
    server_t *server, char *command_buffer, int client_index)
{
    command_t *command = parse_command_buffer(command_buffer);

    if (command == NULL) {
        debug_warning(server->options->debug,
            "Failed to parse command from client %d\n", client_index - 2);
        write(server->fds[client_index].fd, "ko\n", 3);
        return false;
    }
    execute_command(server->clients[client_index - 2], command);
    destroy_command(command);
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
void handle_client_message(server_t *server, int client_index)
{
    char *message = read_socket(server->fds[client_index].fd);

    if (!message)
        return remove_client(server, client_index);
    trim(message);
    if (strlen(message) == 0)
        return free(message);
    if (server->clients[client_index - 2] == NULL) {
        handle_team_join(server, message, client_index);
    } else {
        to_lowercase(message);
        debug_cmd(server->options->debug, "Player %d (Client %d): '%s'\n",
            server->clients[client_index - 2]->player->id, client_index - 2,
            message);
        handle_command(server, message, client_index);
    }
    free(message);
}

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
    if (client->player != NULL) {
        destroy_player(client->player);
        client->player = NULL;
    }
    if (client->team_name != NULL) {
        free(client->team_name);
        client->team_name = NULL;
    }
    free(client);
    client = NULL;
}

/**
 * @brief Initializes a client structure with server connection details
 *
 * @param client Pointer to the client structure to initialize
 * @param server Pointer to the server instance
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
static void setup_client(client_t *client, server_t *server, int client_index)
{
    client->server = server;
    client->index = client_index - 2;
    client->sockfd = server->fds[client_index].fd;
    client->team_name = NULL;
    client->player = NULL;
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
    setup_client(client, server, client_index);
    if (team && team->name)
        client->team_name = strdup(team->name);
    if (!client->team_name) {
        debug_warning(server->options->debug,
            "Failed to allocate memory for team name\n");
        free(client);
        return NULL;
    }
    client->player = hatch_player(
        team, server->game->map, server->game->next_player_id, client);
    server->game->next_player_id++;
    return client;
}
