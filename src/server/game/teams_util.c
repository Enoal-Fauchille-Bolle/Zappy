/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Teams Management
*/

#include "team/team.h"
#include <string.h>

/**
 * @brief Counts the number of team names in a null-terminated array
 *
 * @param teams Null-terminated array of team name strings
 * @return int The number of team names in the array, or 0 if teams is NULL
 */
int get_teams_name_count(char **teams)
{
    int i = 0;

    for (i = 0; teams && teams[i]; i++)
        ;
    return i;
}

/**
 * @brief Retrieves a team by its name from an array of teams
 *
 * @param teams Array of team pointers, NULL-terminated
 * @param name Name of the team to search for
 * @return team_t* Pointer to the team if found, NULL otherwise
 */
team_t *get_team_from_name(team_t **teams, const char *name)
{
    for (int i = 0; teams && teams[i]; i++) {
        if (strcmp(teams[i]->name, name) == 0) {
            return teams[i];
        }
    }
    return NULL;
}
