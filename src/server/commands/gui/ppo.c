/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ppo Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
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

/**
 * @brief Sends player information to the client.
 *
 * This function formats and sends the player's position and orientation to the
 * client in the format "ppo <player_id> <x> <y> <orientation>\n".
 *
 * @param client The client to which the player information will be sent.
 * @param player The player whose information is being sent.
 */
static void send_player_info(client_t *client, player_t *player)
{
    send_to_client(client, "ppo #%zu %d %d %d\n", player->id, player->pos.x,
        player->pos.y, player->orientation + 1);
    debug_player(client->server->options->debug,
        "Client %d: ppo command sent for player ID %zu: position (%d, %d), "
        "orientation "
        "%d\n",
        client->index, player->id, player->pos.x, player->pos.y,
        player->orientation + 1);
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
            "Invalid number of arguments for ppo command\n");
        send_to_client(client, "sbp\n");
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
            "Invalid player ID for ppo command: %zu\n", player_id);
        send_to_client(client, "sbp\n");
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
            "Player not found for ppo command\n");
        send_to_client(client, "sbp\n");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Sends player position and orientation to all GUIs.
 *
 * This function formats the player's position and orientation and sends it to
 * all connected GUIs. It is typically called when a player's position changes.
 *
 * @param player The player whose position and orientation will be sent.
 */
void ppo_event(player_t *player)
{
    if (player == NULL || player->client == NULL ||
        player->client->server == NULL) {
        fprintf(stderr, "Invalid player or client pointer\n");
        return;
    }
    send_to_all_guis(player->client->server, "ppo #%zu %d %d %d\n", player->id,
        player->pos.x, player->pos.y, player->orientation + 1);
}

/**
 * @brief Handles the ppo command to send player position to the client.
 *
 * This function checks if the command has the correct number of arguments,
 * retrieves the player by ID, and sends their position and orientation to the
 * client.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the player ID.
 */
void ppo_command(client_t *client, command_t *command)
{
    size_t player_id = 0;
    player_t *player = NULL;

    if (check_args_number(command, client) == FAILURE)
        return;
    player_id = strtoull(command->argv[0], NULL, 10);
    if (check_player_id(player_id, client) == FAILURE)
        return;
    player = get_player_by_id(client->server->game, player_id);
    if (check_player_exists(player, client) == FAILURE)
        return;
    send_player_info(client, player);
}
