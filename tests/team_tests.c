/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team_tests
*/

#include "map/coordinates.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stddef.h>
#include <stdio.h>

Test(team, create_team)
{
    const char *team_name = "TestTeam";
    team_t *team = create_team(team_name);

    cr_assert_not_null(team, "Team should not be NULL");
    cr_assert_str_eq(team->name, team_name, "Team name should match");
    cr_assert_not_null(
        team->players, "Team players vector should not be NULL");
    cr_assert_not_null(team->eggs, "Team eggs vector should not be NULL");
    destroy_team(team);
}

Test(team, create_team_null_name)
{
    team_t *team = create_team(NULL);

    cr_assert_null(team, "Team should be NULL when name is NULL");
    destroy_team(team);
}

Test(team, destroy_team_null)
{
    team_t *team = NULL;

    destroy_team(team);      // Should not crash
    cr_assert(true, "Destroying NULL team should not cause a crash");
}

Test(team, destroy_team_valid_with_players_and_eggs)
{
    team_t *team = create_team("TestTeam");
    player_t *player = create_player((pos_t){0, 0}, 1);

    create_egg((pos_t){0, 0}, team);
    add_player_to_team(team, player);
    destroy_team(team);      // Should not crash
    cr_assert(true, "Destroying valid team with players and eggs should not "
                    "cause a crash");
}

Test(team, destroy_team_valid_with_null_players_and_eggs)
{
    team_t *team = create_team("TestTeam");

    vector_destroy(team->players);
    team->players = NULL;
    vector_destroy(team->eggs);
    team->eggs = NULL;
    destroy_team(team);
    cr_assert(true, "Destroying valid team with NULL players and eggs should "
                    "not cause a crash");
}
