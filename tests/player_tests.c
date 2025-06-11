/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_tests
*/

#include "map/coordinates.h"
#include "map/map.h"
#include "player/player.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stddef.h>
#include <stdio.h>

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
    player->orientation = NORTH;
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
    player->orientation = NORTH;
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
    player->orientation = NORTH;
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
    player->orientation = NORTH;
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
    player->orientation = NORTH;
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

static bool predicate(const vector_item_t *element, void *context)
{
    const player_t *player_a = (const player_t *)element;
    const player_t *player_b = (const player_t *)context;

    return player_a == player_b;
}

static bool player_in(tile_t *tile, player_t *player)
{
    const vector_vtable_t *vtable = vector_get_vtable(tile->players);

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return NULL;
    }
    return vtable->any(tile->players, predicate, player);
}

static player_t *player_at(tile_t *tile, size_t idx)
{
    const vector_vtable_t *vtable = vector_get_vtable(tile->players);

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return NULL;
    }
    if (idx >= vtable->size(tile->players)) {
        fprintf(stderr, "Index out of bounds: %zu\n", idx);
        return NULL;
    }
    return vtable->at(tile->players, idx);
}

Test(player, move_player_on_map)
{
    pos_t pos = {5, 5};
    player_t *player = create_player(1, pos, 1);
    map_t *map = create_map(10, 10);
    tile_t *prev_tile;

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_not_null(map, "Map should not be NULL");
    add_player_to_map(map, player);
    prev_tile = get_tile(map, player->pos);
    cr_assert_eq(player_at(get_tile(map, player->pos), 0), player,
        "Player should be on the tile at (5,5) before moving");
    player->orientation = EAST;
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 6,
        "Player X position should be 6 after moving east from (5,5)");
    cr_assert_eq(player->pos.y, 5,
        "Player Y position should remain 5 after moving east from (5,5)");
    cr_assert(player_in(get_tile(map, player->pos), player),
        "Player should still be on the tile at (6,5) after moving");
    cr_assert_not(player_in(prev_tile, player),
        "Player should no longer be on the previous tile at (5,5) after "
        "moving");
    destroy_player(player);
    destroy_map(map);
}

Test(player, move_player_forward_in_each_direction)
{
    pos_t pos = {5, 5};
    player_t *player = create_player(1, pos, 1);
    map_t *map = create_map(10, 10);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_not_null(map, "Map should not be NULL");
    add_player_to_map(map, player);

    // Move North
    player->orientation = NORTH;
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 5,
        "Player X position should remain 5 after moving north from (5,5)");
    cr_assert_eq(player->pos.y, 4,
        "Player Y position should be 4 after moving north from (5,5)");

    // Move East
    player->orientation = EAST;
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 6,
        "Player X position should be 6 after moving east from (5,4)");
    cr_assert_eq(player->pos.y, 4,
        "Player Y position should remain 4 after moving east from (5,4)");

    // Move South
    player->orientation = SOUTH;
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 6,
        "Player X position should remain 6 after moving south from (6,4)");
    cr_assert_eq(player->pos.y, 5,
        "Player Y position should be 5 after moving south from (6,4)");

    // Move West
    player->orientation = WEST;
    move_player_forward(player, map);
    cr_assert_eq(player->pos.x, 5,
        "Player X position should be 5 after moving west from (6,5)");
    cr_assert_eq(player->pos.y, 5,
        "Player Y position should remain 5 after moving west from (6,5)");

    destroy_player(player);
    destroy_map(map);
}

Test(player, west_wraps_correctly)
{
    map_t *map = create_map(10, 10);
    pos_t pos = {0, 5};
    pos_t new_pos = get_forward_position(pos, WEST, map);

    cr_assert_eq(new_pos.x, 9, "X should wrap to 9 when moving WEST from 0");
    cr_assert_eq(new_pos.y, 5, "Y should remain unchanged");
    destroy_map(map);
}

Test(player, move_forward_null)
{
    player_t *player = create_player(1, (pos_t){5, 5}, 1);
    map_t *map = create_map(10, 10);

    cr_assert_not_null(player, "Player should not be NULL");
    cr_assert_not_null(map, "Map should not be NULL");
    move_player_forward(NULL, map);
    move_player_forward(player, NULL);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(player, destroy_player_null)
{
    player_t *player = create_player(1, (pos_t){5, 5}, 1);

    cr_assert_not_null(player, "Player should not be NULL");
    destroy_player(NULL);      // Should not crash
    destroy_player(player);
}
