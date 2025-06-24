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
    #include "vector.h"
    #include <stddef.h>
    #include <stdbool.h>

typedef unsigned int tick_t;
typedef unsigned int tick_rate_t;

typedef struct game_s {
    server_t *server;           // Pointer to the server managing the game
    map_t *map;                 // Pointer to the game map
    team_t **teams;             // Array of teams in the game, NULL-terminated
    vector_t *incantations;     // Vector of incantations in progress
    size_t next_player_id;      // Next player ID to assign
    size_t next_egg_id;         // Next egg ID to assign
    tick_rate_t tick_rate;      // Rate at which the game ticks
    tick_t game_tick;           // Current game tick count
    game_state_t game_state;    // Current state of the game
} game_t;

game_t *create_game(server_t *server);
void game_tick(game_t *game, server_options_t *options);
player_t *get_player_by_id(game_t *game, size_t id);
void update_end(game_t *game);
void destroy_game(game_t *game);

#endif /* !GAME_H_ */
