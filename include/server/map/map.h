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
    #include "player/player.h"
    #include "vector.h"
    #include <unistd.h>

typedef struct tile_s {
    resources_t resources;      // Items on the tile
    vector_t *players;          // Array of pointers to players on the tile
} tile_t;

typedef struct map_s {
    size_t width;
    size_t height;
    vector_t *tiles;      // 1D array of tiles representing the map
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);
void init_tile(tile_t *tile);
tile_t *get_tile(const map_t *map, const pos_t pos);
tile_t *get_tile_by_index(const map_t *map, size_t index);
void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_map(map_t *map, player_t *player);
void remove_player_from_map(map_t *map, player_t *player);

#endif /* !MAP_H_ */
