/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/
// clang-format Language: C

#ifndef PLAYER_H_
    #define PLAYER_H_

    #include "map/coordinates.h"
    #include <stddef.h>
    #include <unistd.h>

typedef struct inventory_s {
    size_t food;
    size_t linemate;
    size_t deraumere;
    size_t sibur;
    size_t mendiane;
    size_t phiras;
    size_t thystame;
} inventory_t;

typedef struct player_s {
    size_t id;
    pos_t pos;
    orientation_t orientation;
    size_t level;
    inventory_t inventory;
    size_t team_id;
    size_t tick_cooldown;      // If 0, player can act
} player_t;

player_t *create_player(size_t id, const pos_t pos, size_t team_id);
void destroy_player(player_t *player);
void turn_player_left(player_t *player);
void turn_player_right(player_t *player);
void move_player_forward(player_t *player, map_t *map);

#endif /* !PLAYER_H_ */
