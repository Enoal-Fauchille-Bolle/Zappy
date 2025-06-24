/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** check_incantation
*/

#include "constants.h"
#include "game/game_constants.h"
#include "map/coordinates.h"
#include "map/resources.h"
#include "map/tile.h"
#include "team/player/player.h"
#include <stdio.h>

static bool compare_resources(
    const inventory_t resources, const inventory_t requirements)
{
    for (resource_t resource = FOOD; resource < RESOURCE_COUNT; resource++) {
        if (resources[resource] < requirements[resource]) {
            return FAILURE;
        }
    }
    return SUCCESS;
}

/**
 * @brief Check if the incantation requirements are met for a player at a given
 * position and level.
 *
 * This function checks if the specified player at the given position on the
 * map has enough players and resources to perform an incantation at the
 * specified level.
 *
 * @param map Pointer to the game map.
 * @param pos Position of the player on the map.
 * @param level Level of the incantation (1 to MAX_PLAYER_LEVEL).
 * @return true if all requirements are met, false otherwise.
 */
bool check_incantation_requirements(map_t *map, pos_t pos, level_t level)
{
    tile_t *tile = NULL;

    if (!map || pos.x < 0 || pos.y < 0 || pos.x >= (int)map->width ||
        pos.y >= (int)map->height || level < 1 || level > MAX_PLAYER_LEVEL) {
        fprintf(stderr, "Invalid map pointer\n");
        return FAILURE;
    }
    tile = get_tile(map, pos);
    if (!tile) {
        fprintf(stderr, "Tile not found for player at position (%d, %d)\n",
            pos.x, pos.y);
        return FAILURE;
    }
    if (level >= MAX_PLAYER_LEVEL ||
        get_nb_players_on_tile_with_level(tile, level) <
            NB_PLAYERS_REQUIRED[level - 1])
        return FAILURE;
    if (compare_resources(tile->resources, INCANT_REQS[level - 1]) == FAILURE)
        return FAILURE;
    return SUCCESS;
}
