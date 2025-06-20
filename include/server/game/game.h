/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Structure Header
*/

#ifndef GAME_H_
    #define GAME_H_

    #include "connection/client.h"
    #include "map/map.h"
    #include "game/game_state.h"
    #include "options_parser/options.h"
    #include "team/team.h"
    #include <stddef.h>
    #include <stdbool.h>

typedef unsigned int tick_t;
typedef unsigned int tick_rate_t;

typedef struct game_s {
    server_t *server;
    map_t *map;
    team_t **teams;
    size_t next_player_id;
    size_t next_egg_id;
    tick_rate_t tick_rate;
    tick_t game_tick;
    game_state_t game_state;
} game_t;

game_t *create_game(server_t *server);
void game_tick(game_t *game, server_options_t *options);
player_t *get_player_by_id(game_t *game, size_t id);
void destroy_game(game_t *game);

#endif /* !GAME_H_ */
