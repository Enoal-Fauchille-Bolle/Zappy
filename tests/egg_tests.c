/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg_tests
*/

#include "egg/egg.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/tile.h"
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

Test(egg, create_egg)
{
    pos_t pos = {5, 4};
    void *team = NULL;      // Placeholder for team pointer
    egg_t *egg = create_egg(pos, team);

    cr_assert_not_null(egg, "Egg should not be NULL");
    cr_assert_eq(egg->pos.x, 5, "Egg X position should be 5");
    cr_assert_eq(egg->pos.y, 4, "Egg Y position should be 4");
    cr_assert_eq(egg->team, team, "Egg team pointer should match");
    destroy_egg(egg);
}

Test(egg, create_egg_null_team)
{
    pos_t pos = {5, 4};
    egg_t *egg = create_egg(pos, NULL);

    cr_assert_not_null(egg, "Egg should not be NULL even with NULL team");
    cr_assert_eq(egg->pos.x, 5, "Egg X position should be 5");
    cr_assert_eq(egg->pos.y, 4, "Egg Y position should be 4");
    cr_assert_null(egg->team, "Egg team pointer should be NULL");
    destroy_egg(egg);
}

Test(egg, destroy_egg_null)
{
    egg_t *egg = NULL;

    destroy_egg(egg);      // Should not crash
    cr_assert(true, "Destroying NULL egg should not cause a crash");
}

Test(egg, destroy_egg_valid)
{
    pos_t pos = {5, 4};
    void *team = NULL;      // Placeholder for team pointer
    egg_t *egg = create_egg(pos, team);

    cr_assert_not_null(egg, "Egg should not be NULL");
    destroy_egg(egg);      // Should not crash
    cr_assert(true, "Destroying valid egg should not cause a crash");
}

Test(egg, add_egg_to_tile_null)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_tile(NULL, egg);
    add_egg_to_tile(tile, NULL);
    add_egg_to_tile(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
}

Test(egg, remove_egg_from_tile_null)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    remove_egg_from_tile(NULL, egg);
    remove_egg_from_tile(tile, NULL);
    remove_egg_from_tile(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
}

Test(egg, add_egg_to_tile_valid)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    cr_assert_not_null(tile->eggs, "Tile eggs vector should not be NULL");
    add_egg_to_tile(tile, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Tile should have one egg after adding");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_tile_valid)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_tile(tile, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Tile should have one egg before removal");
    remove_egg_from_tile(tile, egg);
    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Tile should have no eggs after removal");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_tile_not_found)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    remove_egg_from_tile(tile, egg);      // Should not crash
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, add_egg_to_map_null)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);

    add_egg_to_map(NULL, egg);
    add_egg_to_map(map, NULL);
    add_egg_to_map(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_map_null)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);

    remove_egg_from_map(NULL, egg);
    remove_egg_from_map(map, NULL);
    remove_egg_from_map(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, add_egg_to_map_valid)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Tile eggs vector should be empty before adding");
    add_egg_to_map(map, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Map tile should have one egg after adding");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_map_valid)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL);
    map_t *map = create_map(1, 1);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_map(map, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Map tile should have one egg before removal");
    remove_egg_from_map(map, egg);
    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Map tile should have no eggs after removal");
    destroy_egg(egg);
    destroy_map(map);
}
