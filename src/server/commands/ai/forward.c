/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Forward Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include <stdbool.h>

/**
 * @brief Handles the forward command to move the player forward.
 *
 * This function moves the player forward in the direction they are facing,
 * sets a cooldown for the command, and sends an acknowledgment message to
 * the client.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void forward_command(client_t *client, command_t *command)
{
    (void)command;
    move_player_forward(client->player, client->server->game->map);
    client->player->tick_cooldown = FORWARD_COMMAND_COOLDOWN;
    send_to_client(client, "ok\n");
    debug_map(client->server->options->debug, "Player %zu moved forward\n",
        client->player->id);
    debug_map(client->server->options->debug,
        "Player %zu position: (%d, %d)\n", client->player->id,
        client->player->pos.x, client->player->pos.y);
}
