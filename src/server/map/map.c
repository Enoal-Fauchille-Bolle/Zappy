/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/

#include "map/map.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

map_t *create_map(size_t width, size_t height)
{
    map_t *map = malloc(sizeof(map_t) + sizeof(tile_t) * width * height);

    if (map == NULL) {
        perror("Failed to allocate memory for map");
        return NULL;
    }
    map->width = width;
    map->height = height;
    for (size_t i = 0; i < width * height; i++)
        map->tiles[i] = (tile_t){0};
    return map;
}

void destroy_map(map_t *map)
{
    if (map == NULL)
        return;
    free(map);
}

tile_t *get_tile(map_t *map, size_t x, size_t y)
{
    size_t true_x = x % map->width;
    size_t true_y = y % map->height;
    size_t index = true_y * map->width + true_x;

    if (map == NULL) {
        fprintf(stderr, "Invalid map\n");
        return NULL;
    }
    return &map->tiles[index];
}
