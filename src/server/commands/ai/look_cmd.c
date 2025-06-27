/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Look Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Handles the look command to get the player's vision.
 *
 * This function retrieves the vision of the player and sends it to the client.
 * If an error occurs during the vision retrieval, it sends an error message.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void look_command(client_t *client, command_t *command)
{
    char *vision = look(client->player, client->server->game->map);

    (void)command;
    if (vision == NULL) {
        send_to_client(client, "ko\n");
        debug_cmd(client->server->options->debug,
            "Player %zu failed to look due to an error\n", client->player->id);
        return;
    }
    client->player->tick_cooldown = LOOK_COMMAND_COOLDOWN;
    send_to_client(client, "%s\n", vision);
    debug_map(client->server->options->debug,
        "Player %zu looked around and saw: %s\n", client->player->id, vision);
    free(vision);
}
