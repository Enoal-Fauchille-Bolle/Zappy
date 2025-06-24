/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** plv Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/client_message.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Sends player information to the client.
 *
 * This function formats and sends the player's level to the client in the
 * format "plv <player_id> <level>\n".
 *
 * @param client The client to which the player information will be sent.
 * @param player The player whose information is being sent.
 */
static void send_player_info(client_t *client, player_t *player)
{
    dprintf(client->sockfd, "plv #%zu %d\n", player->id, player->level);
    debug_player(client->server->options->debug,
        "Client %d: plv command sent for player ID %zu: level %d\n",
        client->index, player->id, player->level);
}

/**
 * @brief Checks if the command has the correct number of arguments.
 *
 * This function validates that the command has exactly one argument (the
 * player ID). If not, it sends an error message to the client.
 *
 * @param command The command structure containing the arguments.
 * @param client The client that sent the command.
 * @return true if the number of arguments is valid, false otherwise.
 */
static bool check_args_number(command_t *command, client_t *client)
{
    if (command->argc != 1) {
        debug_warning(client->server->options->debug,
            "Invalid number of arguments for plv command\n");
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Checks if the player ID is valid.
 *
 * This function checks if the player ID is greater than zero. If not, it sends
 * an error message to the client.
 *
 * @param player_id The player ID to check.
 * @param client The client that sent the command.
 * @return true if the player ID is valid, false otherwise.
 */
static bool check_player_id(size_t player_id, client_t *client)
{
    if (player_id == 0) {
        debug_warning(client->server->options->debug,
            "Invalid player ID for plv command: %zu\n", player_id);
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Checks if the player exists in the game.
 *
 * This function retrieves the player by ID and checks if they exist. If the
 * player does not exist, it sends an error message to the client.
 *
 * @param player The player structure to check.
 * @param client The client that sent the command.
 * @return true if the player exists, false otherwise.
 */
static bool check_player_exists(player_t *player, client_t *client)
{
    if (player == NULL) {
        debug_warning(client->server->options->debug,
            "Player not found for plv command\n");
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Sends player level event to all GUI clients.
 *
 * This function sends the player's level to all GUI clients in the format
 * "plv <player_id> <level>\n". It is called when a player's level changes.
 *
 * @param player The player whose level has changed.
 */
void plv_event(player_t *player)
{
    if (player == NULL || player->client == NULL) {
        debug_warning(player->client->server->options->debug,
            "Player or client is NULL in plv_event\n");
        return;
    }
    send_to_all_guis(
        player->client->server, "plv #%zu %d\n", player->id, player->level);
}

/**
 * @brief Handles the plv command to send player level to the client.
 *
 * This function checks if the command has the correct number of arguments,
 * retrieves the player by ID, and sends their level to the client.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the player ID.
 */
void plv_command(client_t *client, command_t *command)
{
    size_t player_id = 0;
    player_t *player = NULL;

    if (!check_args_number(command, client))
        return;
    player_id = strtoull(command->argv[0], NULL, 10);
    if (!check_player_id(player_id, client))
        return;
    player = get_player_by_id(client->server->game, player_id);
    if (!check_player_exists(player, client))
        return;
    send_player_info(client, player);
}
