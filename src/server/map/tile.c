/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile
*/

#include "map/tile.h"
#include "egg/egg.h"
#include "player/player.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Initialize a tile structure.
 *
 * This function initializes a tile structure by allocating memory for the
 * players vector. If the allocation fails, it prints an error message.
 *
 * @param tile Pointer to the tile_t structure to be initialized
 *
 * @note The players vector is initialized to hold pointers to player_t
 *       structures. The caller is responsible for managing the memory of
 *       the players vector and its contents.
 */
void init_tile(tile_t *tile)
{
    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return;
    }
    tile->players = vector_new(sizeof(player_t *));
    if (tile->players == NULL) {
        perror("Failed to allocate memory for players vector");
        return;
    }
    tile->eggs = vector_new(sizeof(egg_t *));
    if (tile->eggs == NULL) {
        perror("Failed to allocate memory for eggs vector");
        vector_destroy(tile->players);
        return;
    }
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
tile_t *get_tile(const map_t *map, const pos_t pos)
{
    pos_t wrapped_pos;
    size_t index;

    if (map == NULL) {
        fprintf(stderr, "Invalid map\n");
        return NULL;
    }
    wrapped_pos = wrap_coordinates(pos, map->width, map->height);
    index = wrapped_pos.y * map->width + wrapped_pos.x;
    return get_tile_by_index(map, index);
}

/**
 * @brief Get a tile from the map by its index.
 *
 * This function retrieves a tile from the map based on its index in the
 * 1D array representation of the map. If the index is out of bounds, it
 * prints an error message and returns NULL.
 *
 * @param map Pointer to the map structure from which to retrieve the tile
 * @param index Index of the tile in the 1D array representation of the map
 * @return Pointer to the tile_t structure at the specified index,
 *         NULL if the index is out of bounds or if the map is NULL.
 */
tile_t *get_tile_by_index(const map_t *map, size_t index)
{
    const vector_vtable_t *vtable;

    if (map == NULL) {
        fprintf(stderr, "Invalid map\n");
        return NULL;
    }
    vtable = vector_get_vtable(map->tiles);
    if (index >= map->width * map->height) {
        fprintf(stderr, "Index out of bounds: %zu\n", index);
        return NULL;
    }
    return vtable->at(map->tiles, index);
}
