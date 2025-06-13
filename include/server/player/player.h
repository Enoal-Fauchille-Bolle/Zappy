/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/
// clang-format Language: C

#ifndef PLAYER_H_
    #define PLAYER_H_

    #include "map/tile.h"
    #include <stddef.h>
    #include <unistd.h>

typedef struct player_s {
    pos_t pos;
    orientation_t orientation;
    size_t level;      // Player's level, starting at 1
    inventory_t inventory;
    size_t tick_cooldown;      // If 0, player can act
} player_t;

player_t *create_player(const pos_t pos);
void destroy_player(player_t *player);

void turn_player_left(player_t *player);
void turn_player_right(player_t *player);
void move_player_forward(player_t *player, map_t *map);

void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_map(map_t *map, player_t *player);
void remove_player_from_map(map_t *map, player_t *player);

#endif /* !PLAYER_H_ */
