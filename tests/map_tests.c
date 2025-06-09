/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_tests
*/

#include "map/map.h"
#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(map, get_tile_inside_size)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile(map, (pos_t){5, 5});

    cr_assert_not_null(tile, "Tile should not be NULL");
    cr_assert_eq(tile->inventory.food, 0, "Food should be initialized to 0");
    cr_assert_eq(
        tile->inventory.linemate, 0, "Linemate should be initialized to 0");
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
    cr_assert_eq(tile->inventory.food, 0, "Food should be initialized to 0");
    cr_assert_eq(
        tile->inventory.linemate, 0, "Linemate should be initialized to 0");
    destroy_map(map);
}

Test(map, modify_tile)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile(map, (pos_t){5, 5});

    cr_assert_not_null(tile, "Tile should not be NULL");
    tile->inventory.food = 10;
    tile->inventory.linemate = 5;

    tile_t *tile_new_ptr = get_tile(map, (pos_t){5, 5});
    cr_assert_eq(tile_new_ptr->inventory.food, 10, "Food should be set to 10");
    cr_assert_eq(
        tile_new_ptr->inventory.linemate, 5, "Linemate should be set to 5");
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
        cr_assert_eq(map->tiles[i].inventory.food, 0,
            "Food should be initialized to 0");
        cr_assert_eq(map->tiles[i].inventory.linemate, 0,
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
