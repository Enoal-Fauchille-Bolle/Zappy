/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Inventory Command
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
#include <unistd.h>

static void send_error(client_t *client)
{
    send_to_client(client, "ko\n");
    if (client == NULL || client->server == NULL || client->player == NULL)
        return;
    debug_cmd(client->server->options->debug,
        "Player %zu failed to check inventory due to an error\n",
        client->player->id);
}

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
    char *inventory_str = NULL;

    (void)command;
    if (client == NULL)
        return;
    if (client->player == NULL) {
        send_error(client);
        return;
    }
    inventory_str = check_inventory(client->player);
    if (inventory_str == NULL) {
        send_error(client);
        return;
    }
    client->player->tick_cooldown = INVENTORY_COMMAND_COOLDOWN;
    send_to_client(client, "%s\n", inventory_str);
    debug_player(client->server->options->debug,
        "Player %zu checked inventory: %s\n", client->player->id,
        inventory_str);
    free(inventory_str);
}
