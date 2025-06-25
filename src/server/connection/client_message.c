/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Message Handler
*/

#include "command_handler/command.h"
#include "command_handler/command_buffer.h"
#include "command_handler/command_factory.h"
#include "command_handler/command_parser.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/socket.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game_state.h"
#include "team/player/player.h"
#include "utils/string.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send_to_all_guis(server_t *server, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    int len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len <= 0) {
        return;
    }
    for (int i = 2; i < MAX_CLIENTS + 2; i++) {
        if (server->clients[i - 2] != NULL && server->clients[i - 2]->is_gui) {
            write(server->clients[i - 2]->sockfd, buffer, len);
        }
    }
}

static void send_command_failed(client_t *client)
{
    if (client->is_gui) {
        write(client->sockfd, "sbp\n", 4);
    } else {
        write(client->sockfd, "ko\n", 3);
    }
}

/**
 * @brief Checks if the command is valid
 *
 * If the command is NULL, sends a failure response to the client and returns
 * FAILURE. Otherwise, returns SUCCESS.
 *
 * @param server Pointer to the server structure
 * @param command Pointer to the command structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @return SUCCESS if command is valid, FAILURE otherwise
 */
static bool check_command(
    server_t *server, command_t *command, int client_index)
{
    if (command == NULL) {
        debug_warning(server->options->debug,
            "Failed to parse command from client %d\n", client_index - 2);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Checks if the game state allows for command execution
 *
 * If the game state is GAME_END, sends a failure response to the client and
 * destroys the command. Returns FAILURE if the game is over, otherwise returns
 * SUCCESS.
 *
 * @param server Pointer to the server structure
 * @param command Pointer to the command structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @return SUCCESS if game state allows command execution, FAILURE otherwise
 */
static bool check_game_state(
    server_t *server, command_t *command, int client_index)
{
    if (server->game->game_state == GAME_END) {
        send_command_failed(server->clients[client_index - 2]);
        destroy_command(command);
        return FAILURE;
    }
    return SUCCESS;
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
 */
static void handle_command(
    server_t *server, char *command_buffer, int client_index)
{
    command_t *command = parse_command_buffer(command_buffer);

    if (!check_command(server, command, client_index))
        return;
    if (!check_game_state(server, command, client_index))
        return;
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
