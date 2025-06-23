/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile
*/
// clang-format Language: C

#ifndef TILE_H_
    #define TILE_H_
    #include "map/map.h"
    #include "map/coordinates.h"
    #include "map/resources.h"
    #include "vector.h"
    #include <stddef.h>

typedef struct map_s map_t;

typedef struct tile_s {
    resources_t resources;      // Items on the tile
    vector_t *players;          // Array of pointers to players on the tile
    vector_t *eggs;             // Array of pointers to eggs on the tile
} tile_t;

void init_tile(tile_t *tile);
tile_t *get_tile(const map_t *map, const pos_t pos);
tile_t *get_tile_by_index(const map_t *map, size_t index);
size_t get_nb_players_on_tile(const tile_t *tile);
size_t get_nb_eggs_on_tile(const tile_t *tile);

#endif /* !TILE_H_ */
