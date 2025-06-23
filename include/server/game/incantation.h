/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Management Header
*/

#ifndef INCANTATION_H_
    #define INCANTATION_H_

    #include "connection/client.h"
    #include "game/game.h"
    #include "map/coordinates.h"
    #include "team/player/player.h"
    #include "vector.h"
    #include <stdbool.h>
    #include <stddef.h>

typedef struct incantation_s {
    player_t **players;     // Players participating in the incantation
    pos_t pos;              // Position of the incantation
    level_t level;          // Level of the incantation
    tick_t ticks_left;      // Ticks left for the incantation to complete
} incantation_t;

bool check_incantation_requirements(map_t *map, pos_t pos, level_t level);
incantation_t *create_incantation(game_t *game, pos_t pos, level_t level);
void destroy_incantation(incantation_t *incantation);
void destroy_incantation_vector(vector_t *incantations);

#endif /* !INCANTATION_H_ */
