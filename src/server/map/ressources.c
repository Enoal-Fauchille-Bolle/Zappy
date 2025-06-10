/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ressources
*/

#include "map/ressources.h"
#include "map/map.h"
#include <stdio.h>

size_t get_ressource_count(const map_t *map, const float density)
{
    if (map == NULL) {
        fprintf(stderr, "Invalid map pointer\n");
        return 0;
    }
    return map->width * map->height * density;
}
