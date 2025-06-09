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
    #include "player/player.h"
    #include <unistd.h>

typedef struct tile_s {
    inventory_t inventory;      // Items on the tile
    player_t *players[];        // Array of pointers to players on the tile
} tile_t;

typedef struct map_s {
    size_t width;
    size_t height;
    tile_t tiles[];
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);
tile_t *get_tile(map_t *map, const pos_t pos);
void move_player_forward(player_t *player, map_t *map);

#endif /* !MAP_H_ */
