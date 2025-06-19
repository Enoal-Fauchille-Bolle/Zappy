/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look_tests
*/

#include "map/coordinates.h"
#include "map/map.h"
#include "team/player/player.h"
#include "team/team.h"
#include <criterion/criterion.h>
#include <stdio.h>

Test(look, null_player, .timeout = 2)
{
    map_t *map = create_map(1, 1, false);
    player_t *player = create_player((pos_t){0, 0}, 0, NULL, NULL);
    char *result = look(NULL, map);

    cr_assert_null(result, "Look should return NULL for NULL player");
    result = look(NULL, NULL);
    cr_assert_null(result, "Look should return NULL for NULL player and map");
    result = look(player, NULL);
    cr_assert_null(result, "Look should return NULL for NULL map");
    destroy_map(map);
    destroy_player(player);
}

static const char *expected_look_results[] = {"[player]",
    "[player,,,deraumere]", "[player,,,deraumere,,food,,,]",
    "[player,,,deraumere,,food,,,,,thystame,,,,,]",
    "[player,,,deraumere,,food,,,,,thystame,,,,,,,,,,,,,,]"};

Test(look, valid, .timeout = 2)
{
    map_t *map = create_map(7, 7, false);
    team_t *team = create_team("Team1");
    player_t *player = create_player((pos_t){3, 3}, 1, team, NULL);
    char *look_result;

    player->orientation = NORTH;
    add_player_to_map(map, player);
    get_tile(map, (pos_t){1, 0})->resources[THYSTAME] = 1;
    get_tile(map, (pos_t){2, 1})->resources[FOOD] = 1;
    get_tile(map, (pos_t){4, 2})->resources[DERAUMERE] = 1;
    for (int i = 0; i < 4; i++) {
        player->level = i;
        look_result = look(player, map);
        cr_assert_str_eq(look_result, expected_look_results[i],
            "Look result mismatch at level %d: expected '%s', got '%s'", i + 1,
            expected_look_results[i], look_result);
        free(look_result);
    }
    player->level = 4;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, expected_look_results[4],
        "Final look result mismatch: expected '%s', got '%s'",
        expected_look_results[4], look_result);
    free(look_result);
    destroy_map(map);
    destroy_team(team);
}

Test(look, empty_map, .timeout = 2)
{
    map_t *map = create_map(1, 1, false);
    team_t *team = create_team("Team1");
    player_t *player = create_player((pos_t){0, 0}, 1, team, NULL);
    char *look_result;

    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[,,,]",
        "Look result should be '[,,,]', but was '%s'", look_result);
    player->level = 0;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[]",
        "Look result should be '[]', but was '%s'", look_result);
    player->level = 7;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,]",
        "Look result should be "
        "'[,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,]'");
    free(look_result);
    destroy_map(map);
    destroy_team(team);
}

Test(look, player_on_one_by_one_map, .timeout = 2)
{
    map_t *map = create_map(1, 1, false);
    team_t *team = create_team("Team1");
    player_t *player = create_player((pos_t){0, 0}, 1, team, NULL);
    char *look_result;

    player->orientation = NORTH;
    add_player_to_map(map, player);
    player->level = 0;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player]",
        "Look result should be '[player]', but was '%s'", look_result);
    player->level = 1;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player,player,player,player]",
        "Look result should be '[player,player,player,player]', but was '%s'",
        look_result);
    player->level = 2;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[player,player,player,player,player,player,player,player,player]",
        "Look result should be "
        "'[player,player,player,player,player,player,player,player,player]"
        "'");
    free(look_result);
    destroy_map(map);
    destroy_team(team);
}

// Good luck understanding this test, it checks for objects in each direction
// based on the player's level and orientation. Aswell as a look direction
// above the map's length thus testing wrapping. It is a bit complex but it
// ensures that the look function works correctly with various configurations
// of resources and player levels. I hope you have a good sense of
// visualization of the 2D map to understand this test.
Test(look, object_in_each_direction, .timeout = 2)
{
    map_t *map = create_map(3, 3, false);
    team_t *team = create_team("Team1");
    player_t *player = create_player((pos_t){1, 1}, 1, team, NULL);
    char *look_result;

    add_player_to_map(map, player);
    get_tile(map, (pos_t){0, 0})->resources[LINEMATE] =
        1;      // Top left corner
    get_tile(map, (pos_t){2, 0})->resources[DERAUMERE] =
        1;      // Top right corner
    get_tile(map, (pos_t){2, 2})->resources[SIBUR] =
        1;      // Bottom right corner
    get_tile(map, (pos_t){0, 2})->resources[PHIRAS] =
        1;      // Bottom left corner
    player->level = 1;
    player->orientation = NORTH;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player,linemate,,deraumere]",
        "Look result should be '[player,linemate,,deraumere]', but is '%s'",
        look_result);
    player->orientation = EAST;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player,deraumere,,sibur]",
        "Look result should be '[player,deraumere,,sibur]', but is '%s'",
        look_result);
    player->orientation = SOUTH;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player,sibur,,phiras]",
        "Look result should be '[player,sibur,,phiras]', but is '%s'",
        look_result);
    player->orientation = WEST;
    look_result = look(player, map);
    cr_assert_str_eq(look_result, "[player,phiras,,linemate]",
        "Look result should be '[player,phiras,,linemate]', but is '%s'",
        look_result);
    player->level = 2;
    player->orientation = NORTH;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[player,linemate,,deraumere,sibur,phiras,,sibur,phiras]",
        "Look result should be "
        "'[player,linemate,,deraumere,sibur,phiras,,sibur,phiras]', but is "
        "'%s'",
        look_result);
    player->orientation = EAST;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[player,deraumere,,sibur,phiras,linemate,,phiras,linemate]",
        "Look result should be "
        "'[player,deraumere,,sibur,phiras,linemate,,phiras,linemate]', but is "
        "'%s'",
        look_result);
    player->orientation = SOUTH;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[player,sibur,,phiras,linemate,deraumere,,linemate,deraumere]",
        "Look result should be "
        "'[player,sibur,,phiras,linemate,deraumere,,linemate,deraumere]', but "
        "is '%s'",
        look_result);
    player->orientation = WEST;
    look_result = look(player, map);
    cr_assert_str_eq(look_result,
        "[player,phiras,,linemate,deraumere,sibur,,deraumere,sibur]",
        "Look result should be "
        "'[player,phiras,,linemate,deraumere,sibur,,deraumere,sibur]', but "
        "is '%s'",
        look_result);
    free(look_result);
    destroy_map(map);
    destroy_team(team);
}
