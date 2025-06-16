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
    #include "map/resources.h"
    #include "map/tile.h"
    #include <stddef.h>

typedef unsigned int level_t;
typedef unsigned int tick_t;
typedef struct team_s team_t;
typedef struct egg_s egg_t;

typedef struct player_s {
    size_t id;
    pos_t pos;
    orientation_t orientation;
    level_t level;      // Player's level, starting at 1
    inventory_t inventory;
    tick_t tick_cooldown;        // If 0, player can act
    size_t hunger_cooldown;      // If 0, player NEEDS to eat or die
    team_t *team;
} player_t;

player_t *create_player(const pos_t pos, const size_t id, team_t *team);
void destroy_player(player_t *player);

void turn_player_left(player_t *player);
void turn_player_right(player_t *player);
void move_player_forward(player_t *player, map_t *map);
egg_t *lay_egg(player_t *player, map_t *map);
char *look(player_t *player, map_t *map);

void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_map(map_t *map, player_t *player);
void remove_player_from_map(map_t *map, player_t *player);

#endif /* !PLAYER_H_ */
