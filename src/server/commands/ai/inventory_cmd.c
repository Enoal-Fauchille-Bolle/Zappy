/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Inventory Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game_constants.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Handles the inventory command to check the player's inventory.
 *
 * This function retrieves the player's inventory and sends it to the client.
 * If an error occurs during the inventory retrieval, it sends an error
 * message.
 *
 * @param client The client that sent the command.
 * @param command The command structure (not used in this case).
 */
void inventory_command(client_t *client, command_t *command)
{
    char *inventory_str = check_inventory(client->player);

    (void)command;
    if (inventory_str == NULL) {
        write(client->sockfd, "ko\n", 3);
        debug_cmd(client->server->options->debug,
            "Player %zu failed to check inventory due to an error\n",
            client->player->id);
        return;
    }
    client->player->tick_cooldown = INVENTORY_COMMAND_COOLDOWN;
    dprintf(client->sockfd, "%s\n", inventory_str);
    debug_player(client->server->options->debug,
        "Player %zu checked inventory: %s\n", client->player->id,
        inventory_str);
    free(inventory_str);
}
