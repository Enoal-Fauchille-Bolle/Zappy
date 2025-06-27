/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Right Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "map/orientation_names.h"
#include "team/player/player.h"
#include <stdbool.h>

/**
 * @brief Handles the right command to turn the player right.
 *
 * This function turns the player to the right, sets a cooldown for the
 * command, and sends an acknowledgment message to the client.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void right_command(client_t *client, command_t *command)
{
    (void)command;
    turn_player_right(client->player);
    client->player->tick_cooldown = RIGHT_COMMAND_COOLDOWN;
    send_to_client(client, "ok\n");
    debug_map(client->server->options->debug, "Player %zu turned right\n",
        client->player->id);
    debug_map(client->server->options->debug, "Player %zu orientation: %s\n",
        client->player->id, orientation_names[client->player->orientation]);
}
