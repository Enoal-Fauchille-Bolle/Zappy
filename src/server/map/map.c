/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/

#include "map/map.h"
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
