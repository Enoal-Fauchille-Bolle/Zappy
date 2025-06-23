/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** check_incantation
*/

#include "map/resources.h"
#include "map/tile.h"
#include "team/player/player.h"
#include <stdio.h>

static const inventory_t INCANT_REQS[MAX_PLAYER_LEVEL - 1] = {
    [0] = {0, 1, 0, 0, 0, 0, 0},      // Level 1 to 2
    [1] = {0, 1, 1, 1, 0, 0, 0},      // Level 2 to 3
    [2] = {0, 2, 0, 1, 0, 2, 0},      // Level 3 to 4
    [3] = {0, 1, 1, 2, 0, 1, 0},      // Level 4 to 5
    [4] = {0, 1, 2, 1, 3, 0, 0},      // Level 5 to 6
    [5] = {0, 1, 2, 3, 0, 1, 0},      // Level 6 to 7
    [6] = {0, 2, 2, 2, 2, 2, 1}       // Level 7 to 8
};

static const size_t NB_PLAYERS_REQUIRED[MAX_PLAYER_LEVEL - 1] = {
    [0] = 1,      // Level 1 to 2
    [1] = 2,      // Level 2 to 3
    [2] = 2,      // Level 3 to 4
    [3] = 4,      // Level 4 to 5
    [4] = 4,      // Level 5 to 6
    [5] = 6,      // Level 6 to 7
    [6] = 6       // Level 7 to 8
};

static bool compare_resources(
    const inventory_t resources, const inventory_t requirements)
{
    for (resource_t resource = FOOD; resource < RESOURCE_COUNT; resource++) {
        if (resources[resource] < requirements[resource])
            return false;
    }
    return true;
}

/**
 * @brief Check if the tile the player is on, meets the requirements for
 * incantation.
 *
 * This function checks if the player has the required level, the required
 * number of players on the tile with the same level, and the required
 * resources for incantation.
 *
 * @param player Pointer to the player_t structure representing the player
 * @param map Pointer to the map_t structure representing the game map
 * @return true if all requirements are met, false otherwise
 */
bool check_incantation_requirements(player_t *player, map_t *map)
{
    tile_t *tile;

    if (!map || !player) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return false;
    }
    tile = get_tile(map, player->pos);
    if (!tile) {
        fprintf(stderr, "Tile not found for player at position (%d, %d)\n",
            player->pos.x, player->pos.y);
        return false;
    }
    if (player->level >= MAX_PLAYER_LEVEL ||
        get_nb_players_on_tile_with_level(tile, player->level) <
            NB_PLAYERS_REQUIRED[player->level - 1])
        return false;
    if (!compare_resources(tile->resources, INCANT_REQS[player->level - 1]))
        return false;
    return true;
}
