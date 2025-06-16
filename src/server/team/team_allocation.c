/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team allocation
*/

#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *init_name(const char *name)
{
    char *team_name;

    if (name == NULL) {
        fprintf(stderr, "Team name cannot be NULL\n");
        return NULL;
    }
    team_name = strdup(name);
    if (team_name == NULL)
        perror("Failed to allocate memory for team name");
    return team_name;
}

static vector_t *init_players_vector(void)
{
    vector_t *players_vector = vector_new(sizeof(player_t *));

    if (players_vector == NULL)
        perror("Failed to allocate memory for players vector");
    return players_vector;
}

static vector_t *init_eggs_vector(void)
{
    vector_t *eggs_vector = vector_new(sizeof(egg_t *));

    if (eggs_vector == NULL)
        perror("Failed to allocate memory for eggs vector");
    return eggs_vector;
}

/**
 * @brief Create a new team with a specified name.
 *
 * This function allocates memory for a team structure, initializes its name,
 * players vector, and eggs vector. It returns a pointer to the newly created
 * team.
 *
 * @param name The name of the team
 * @return Pointer to the newly created team_t structure on success,
 *         NULL if memory allocation fails
 */
team_t *create_team(const char *name)
{
    team_t *team = malloc(sizeof(team_t));

    if (team == NULL) {
        perror("Failed to allocate memory for team");
        return NULL;
    }
    *team = (team_t){.name = NULL, .players = NULL, .eggs = NULL};
    team->name = init_name(name);
    team->players = init_players_vector();
    team->eggs = init_eggs_vector();
    if (team->eggs == NULL || team->players == NULL || team->name == NULL) {
        destroy_team(team);
        return NULL;
    }
    return team;
}

static void free_players_vector(vector_t *players)
{
    const vector_vtable_t *vtable = vector_get_vtable(players);
    size_t size = vtable->size(players);

    for (size_t i = 0; i < size; i++)
        destroy_player(*(player_t **)vtable->at(players, i));
    vector_destroy(players);
}

static void free_eggs_vector(vector_t *eggs)
{
    const vector_vtable_t *vtable = vector_get_vtable(eggs);
    size_t size = vtable->size(eggs);

    for (size_t i = 0; i < size; i++)
        destroy_egg(*(egg_t **)vtable->at(eggs, i));
    vector_destroy(eggs);
}

/**
 * @brief Free the memory allocated for a team.
 *
 * This function deallocates the memory used by the team structure,
 * including its name, players, and eggs.
 * It should be called when the team is no longer needed to prevent memory
 * leaks.
 *
 * @param team Pointer to the team structure to be destroyed
 *
 * @note If the team pointer is NULL, this function does nothing.
 */
void destroy_team(team_t *team)
{
    if (team == NULL)
        return;
    free(team->name);
    if (team->players != NULL)
        free_players_vector(team->players);
    if (team->eggs != NULL)
        free_eggs_vector(team->eggs);
    free(team);
}
