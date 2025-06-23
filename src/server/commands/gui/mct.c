/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** mct Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/resources.h"
#include "map/tile.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Send tile information to the client.
 *
 * This function formats the tile information and sends it to the client in
 * the expected format for the GUI.
 *
 * @param client The client requesting the tile information.
 * @param tile The tile containing the resource information.
 * @param pos The position of the tile.
 */
static void send_tile_info(client_t *client, tile_t *tile, pos_t pos)
{
    dprintf(client->sockfd, "bct %d %d %zu %zu %zu %zu %zu %zu %zu\n", pos.x,
        pos.y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    debug_resource(client->server->options->debug,
        "bct command sent for tile at (%d, %d): "
        "%zu food, %zu linemate, %zu deraumere, %zu sibur, "
        "%zu mendiane, %zu phiras, %zu thystame\n",
        pos.x, pos.y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
}

/**
 * @brief Iterate through the tiles in the map and send their information to
 * the client.
 *
 * This function retrieves each tile in the map and sends its resource
 * information to the client using the send_tile_info function.
 *
 * @param client The client requesting the tile information.
 * @param pos The current position in the map being processed.
 * @param vtable The vector vtable used to access the tiles.
 */
static void iterate_tiles(
    client_t *client, pos_t *pos, const vector_vtable_t *vtable)
{
    tile_t *tile = vtable->at(client->server->game->map->tiles,
        pos->y * client->server->game->map->width + pos->x);

    if (tile == NULL) {
        debug_error(client->server->options->debug,
            "Failed to retrieve tile at (%d, %d)\n", pos->x, pos->y);
        return;
    }
    send_tile_info(client, tile, *pos);
    pos->x++;
    if (pos->x >= (int)client->server->game->map->width) {
        pos->x = 0;
        pos->y++;
    }
}

/**
 * @brief Handle the mct command for the GUI.
 *
 * This function retrieves the tile information at the specified position and
 * sends it to the client in the format expected by the GUI.
 *
 * @param client The client requesting the tile information.
 * @param command The command structure containing the position arguments.
 */
void mct_command(client_t *client, command_t *command)
{
    const vector_vtable_t *vtable =
        vector_get_vtable(client->server->game->map->tiles);
    pos_t pos = {0, 0};

    (void)command;
    while (pos.y < (int)client->server->game->map->height &&
        pos.x < (int)client->server->game->map->width) {
        iterate_tiles(client, &pos, vtable);
    }
}
