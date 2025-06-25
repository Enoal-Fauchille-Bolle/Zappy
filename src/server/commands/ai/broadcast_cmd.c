/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Broadcast Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief Checks if the number of arguments is valid for the broadcast command.
 *
 * This function verifies that the command has exactly one argument (the
 * broadcast message). If not, it sends an error message to the client.
 *
 * @param command The command structure containing the arguments.
 * @param client The client that sent the command.
 * @return true if the number of arguments is valid, false otherwise.
 */
static bool check_args_number(command_t *command, client_t *client)
{
    if (command->argc != 1) {
        debug_warning(client->server->options->debug,
            "Invalid number of arguments for broadcast command\n");
        write(client->sockfd, "ko\n", 3);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Broadcasts a message to all players.
 *
 * This function sends a broadcast message to all players in the game.
 * It checks if the command has the correct number of arguments and
 * updates the player's cooldown.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the broadcast message.
 */
void broadcast_command(client_t *client, command_t *command)
{
    char *message = NULL;

    if (!check_args_number(command, client))
        return;
    message = command->argv[0];
    write(client->sockfd, "ok\n", 3);
    broadcast_to_all_players(client->server->game, client->player, message);
    pbc_event(client->player, message);
    client->player->tick_cooldown = BROADCAST_COMMAND_COOLDOWN;
    debug_player(client->server->options->debug,
        "Player %zu broadcasted '%s'\n", client->player->id, message);
}
