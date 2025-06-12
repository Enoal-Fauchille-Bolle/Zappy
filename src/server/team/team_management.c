/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team
*/

#include "map/coordinates.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Get the size of the eggs vector in a team.
 *
 * This function retrieves the number of eggs associated with a team.
 * It checks if the team and its eggs vector are valid before returning the
 * size.
 *
 * @param team Pointer to the team structure
 * @return The number of eggs in the team
 */
size_t get_egg_count(const team_t *team)
{
    const vector_vtable_t *vtable;

    if (!team || !team->eggs) {
        fprintf(stderr, "Invalid team pointer\n");
        return 0;
    }
    vtable = vector_get_vtable(team->eggs);
    return vtable->size(team->eggs);
}

/**
 * @brief Add a player to a team's players vector.
 *
 * This function adds a player to the players vector of a team. It checks if
 * the team and player pointers are valid before attempting to add the player.
 *
 * @param team Pointer to the team structure
 * @param player Pointer to the player structure to be added
 */
void add_player_to_team(team_t *team, player_t *player)
{
    const vector_vtable_t *vtable;

    if (!team || !team->players || !player) {
        fprintf(stderr, "Invalid team or player pointer\n");
        return;
    }
    vtable = vector_get_vtable(team->players);
    vtable->push_back(team->players, &player);
}

static egg_t *get_random_egg(team_t *team)
{
    const vector_vtable_t *vtable;
    size_t egg_count;

    if (!team || !team->eggs) {
        fprintf(stderr, "Invalid team pointer\n");
        return NULL;
    }
    vtable = vector_get_vtable(team->eggs);
    egg_count = get_egg_count(team);
    return *(egg_t **)vtable->at(team->eggs, rand() % egg_count);
}

bool hatch_player(team_t *team, map_t *map, const size_t id)
{
    if (!team || !team->players) {
        fprintf(stderr, "Invalid team pointer\n");
        return false;
    }
    if (get_egg_count(team) == 0)
        return false;
    spawn_player_from_egg(get_random_egg(team), map, id);
    return true;
}
