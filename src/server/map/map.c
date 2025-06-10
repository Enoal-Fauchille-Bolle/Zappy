/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/

#include "map/map.h"
#include "map/coordinates.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Create a new map with specified dimensions.
 *
 * This function allocates memory for a map structure and initializes its
 * dimensions and tiles. Each tile is initialized to zero.
 *
 * @param width Width of the map
 * @param height Height of the map
 * @return Pointer to the initialized map_t structure on success,
 *         NULL if memory allocation fails
 *
 * @note Caller is responsible for freeing the returned structure using
 *       destroy_map() when it is no longer needed.
 */
map_t *create_map(size_t width, size_t height)
{
    map_t *map = malloc(sizeof(map_t) + sizeof(tile_t) * width * height);

    if (map == NULL) {
        perror("Failed to allocate memory for map");
        return NULL;
    }
    if (width == 0 || height == 0) {
        fprintf(
            stderr, "Invalid map dimensions: width and height must be > 0\n");
        free(map);
        return NULL;
    }
    map->width = width;
    map->height = height;
    for (size_t i = 0; i < width * height; i++)
        map->tiles[i] = (tile_t){0};
    return map;
}

/**
 * @brief Free the memory allocated for the map.
 *
 * This function deallocates the memory used by the map structure.
 * It should be called when the map is no longer needed to prevent memory
 * leaks.
 *
 * @param map Pointer to the map structure to be destroyed
 *
 * @note If the map pointer is NULL, this function does nothing.
 */
void destroy_map(map_t *map)
{
    if (map == NULL)
        return;
    free(map);
}

/**
 * @brief Get a tile from the map at specified coordinates.
 *
 * This function retrieves a tile from the map based on the provided
 * coordinates (x, y). If the coordinates are outside the map's dimensions,
 * they are wrapped around using modulo operation to ensure valid access.
 *
 * @return Pointer to the initialized tile_t structure on success,
 *         NULL if the given map is NULL.
 *
 * @note Caller is NOT responsible for freeing the returned structure
 */
tile_t *get_tile(map_t *map, const pos_t pos)
{
    pos_t wrapped_pos = wrap_coordinates(pos, map->width, map->height);
    size_t index = wrapped_pos.y * map->width + wrapped_pos.x;

    if (map == NULL) {
        fprintf(stderr, "Invalid map\n");
        return NULL;
    }
    return &map->tiles[index];
}

/**
 * @brief Move the player forward in the direction they are facing.
 *
 * This function updates the player's position based on their current
 * orientation and the map's dimensions. If the player or map pointer is NULL,
 * it prints an error message and returns without making any changes.
 *
 * @param player Pointer to the player structure to be moved
 * @param map Pointer to the map structure where the player is located
 */
// TODO: Remove player pointer from the tile at the current position
//       Add player pointer to the tile at the new position
void move_player_forward(player_t *player, map_t *map)
{
    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return;
    }
    player->pos = get_forward_position(player->pos, player->orientation, map);
    player->tick_cooldown = 7;
}
