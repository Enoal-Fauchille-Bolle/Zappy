/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** add_and_remove
*/

#include "egg/egg.h"
#include "player/player.h"
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

/**
 * @brief Add an egg to the map at its current position.
 *
 * This function places an egg on the tile corresponding to its current
 * position in the map. If the map or egg pointer is NULL, it prints an
 * error message and returns without making any changes.
 *
 * @param map Pointer to the map structure where the egg will be added
 * @param egg Pointer to the egg structure to be added to the map
 */
void add_egg_to_map(map_t *map, egg_t *egg)
{
    if (map == NULL || egg == NULL) {
        fprintf(stderr, "Invalid map or egg pointer\n");
        return;
    }
    add_egg_to_tile(get_tile(map, egg->pos), egg);
}

/**
 * @brief Remove an egg from the map at its current position.
 *
 * This function removes an egg from the tile corresponding to its current
 * position in the map. If the map or egg pointer is NULL, it prints an
 * error message and returns without making any changes.
 *
 * @param map Pointer to the map structure where the egg will be removed
 * @param egg Pointer to the egg structure to be removed from the map
 */
void remove_egg_from_map(map_t *map, egg_t *egg)
{
    if (map == NULL || egg == NULL) {
        fprintf(stderr, "Invalid map or egg pointer\n");
        return;
    }
    remove_egg_from_tile(get_tile(map, egg->pos), egg);
}
