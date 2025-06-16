/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Teams Management
*/

#ifndef TEAMS_H_
    #define TEAMS_H_

    #include "team/team.h"

int get_teams_name_count(char **teams);
team_t *get_team_from_name(team_t **teams, const char *name);

#endif /* !TEAMS_H_ */
