/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** sgt Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include <stdbool.h>

/**
 * @brief Sends the current tick rate to the client
 *
 * This function sends the current tick rate of the game to the client
 * in response to the "sgt" command.
 *
 * @param client Pointer to the client structure
 * @param command Pointer to the command structure (unused)
 */
void sgt_command(client_t *client, command_t *command)
{
    (void)command;
    send_to_client(client, "sgt %u\n", client->server->game->tick_rate);
    debug_game(client->server->options->debug,
        "Client %d: sgt command sent: %zu tick speed\n", client->index,
        client->server->game->tick_rate);
}
