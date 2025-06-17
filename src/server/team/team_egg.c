/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team_egg
*/

#include "connection/client.h"
#include "map/coordinates.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
 * @brief Add an egg to a team's eggs vector.
 *
 * This function adds an egg to the eggs vector of a team. It checks if the
 * team and egg pointers are valid before attempting to add the egg.
 *
 * @param team Pointer to the team structure
 * @param egg Pointer to the egg structure to be added
 */
void add_egg_to_team(team_t *team, egg_t *egg)
{
    const vector_vtable_t *vtable;

    if (!team || !team->eggs || !egg) {
        fprintf(stderr, "Invalid team or egg pointer\n");
        return;
    }
    vtable = vector_get_vtable(team->eggs);
    vtable->push_back(team->eggs, &egg);
}

/**
 * @brief Remove an egg from a team's eggs vector.
 *
 * This function removes an egg from the eggs vector of a team. It checks if
 * the team and egg pointers are valid before attempting to remove the egg.
 *
 * @param team Pointer to the team structure
 * @param egg Pointer to the egg structure to be removed
 */
void remove_egg_from_team(team_t *team, egg_t *egg)
{
    const vector_vtable_t *vtable;
    egg_t *current_egg;

    if (team == NULL || egg == NULL) {
        fprintf(stderr, "Invalid team or egg pointer\n");
        return;
    }
    vtable = vector_get_vtable(team->eggs);
    for (size_t i = 0; i < vtable->size(team->eggs); i++) {
        current_egg = *(egg_t **)vtable->at(team->eggs, i);
        if (current_egg == egg) {
            vtable->erase(team->eggs, i);
            return;
        }
    }
    fprintf(stderr, "Egg not found in team\n");
}

static egg_t *get_random_egg(team_t *team)
{
    const vector_vtable_t *vtable;
    size_t egg_count;

    vtable = vector_get_vtable(team->eggs);
    egg_count = get_egg_count(team);
    return *(egg_t **)vtable->at(team->eggs, rand() % egg_count);
}

/**
 * @brief Spawn a player from a random egg in the team.
 *
 * This function spawns a player at the location of a randomly selected egg
 * from the team's eggs vector. It checks if the team and map pointers are
 * valid before proceeding with the spawn.
 *
 * @param team Pointer to the team structure
 * @param map Pointer to the map structure where the player will be spawned
 * @param id The ID of the player to be spawned
 * @return player_t* Pointer to the newly spawned player, or NULL if spawning
 * failed
 */
player_t *hatch_player(
    team_t *team, map_t *map, const size_t id, client_t *client)
{
    if (!team || !team->players || !team->eggs || !map) {
        fprintf(stderr, "Invalid team or map pointer\n");
        return NULL;
    }
    if (get_egg_count(team) == 0)
        return NULL;
    return spawn_player_from_egg(get_random_egg(team), map, id, client);
}
