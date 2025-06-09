/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_tests
*/

#include "map/map.h"
#include "player/player.h"
#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(player, create_player)
{
    pos_t pos = {5, 4};
    player_t *player = create_player(1, pos, 1);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_eq(player->id, 1, "Player ID should be 1");
    cr_assert_eq(player->pos.x, 5, "Player X position should be 5");
    cr_assert_eq(player->pos.y, 4, "Player Y position should be 4");
    cr_assert_eq(
        player->orientation, NORTH, "Player orientation should be NORTH");
    cr_assert_eq(player->level, 1, "Player level should be 1");
    destroy_player(player);
}

Test(player, turn_left_360)
{
    pos_t pos = {5, 4};
    player_t *player = create_player(1, pos, 1);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_eq(player->orientation, NORTH,
        "Player orientation should be NORTH before left turn");
    turn_player_left(player);
    cr_assert_eq(player->orientation, WEST,
        "Player orientation should be WEST after left turn");
    turn_player_left(player);
    cr_assert_eq(player->orientation, SOUTH,
        "Player orientation should be SOUTH after second left turn");
    turn_player_left(player);
    cr_assert_eq(player->orientation, EAST,
        "Player orientation should be EAST after third left turn");
    turn_player_left(player);
    cr_assert_eq(player->orientation, NORTH,
        "Player orientation should be NORTH after fourth left turn");
    destroy_player(player);
}

Test(player, turn_right_360)
{
    pos_t pos = {5, 4};
    player_t *player = create_player(1, pos, 1);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_eq(player->orientation, NORTH,
        "Player orientation should be NORTH before right turn");
    turn_player_right(player);
    cr_assert_eq(player->orientation, EAST,
        "Player orientation should be EAST after right turn");
    turn_player_right(player);
    cr_assert_eq(player->orientation, SOUTH,
        "Player orientation should be SOUTH after second right turn");
    turn_player_right(player);
    cr_assert_eq(player->orientation, WEST,
        "Player orientation should be WEST after third right turn");
    turn_player_right(player);
    cr_assert_eq(player->orientation, NORTH,
        "Player orientation should be NORTH after fourth right turn");
    destroy_player(player);
}

Test(player, move_player_forward_basic)
{
    pos_t pos = {0, 0};
    player_t *player = create_player(1, pos, 1);
    map_t *map = create_map(10, 10);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_not_null(map, "Map should not be NULL");
    turn_player_right(player);
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 1,
        "Player X position should be 1 after rotate and move");
    cr_assert_eq(player->pos.y, 0,
        "Player Y position should be unchanged at 0 after rotate and move");
    turn_player_right(player);
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 1,
        "Player X position should remain 1 after second move");
    cr_assert_eq(
        player->pos.y, 1, "Player Y position should be 1 after second move");
    destroy_player(player);
    destroy_map(map);
}

Test(player, move_player_forward_wrap)
{
    pos_t pos = {0, 0};
    player_t *player = create_player(1, pos, 1);
    map_t *map = create_map(10, 10);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_not_null(map, "Map should not be NULL");
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 0,
        "Player X position should remain 0 after moving forward from (0,0)");
    cr_assert_eq(player->pos.y, 9,
        "Player Y position should wrap to 9 after moving forward from (0,0)");
    turn_player_left(player);
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 9,
        "Player X position should wrap to 9 after moving left from (0,9)");
    cr_assert_eq(player->pos.y, 9,
        "Player Y position should remain 9 after moving left from (0,9)");
    destroy_player(player);
    destroy_map(map);
}
