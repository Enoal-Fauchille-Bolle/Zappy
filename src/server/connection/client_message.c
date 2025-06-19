/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Message Handler
*/

#include "command_handler/command.h"
#include "command_handler/command_buffer.h"
#include "command_handler/command_parser.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/socket.h"
#include "debug.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include "utils/string.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
 */
static void handle_command(
    server_t *server, char *command_buffer, int client_index)
{
    command_t *command = parse_command_buffer(command_buffer);

    if (command == NULL) {
        debug_warning(server->options->debug,
            "Failed to parse command from client %d\n", client_index - 2);
        write(server->fds[client_index].fd, "ko\n", 3);
        return;
    }
    add_command_to_buffer(server->clients[client_index - 2], command);
    if (!server->clients[client_index - 2]->is_gui) {
        debug_cmd(server->options->debug,
            "Player %zu: '%s' command added to buffer\n",
            server->clients[client_index - 2]->player->id, command_buffer);
    } else {
        debug_cmd(server->options->debug,
            "GUI Client %d: '%s' command added to buffer\n", client_index - 2,
            command_buffer);
    }
}

/**
 * @brief Handles a team join request from a client
 *
 * Checks if the team exists and if the client can join it. If successful,
 * creates a new client, assigns it to the team, and sends a welcome message.
 * If the team does not exist or the client cannot join, sends an error
 * message.
 *
 * @param server Pointer to the server structure
 * @param team_name Name of the team to join
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
static void process_single_command(
    server_t *server, char *command, int client_index)
{
    trim(command);
    if (strlen(command) > 0) {
        handle_command(server, command, client_index);
    }
}

/**
 * @brief Handles a team join request from a client
 *
 * Checks if the team exists and if the client can join it. If successful,
 * creates a new client, assigns it to the team, and sends a welcome message.
 * If the team does not exist or the client cannot join, sends an error
 * message.
 *
 * @param server Pointer to the server structure
 * @param team_name Name of the team to join
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
static void handle_all_commands(
    server_t *server, char *message, int client_index)
{
    char *message_copy = strdup(message);
    char *start = message_copy;
    char *end = strchr(start, '\n');

    while (end != NULL) {
        *end = '\0';
        process_single_command(server, start, client_index);
        start = end + 1;
        end = strchr(start, '\n');
    }
    process_single_command(server, start, client_index);
    free(message_copy);
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
    if (strlen(message) == 0) {
        free(message);
        return;
    }
    if (server->clients[client_index - 2] == NULL) {
        trim(message);
        handle_team_join(server, message, client_index);
    } else {
        to_lowercase(message);
        handle_all_commands(server, message, client_index);
    }
    free(message);
}
