/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ressources
*/

#include "map/ressources.h"
#include "map/map.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Calculate the minimum number of resource needed based on the map
 * size and density.
 *
 * This function computes the minimum number of a specific resource required
 * for a given map size and resource density. It multiplies the map's width and
 * height by the specified density to determine the total number of resources
 * needed.
 *
 * @param map Pointer to the map structure containing width and height
 * @param density The density of resources to be calculated (between 0 and 1)
 * @return The minimum number of resources needed for the map
 * @note If the map pointer is NULL, it prints an error message and returns 0.
 * @note The density should be a float value between 0 and 1, representing the
 * proportion of the map that should be filled with resources. For example, a
 * density of 0.1 means that 10% of the map should be filled with the specified
 * resource.
 */
size_t get_minimum_ressource_count(const map_t *map, const float density)
{
    if (map == NULL) {
        fprintf(stderr, "Invalid map pointer\n");
        return 0;
    }
    return map->width * map->height * density;
}

/**
 * @brief Count the total number of a specific resource in the map.
 *
 * This function iterates through all tiles in the map and sums up the
 * occurrences of a specified resource type. It returns the total count of that
 * resource across the entire map.
 *
 * @param map Pointer to the map structure to be searched
 * @param ressource The type of resource to count (e.g., FOOD, LINEMATE, etc.)
 * @return The total count of the specified resource in the map
 * @note If the map pointer is NULL, it prints an error message and returns 0.
 */
size_t count_ressource(const map_t *map, const ressource_t ressource)
{
    size_t count = 0;

    if (map == NULL) {
        fprintf(stderr, "Invalid map pointer\n");
        return 0;
    }
    for (size_t i = 0; i < map->width * map->height; i++)
        count += map->tiles[i].ressources[ressource];
    return count;
}

/**
 * @brief Spread a specific resource across the map to meet minimum density.
 *
 * This function checks the current count of a specified resource in the map
 * and spreads additional resources randomly across tiles until the minimum
 * required count is reached. It does not modify the map if the current count
 * exceeds the minimum.
 *
 * @param map Pointer to the map structure where resources will be spread
 * @param ressource The type of resource to spread (e.g., FOOD, LINEMATE, etc.)
 * @note If the map pointer is NULL, it prints an error message and returns
 */
void spread_ressource(map_t *map, const ressource_t ressource)
{
    size_t min =
        get_minimum_ressource_count(map, ressource_densities[ressource]);
    size_t current_count = count_ressource(map, ressource);
    size_t target_count = min - current_count;
    tile_t *current_tile;

    if (current_count > min)
        return;
    for (size_t i = 0; i < target_count; i++) {
        current_tile = &map->tiles[rand() % (map->width * map->height)];
        current_tile->ressources[ressource]++;
    }
}

/**
 * @brief Spread all resources across the map to meet their respective minimum
 * densities.
 *
 * This function iterates through all defined resource types and calls
 * `spread_ressource` for each type to ensure that the map has the required
 * minimum number of each resource based on their defined densities.
 *
 * @param map Pointer to the map structure where resources will be spread
 * @note If the map pointer is NULL, it prints an error message and returns
 */
void spread_ressources(map_t *map)
{
    if (map == NULL) {
        fprintf(stderr, "Invalid map pointer\n");
        return;
    }
    for (ressource_t ressource = FOOD; ressource <= THYSTAME; ressource++)
        spread_ressource(map, ressource);
}
