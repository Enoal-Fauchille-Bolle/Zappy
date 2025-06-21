/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Fork Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief Handles the "fork" command for a client.
 *
 * This command allows a player to lay an egg, which is a new player that
 * will be created in the game. The player must have enough cooldown time
 * before they can use this command again.
 *
 * @param client Pointer to the client structure representing the player.
 * @param command Pointer to the command structure containing command details.
 */
void fork_command(client_t *client, command_t *command)
{
    (void)command;
    client->player->tick_cooldown = FORK_COMMAND_COOLDOWN;
    lay_egg(client->player, client->server->game->map);
    write(client->sockfd, "ok\n", 3);
    debug_map(client->server->options->debug,
        "Player %zu laid an egg at position (%d, %d)\n", client->player->id,
        client->player->pos.x, client->player->pos.y);
}
