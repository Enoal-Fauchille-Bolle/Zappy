/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** team_tests
*/

#include "map/coordinates.h"
#include "map/map.h"
#include "map/tile.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
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
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);

    create_egg((pos_t){0, 0}, team, false);
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

Test(team, get_egg_count_null_team)
{
    size_t count = get_egg_count(NULL);

    cr_assert_eq(count, 0, "Count should be 0 for NULL team");
}

Test(team, get_egg_count_valid)
{
    team_t *team = create_team("TestTeam");
    create_egg((pos_t){0, 0}, team, false);
    create_egg((pos_t){1, 1}, team, false);
    size_t count = get_egg_count(team);

    cr_assert_eq(count, 2, "Team should have 2 eggs");
    destroy_team(team);
}

Test(team, get_egg_count_valid_after_adding_eggs)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");
    spawn_min_eggs(map, team, 20, false);
    size_t count = get_egg_count(team);

    cr_assert_eq(count, 20, "Team should have 20 eggs");
    destroy_team(team);
}

Test(team, hatch_player_null_team)
{
    bool result = hatch_player(NULL, NULL, 1, NULL);

    cr_assert_eq(result, false, "Hatching player from NULL team should fail");
}

Test(team, hatch_player_valid)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");
    spawn_min_eggs(map, team, 1, false);
    size_t initial_player_count = 0;

    bool result = hatch_player(team, map, 1, NULL);

    cr_assert_eq(result, true, "Hatching player should succeed");
    cr_assert_eq(vector_get_vtable(team->players)->size(team->players),
        initial_player_count + 1, "Player count should increase by 1");

    player_t *player = *(player_t **)vector_get_vtable(team->players)
                            ->at(team->players, initial_player_count);
    tile_t *tile = get_tile(map, player->pos);
    player_t *player_on_tile =
        *(player_t **)vector_get_vtable(tile->players)->at(tile->players, 0);
    cr_assert_eq(player_on_tile, player, "Player should be on the tile");
    cr_assert_not_null(tile, "Tile should not be NULL");
    destroy_team(team);
    destroy_map(map);
}

Test(team, hatch_player_no_eggs)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");

    bool result = hatch_player(team, map, 1, NULL);

    cr_assert_eq(result, false, "Hatching player with no eggs should fail");
    destroy_team(team);
    destroy_map(map);
}

Test(team, add_player_to_team_null)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    team_t *team = create_team("TestTeam");

    add_player_to_team(NULL, player);      // Should not crash
    add_player_to_team(NULL, NULL);        // Should not crash
    add_player_to_team(team, NULL);        // Should not crash
    vector_destroy(team->players);
    team->players = NULL;      // Reset to avoid memory leak
    add_player_to_team(team, player);
    destroy_player(player);
    destroy_team(team);      // Should not crash
}

Test(team, remove_player_from_team_null)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    team_t *team = create_team("TestTeam");

    remove_player_from_team(NULL, player);      // Should not crash
    remove_player_from_team(team, NULL);        // Should not crash
    remove_player_from_team(NULL, NULL);        // Should not crash
    add_player_to_team(team, player);
    remove_player_from_team(team, player);
    destroy_player(player);
    destroy_team(team);      // Should not crash
}

Test(team, remove_player_from_team_valid)
{
    team_t *team = create_team("TestTeam");
    player_t *player = create_player((pos_t){0, 0}, 1, team, NULL);

    cr_assert_eq(vector_get_vtable(team->players)->size(team->players), 1,
        "Team should have one player before removal");
    remove_player_from_team(team, player);
    cr_assert_eq(vector_get_vtable(team->players)->size(team->players), 0,
        "Team should have no players after removal");
    destroy_team(team);
}

Test(team, remove_player_from_team_not_found)
{
    team_t *team = create_team("TestTeam");
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);

    create_player((pos_t){1, 1}, 2, team, NULL);
    remove_player_from_team(team, player);      // Should not crash
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_team(team);
}

Test(team, remove_egg_from_team_not_found)
{
    team_t *team = create_team("TestTeam");
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);

    create_egg((pos_t){1, 1}, team, false);
    remove_egg_from_team(team, egg);      // Should not crash
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_team(team);
}

Test(team, get_egg_count_null_team_eggs)
{
    team_t *team = create_team("TestTeam");
    vector_destroy(team->eggs);
    team->eggs = NULL;
    size_t count = get_egg_count(team);

    cr_assert_eq(count, 0, "Count should be 0 for NULL team");
}

Test(team, add_egg_to_team_null)
{
    team_t *team = create_team("TestTeam");

    add_egg_to_team(team, NULL);      // Should not crash
    add_egg_to_team(NULL, NULL);      // Should not crash
    vector_destroy(team->eggs);
    team->eggs = NULL;      // Reset to avoid memory leak
    add_egg_to_team(team, NULL);
    destroy_team(team);
}

Test(team, remove_egg_from_team_null)
{
    team_t *team = create_team("TestTeam");

    remove_egg_from_team(team, NULL);      // Should not crash
    remove_egg_from_team(NULL, NULL);      // Should not crash
    vector_destroy(team->eggs);
    team->eggs = NULL;      // Reset to avoid memory leak
    remove_egg_from_team(team, NULL);
    destroy_team(team);
}

Test(team, hatch_player_null)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");

    hatch_player(NULL, map, 1, NULL);
    hatch_player(team, NULL, 1, NULL);
    vector_destroy(team->eggs);
    team->eggs = NULL;
    hatch_player(team, NULL, 1, NULL);
    vector_destroy(team->players);
    team->players = NULL;
    hatch_player(team, NULL, 1, NULL);
    hatch_player(NULL, NULL, 1, NULL);
    destroy_team(team);
    destroy_map(map);
}

Test(team, create_player_with_team)
{
    team_t *team = create_team("TestTeam");
    player_t *player = create_player((pos_t){0, 0}, 1, team, NULL);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_eq(
        player->team, team, "Player's team should match the created team");
    cr_assert_eq(vector_get_vtable(team->players)->size(team->players), 1,
        "Team should have one player after creation");
    cr_assert_eq(
        *(player_t **)vector_get_vtable(team->players)->at(team->players, 0),
        player, "Player should be the first in the team's players vector");
    destroy_team(team);
}
