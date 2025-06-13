/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/
// clang-format Language: C

#ifndef MAP_H_
    #define MAP_H_

    #include "map/coordinates.h"
    #include "map/resources.h"
    #include "vector.h"
    #include <unistd.h>

typedef struct map_s {
    size_t width;
    size_t height;
    vector_t *tiles;      // 1D array of tiles representing the map
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);

#endif /* !MAP_H_ */
