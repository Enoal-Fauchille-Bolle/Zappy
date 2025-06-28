/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "game/incantation.h"
#include "team/player/player.h"
#include <stdbool.h>

/**
 * @brief Check if the player can start an incantation
 *
 * This function checks if the player is already in an incantation or if the
 * requirements for the incantation are met. If not, it sends a "ko" response
 * to the client and logs a warning.
 *
 * @param client Pointer to the client structure representing the player
 * @return true if the player can start an incantation, false otherwise
 */
static bool can_start_incantation(client_t *client)
{
    if (!client->player || client->player->in_incantation) {
        send_to_client(client, "ko\n");
        debug_warning(client->server->options->debug,
            "Player %zu is already in an incantation\n", client->player->id);
        return FAILURE;
    }
    if (!check_incantation_requirements(client->server->game->map,
            client->player->pos, client->player->level)) {
        send_to_client(client, "ko\n");
        debug_warning(client->server->options->debug,
            "Player %zu at (%d, %d) failed incantation requirements for level "
            "%u\n",
            client->player->id, client->player->pos.x, client->player->pos.y,
            client->player->level);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Command handler for the incantation command
 *
 * This function is called when a player issues the incantation command. It
 * checks if the player can start an incantation, sets the cooldown, and
 * creates the incantation.
 *
 * @param client Pointer to the client structure representing the player
 * @param command Pointer to the command structure (not used in this function)
 */
void incantation_command(client_t *client, command_t *command)
{
    (void)command;
    if (client == NULL)
        return;
    if (client->player == NULL || client->server == NULL) {
        send_to_client(client, "ko\n");
        return;
    }
    if (!can_start_incantation(client))
        return;
    client->player->tick_cooldown = INCANTATION_COMMAND_COOLDOWN;
    debug_game(client->server->options->debug,
        "Player %zu started an "
        "incantation at (%d, %d) for level %u -> %u\n",
        client->player->id, client->player->pos.x, client->player->pos.y,
        client->player->level, client->player->level + 1);
    create_incantation(
        client->server->game, client->player->pos, client->player->level);
}
