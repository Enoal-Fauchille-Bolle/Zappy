/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** bct Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/client_message.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/resources.h"
#include "map/tile.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Check if the position is valid for the bct command.
 *
 * This function checks if the provided position has valid coordinates.
 * If the coordinates are invalid, it sends an error message to the client.
 *
 * @param pos The position to check.
 * @param client The client requesting the tile information.
 * @return true if the position is valid, false otherwise.
 */
static bool check_pos_valid(pos_t pos, client_t *client)
{
    if (pos.x < 0 || pos.y < 0) {
        debug_warning(client->server->options->debug,
            "Invalid coordinates for bct command\n");
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Check if the tile is valid for the bct command.
 *
 * This function checks if the tile at the given position exists and is valid.
 * If not, it sends an error message to the client.
 *
 * @param tile The tile to check.
 * @param client The client requesting the tile information.
 * @param pos The position of the tile.
 * @return true if the tile is valid, false otherwise.
 */
static bool check_tile_valid(tile_t *tile, client_t *client, pos_t pos)
{
    if (tile == NULL) {
        debug_warning(client->server->options->debug,
            "Tile not found for bct command at coordinates: %d %d\n", pos.x,
            pos.y);
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Get the position from command arguments.
 *
 * This function extracts the position from the command arguments and checks
 * if it is within the bounds of the game map.
 *
 * @param command The command structure containing the arguments.
 * @param game The game structure containing the map dimensions.
 * @return pos_t The position extracted from the command arguments, or (-1, -1)
 * if invalid.
 */
static pos_t get_pos_from_arguments(command_t *command, game_t *game)
{
    pos_t pos = {-1, -1};

    if (command->argc != 2) {
        return pos;
    }
    pos.x = strtoull(command->argv[0], NULL, 10);
    pos.y = strtoull(command->argv[1], NULL, 10);
    if (pos.x >= (int)game->map->width || pos.y >= (int)game->map->height) {
        pos.x = -1;
        pos.y = -1;
        return pos;
    }
    return pos;
}

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
        "Client %d: bct command sent for tile at (%d, %d): "
        "%zu food, %zu linemate, %zu deraumere, %zu sibur, "
        "%zu mendiane, %zu phiras, %zu thystame\n", client->index,
        pos.x, pos.y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
}

void bct_event(tile_t *tile, pos_t pos, server_t *server)
{
    if (tile == NULL || server == NULL) {
        fprintf(stderr, "Invalid tile or server pointer in bct_event\n");
        return;
    }
    send_to_all_guis(server, "bct %d %d %zu %zu %zu %zu %zu %zu %zu\n",
        pos.x, pos.y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
}

/**
 * @brief Handle the bct command for the GUI.
 *
 * This function retrieves the tile information at the specified position and
 * sends it to the client in the format expected by the GUI.
 *
 * @param client The client requesting the tile information.
 * @param command The command structure containing the position arguments.
 */
void bct_command(client_t *client, command_t *command)
{
    tile_t *tile = NULL;
    pos_t pos = get_pos_from_arguments(command, client->server->game);

    if (check_pos_valid(pos, client) == FAILURE)
        return;
    tile = get_tile(client->server->game->map, pos);
    if (check_tile_valid(tile, client, pos) == FAILURE)
        return;
    send_tile_info(client, tile, pos);
}
