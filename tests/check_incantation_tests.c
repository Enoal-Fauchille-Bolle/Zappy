/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** check_incantation_tests
*/

#include "connection/client.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "team/player/player.h"
#include "game/incantation.h"
#include "game/game_constants.h"
#include <criterion/criterion.h>
#include <time.h>

Test(check_incantation, check_requirements_null_map, .timeout = 2)
{
    pos_t pos = {0, 0};
    level_t level = 1;

    cr_assert_eq(check_incantation_requirements(NULL, pos, level), false,
        "Should return false for NULL map");
}

Test(check_incantation, player_already_at_max_level, .timeout = 2)
{
    pos_t pos = {0, 0};
    level_t level = MAX_PLAYER_LEVEL;
    map_t *map = create_map(10, 10, false);

    cr_assert_eq(check_incantation_requirements(map, pos, level), false,
        "Should return false for player already at max level");
    destroy_map(map);
}

Test(check_incantation, player_at_level_2_not_enough_players, .timeout = 2)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    map_t *map = create_map(10, 10, false);
    pos_t pos = {0, 0};
    level_t level = 2;

    add_player_to_map(map, player);
    player->level = 2;
    cr_assert_eq(check_incantation_requirements(map, pos, level), false,
        "Should return false for player at level 2 with not enough players");
    destroy_player(player);
    destroy_map(map);
}

Test(check_incantation, player_at_level_1_valid, .timeout = 2)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    map_t *map = create_map(10, 10, false);
    pos_t pos = {0, 0};
    level_t level = 1;

    add_player_to_map(map, player);
    get_tile(map, player->pos)->resources[LINEMATE] = 1;

    cr_assert_eq(check_incantation_requirements(map, pos, level), true,
        "Should return true for player at level 1 with enough resources");
    destroy_player(player);
    destroy_map(map);
}

Test(check_incantation, player_at_level_2_valid, .timeout = 2)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    player_t *another_player = create_player((pos_t){0, 0}, 2, NULL, NULL);
    map_t *map = create_map(10, 10, false);
    tile_t *tile = get_tile(map, player->pos);
    pos_t pos = {0, 0};
    level_t level = 2;

    add_player_to_map(map, player);
    add_player_to_map(map, another_player);
    player->level = 2;
    another_player->level = 2;
    tile->resources[LINEMATE] = 1;
    tile->resources[DERAUMERE] = 1;
    tile->resources[SIBUR] = 1;

    cr_assert_eq(check_incantation_requirements(map, pos, level), true,
        "Should return true for player at level 2 with enough resources");
    destroy_player(player);
    destroy_map(map);
}

Test(check_incantation, player_at_level_2_invalid, .timeout = 2)
{
    player_t *player = create_player((pos_t){0, 0}, 1, NULL, NULL);
    player_t *another_player = create_player((pos_t){0, 0}, 2, NULL, NULL);
    map_t *map = create_map(10, 10, false);
    tile_t *tile = get_tile(map, player->pos);
    pos_t pos = {0, 0};
    level_t level = 2;

    add_player_to_map(map, player);
    add_player_to_map(map, another_player);
    player->level = 2;
    another_player->level = 2;
    tile->resources[DERAUMERE] = 1;
    tile->resources[SIBUR] = 1;

    cr_assert_eq(check_incantation_requirements(map, pos, level), false,
        "Should return false for player at level 2 with enough resources");
    destroy_player(player);
    destroy_map(map);
}
