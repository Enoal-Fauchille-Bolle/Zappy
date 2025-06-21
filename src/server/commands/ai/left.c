/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Left Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "map/orientation_names.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief Handles the left command to turn the player left.
 *
 * This function turns the player to the left, sets a cooldown for the
 * command, and sends an acknowledgment message to the client.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void left_command(client_t *client, command_t *command)
{
    (void)command;
    turn_player_left(client->player);
    client->player->tick_cooldown = LEFT_COMMAND_COOLDOWN;
    write(client->sockfd, "ok\n", 3);
    debug_map(client->server->options->debug, "Player %zu turned left\n",
        client->player->id);
    debug_map(client->server->options->debug, "Player %zu orientation: %s\n",
        client->player->id, orientation_names[client->player->orientation]);
}
