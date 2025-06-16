/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/
// clang-format Language: C

#ifndef EGG_H_
    #define EGG_H_

    #include "map/coordinates.h"
    #include "map/tile.h"
    #include "team/player/player.h"
    #include <stddef.h>

typedef struct team_s team_t;

typedef struct egg_s {
    pos_t pos;
    team_t *team;
} egg_t;

egg_t *create_egg(const pos_t pos, team_t *team);
void destroy_egg(egg_t *egg);

void spawn_min_eggs(map_t *map, team_t *team, size_t min);
player_t *spawn_player_from_egg(
    egg_t *egg, map_t *map, const size_t player_id);

void add_egg_to_tile(tile_t *tile, egg_t *egg);
void remove_egg_from_tile(tile_t *tile, egg_t *egg);
void add_egg_to_map(map_t *map, egg_t *egg);
void remove_egg_from_map(map_t *map, egg_t *egg);

#endif /* !EGG_H_ */
