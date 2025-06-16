/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_tests
*/

#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "team/player/player.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>
#include <time.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(map, get_tile_inside_size)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile(map, (pos_t){5, 5});

    cr_assert_not_null(tile, "Tile should not be NULL");
    cr_assert_eq(tile->resources[FOOD], 0, "Food should be initialized to 0");
    cr_assert_eq(
        tile->resources[LINEMATE], 0, "Linemate should be initialized to 0");
    destroy_map(map);
}

Test(map, get_tile_outside_size)
{
    map_t *map = create_map(10, 10);
    // Accessing tile at 0, 0 due to wrapping
    tile_t *tile = get_tile(map, (pos_t){10, 10});
    // Same as above, accessing tile at 0, 0
    tile_t *alternate_tile = get_tile(map, (pos_t){0, 0});

    cr_assert_not_null(
        tile, "Tile should not be NULL for out of bounds access");
    cr_assert_eq(tile, alternate_tile,
        "Tile should be the same for wrapped coordinates");
    cr_assert_eq(tile->resources[FOOD], 0, "Food should be initialized to 0");
    cr_assert_eq(
        tile->resources[LINEMATE], 0, "Linemate should be initialized to 0");
    destroy_map(map);
}

Test(map, modify_tile)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile(map, (pos_t){5, 5});

    cr_assert_not_null(tile, "Tile should not be NULL");
    tile->resources[FOOD] = 10;
    tile->resources[LINEMATE] = 5;

    tile_t *tile_new_ptr = get_tile(map, (pos_t){5, 5});
    cr_assert_eq(
        tile_new_ptr->resources[FOOD], 10, "Food should be set to 10");
    cr_assert_eq(
        tile_new_ptr->resources[LINEMATE], 5, "Linemate should be set to 5");
    destroy_map(map);
}

Test(map, create_map_null)
{
    map_t *map = create_map(0, 0);

    cr_assert_null(map, "Map should be NULL for invalid dimensions");
}

Test(map, create_map_valid)
{
    map_t *map = create_map(10, 10);

    cr_assert_not_null(map, "Map should not be NULL for valid dimensions");
    cr_assert_eq(map->width, 10, "Map width should be 10");
    cr_assert_eq(map->height, 10, "Map height should be 10");
    for (size_t i = 0; i < 100; i++) {
        cr_assert_eq(get_tile_by_index(map, i)->resources[FOOD], 0,
            "Food should be initialized to 0");
        cr_assert_eq(get_tile_by_index(map, i)->resources[LINEMATE], 0,
            "Linemate should be initialized to 0");
    }
    destroy_map(map);
}

Test(map, destroy_map_null)
{
    map_t *map = NULL;

    destroy_map(map);      // Should not crash or do anything
    cr_assert(true, "Destroying NULL map should not cause any issues");
}

Test(map, create_0_size)
{
    map_t *map = create_map(0, 10);

    cr_assert_null(map, "Map should be NULL for zero width");
    map = create_map(10, 0);
    cr_assert_null(map, "Map should be NULL for zero height");
    destroy_map(map);
}

Test(wrap_coordinates, invalid_coordinates)
{
    pos_t pos = {5, 5};
    pos_t wrapped_pos = wrap_coordinates(pos, 0, 2);

    cr_assert_eq(wrapped_pos.x, 0, "Wrapped X should be 0 for zero width");
    cr_assert_eq(wrapped_pos.y, 0, "Wrapped Y should be 0 for zero height");
    wrapped_pos = wrap_coordinates(pos, 2, 0);
    cr_assert_eq(wrapped_pos.x, 0, "Wrapped X should be 0 for zero height");
    cr_assert_eq(wrapped_pos.y, 0, "Wrapped Y should be 0 for zero width");
}

Test(get_forward_position, invalid_coordinates)
{
    pos_t pos = {5, 6};
    pos_t new_pos = get_forward_position(pos, NORTH, NULL);

    cr_assert_eq(new_pos.x, 5, "X should remain unchanged for zero size map");
    cr_assert_eq(new_pos.y, 6, "Y should remain unchanged for zero size map");
}

Test(get_tile, null)
{
    tile_t *tile = get_tile(NULL, (pos_t){0, 0});

    cr_assert_null(tile, "Tile should be NULL for NULL map");
}

Test(add_player_to_map, null)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    map_t *map = create_map(1, 1);

    add_player_to_map(map, NULL);
    add_player_to_map(NULL, player);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(remove_player_from_map, null)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    map_t *map = create_map(1, 1);

    remove_player_from_map(map, NULL);
    remove_player_from_map(NULL, player);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(access_tile_by_index, null_map)
{
    tile_t *tile = get_tile_by_index(NULL, 0);

    cr_assert_null(tile, "Tile should be NULL for NULL map");
}

Test(access_tile_by_index, out_of_bounds)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile_by_index(map, 100);

    cr_assert_null(tile, "Tile should be NULL for out of bounds index");
    destroy_map(map);
}

Test(add_player_to_tile, null)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_player_to_tile(NULL, player);
    add_player_to_tile(NULL, NULL);
    add_player_to_tile(tile, NULL);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(remove_player_from_tile, null)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    remove_player_from_tile(NULL, player);
    remove_player_from_tile(NULL, NULL);
    remove_player_from_tile(tile, NULL);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(remove_player_from_tile, player_not_in_tile)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    player_t *another_player = create_player((pos_t){0, 0}, 1);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_player_to_tile(tile, another_player);
    remove_player_from_tile(tile, player);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_player(another_player);
    destroy_map(map);
}

Test(remove_player_from_tile, player_in_tile)
{
    player_t *player = create_player((pos_t){0, 0}, 0);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_player_to_tile(tile, player);
    remove_player_from_tile(tile, player);
    // No assertion here, just checking for crashes
    destroy_player(player);
    destroy_map(map);
}

Test(init_tile, null)
{
    init_tile(NULL);
    // No assertion here, just checking for crashes
}
