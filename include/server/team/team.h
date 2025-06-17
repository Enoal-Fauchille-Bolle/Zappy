/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team
*/
// clang-format Language: C

#ifndef TEAM_H_
    #define TEAM_H_
    #include "connection/client.h"
    #include "map/map.h"
    #include "team/egg/egg.h"
    #include "team/player/player.h"
    #include "vector.h"
    #include <stddef.h>

typedef struct team_s {
    char *name;             // Team name
    vector_t *players;      // Vector of player_t pointers
    vector_t *eggs;         // Vector of egg_t pointers
} team_t;

team_t *create_team(const char *name);
void destroy_team(team_t *team);

void add_player_to_team(team_t *team, player_t *player);
void remove_player_from_team(team_t *team, player_t *player);
void add_egg_to_team(team_t *team, egg_t *egg);
void remove_egg_from_team(team_t *team, egg_t *egg);
size_t get_egg_count(const team_t *team);

player_t *hatch_player(team_t *team, map_t *map, const size_t id,
    client_t *client);

#endif /* !TEAM_H_ */
