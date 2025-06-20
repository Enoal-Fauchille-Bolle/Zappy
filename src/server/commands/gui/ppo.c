/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ppo Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "game/game.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    dprintf(client->sockfd, "ppo %zu %d %d %d\n", player->id, player->pos.x,
        player->pos.y, player->orientation + 1);
}
