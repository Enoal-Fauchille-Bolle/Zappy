/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/

#include "debug_categories.h"
#include "map/tile.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initialize a vector of tiles for the map.
 *
 * This function creates a new vector to hold tiles and initializes each tile
 * to zero. It allocates memory for the vector and each tile, ensuring that
 * the vector can hold the specified number of tiles based on the map's width
 * and height.
 *
 * @param width Width of the map
 * @param height Height of the map
 * @return Pointer to the initialized vector_t structure on success,
 *         NULL if memory allocation fails
 */
static vector_t *init_tiles_vector(size_t width, size_t height)
{
    vector_t *tiles = vector_new(sizeof(tile_t));
    const vector_vtable_t *vtable = vector_get_vtable(tiles);
    tile_t empty_tile = {0};

    if (tiles == NULL) {
        perror("Failed to allocate memory for tiles vector");
        return NULL;
    }
    for (size_t i = 0; i < width * height; i++)
        vtable->push_back(tiles, &empty_tile);
    return tiles;
}

/**
 * @brief Create a new map with specified dimensions.
 *
 * This function allocates memory for a map structure and initializes its
 * dimensions and tiles. Each tile is initialized to zero.
 *
 * @param width Width of the map
 * @param height Height of the map
 * @param debug Boolean flag to enable debug messages
 * @return Pointer to the initialized map_t structure on success,
 *         NULL if memory allocation fails
 *
 * @note Caller is responsible for freeing the returned structure using
 *       destroy_map() when it is no longer needed.
 */
map_t *create_map(size_t width, size_t height, bool debug)
{
    map_t *map = malloc(sizeof(map_t));

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
    map->tiles = init_tiles_vector(width, height);
    for (size_t i = 0; i < width * height; i++)
        init_tile(get_tile_by_index(map, i));
    debug_map(debug, "Map created with dimensions %zu x %zu\n", width, height);
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
    tile_t *tile;

    if (map == NULL)
        return;
    for (size_t i = 0; i < map->width * map->height; i++) {
        tile = get_tile_by_index(map, i);
        if (tile != NULL) {
            vector_destroy(tile->players);
            vector_destroy(tile->eggs);
        }
    }
    vector_destroy(map->tiles);
    free(map);
    map = NULL;
}
