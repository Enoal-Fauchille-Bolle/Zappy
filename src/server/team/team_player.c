/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team
*/

#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    player->team = team;
    vtable = vector_get_vtable(team->players);
    if (!vtable) {
        fprintf(stderr, "Invalid vtable pointer\n");
        return;
    }
    vtable->push_back(team->players, &player);
}

/**
 * @brief Remove a player from a team's players vector.
 *
 * This function removes a player from the players vector of a team. It checks
 * if the team and player pointers are valid before attempting to remove the
 * player.
 *
 * @param team Pointer to the team structure
 * @param player Pointer to the player structure to be removed
 */
void remove_player_from_team(team_t *team, player_t *player)
{
    const vector_vtable_t *vtable;
    player_t *current_player;

    if (team == NULL || player == NULL) {
        fprintf(stderr, "Invalid team or player pointer\n");
        return;
    }
    vtable = vector_get_vtable(team->players);
    for (size_t i = 0; i < vtable->size(team->players); i++) {
        current_player = *(player_t **)vtable->at(team->players, i);
        if (current_player == player) {
            current_player->team = NULL;
            vtable->erase(team->players, i);
            return;
        }
    }
    fprintf(stderr, "Player not found on team\n");
}

/**
 * @brief Get the number of players in a team.
 *
 * This function returns the number of players in a team's players vector.
 * It checks if the team pointer and its players vector are valid before
 * attempting to retrieve the size.
 *
 * @param team Pointer to the team structure
 * @return size_t The number of players in the team, or 0 if invalid
 */
size_t get_team_player_count(const team_t *team)
{
    const vector_vtable_t *vtable;

    if (team == NULL || team->players == NULL) {
        fprintf(stderr, "Invalid team pointer\n");
        return 0;
    }
    vtable = vector_get_vtable(team->players);
    return vtable->size(team->players);
}
