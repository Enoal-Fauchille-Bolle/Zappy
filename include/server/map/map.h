/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map
*/
// clang-format Language: C

#ifndef MAP_H_
    #define MAP_H_

    #include <unistd.h>

typedef struct tile_s {
    size_t food;
    size_t linemate;
    size_t deraumere;
    size_t sibur;
    size_t mendiane;
    size_t phiras;
    size_t thystame;
} tile_t;

typedef struct map_s {
    size_t width;
    size_t height;
    tile_t tiles[];
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);

#endif /* !MAP_H_ */
