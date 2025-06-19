/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/

#include "map/map.h"
#include "map/tile.h"
#include "team/egg/egg.h"
#include <stdio.h>

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
