/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player management
*/

#include "player/player.h"
#include "map/map.h"
#include "map/tile.h"
#include <stdio.h>

/**
 * @brief Add a player to the map at their current position.
 *
 * This function places a player on the tile corresponding to their current
 * position in the map. If the map or player pointer is NULL, it prints an
 * error message and returns without making any changes.
 *
 * @param map Pointer to the map structure where the player will be added
 * @param player Pointer to the player structure to be added to the map
 */
void add_player_to_map(map_t *map, player_t *player)
{
    if (map == NULL || player == NULL) {
        fprintf(stderr, "Invalid map or player pointer\n");
        return;
    }
    add_player_to_tile(get_tile(map, player->pos), player);
}

/**
 * @brief Remove a player from the map at their current position.
 *
 * This function removes a player from the tile corresponding to their current
 * position in the map. If the map or player pointer is NULL, it prints an
 * error message and returns without making any changes. If the tile is not
 * found, it also prints an error message.
 *
 * @param map Pointer to the map structure where the player will be removed
 * @param player Pointer to the player structure to be removed from the map
 */
void remove_player_from_map(map_t *map, player_t *player)
{
    if (map == NULL || player == NULL) {
        fprintf(stderr, "Invalid map or player pointer\n");
        return;
    }
    remove_player_from_tile(get_tile(map, player->pos), player);
}
