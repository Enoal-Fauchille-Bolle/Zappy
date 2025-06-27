/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connect_nbr Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include "team/team.h"
#include <stdbool.h>

/**
 * @brief Handles the connect_nbr command to get the number of eggs in the
 * team.
 *
 * This function retrieves the number of eggs in the player's team and sends it
 * to the client. It also sets a cooldown for the command.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void connect_nbr_command(client_t *client, command_t *command)
{
    (void)command;
    client->player->tick_cooldown = CONNECT_NBR_COMMAND_COOLDOWN;
    send_to_client(client, "%zu\n", get_egg_count(client->player->team));
    debug_game(client->server->options->debug, "Team '%s' has %zu eggs\n",
        client->player->team->name, get_egg_count(client->player->team));
}
