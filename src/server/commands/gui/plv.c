/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** plv Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
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
    dprintf(client->sockfd, "plv %zu %d\n", player->id, player->level);
    debug_player(client->server->options->debug,
        "plv command sent for player ID %zu: level %d\n", player->id,
        player->level);
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

    if (command->argc != 1) {
        write(client->sockfd, "sbp\n", 4);
        return;
    }
    player_id = strtoull(command->argv[0], NULL, 10);
    if (player_id == 0) {
        write(client->sockfd, "sbp\n", 4);
        return;
    }
    player = get_player_by_id(client->server->game, player_id);
    if (player == NULL) {
        write(client->sockfd, "sbp\n", 4);
        return;
    }
    send_player_info(client, player);
}
