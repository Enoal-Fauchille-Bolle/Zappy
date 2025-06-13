/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resources_tests
*/

#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(resources, get_minimum_resource_count)
{
    map_t *map = create_map(10, 10);
    size_t count = get_minimum_resource_count(map, FOOD_DENSITY);

    cr_assert_not_null(map, "Map should not be NULL");
    cr_assert_eq(
        count, 50, "Minimum resource count should be 50 for 0.5 density");
    destroy_map(map);
}

Test(resources, get_minimum_resource_count_not_whole)
{
    map_t *map = create_map(5, 5);
    size_t count = get_minimum_resource_count(map, THYSTAME_DENSITY);

    cr_assert_not_null(map, "Map should not be NULL");
    cr_assert_eq(
        count, 1, "Minimum resource count should be 1(.25) for 0.05 density");
    destroy_map(map);
}

Test(resources, count_resource)
{
    map_t *map = create_map(10, 10);
    size_t count;

    cr_assert_not_null(map, "Map should not be NULL");
    spread_resource(map, FOOD);
    count = count_resource(map, FOOD);
    cr_assert_gt(
        count, 0, "Food count should be greater than 0 after spreading");
    cr_assert_eq(count, get_minimum_resource_count(map, FOOD_DENSITY),
        "Food count should match minimum resource count for 0.5 density");
    destroy_map(map);
}

Test(resources, spread_resource)
{
    map_t *map = create_map(10, 10);
    size_t initial_count, new_count;

    cr_assert_not_null(map, "Map should not be NULL");
    initial_count = count_resource(map, FOOD);
    spread_resource(map, FOOD);
    new_count = count_resource(map, FOOD);

    cr_assert_gt(new_count, initial_count,
        "Food count should increase after spreading resources");
    destroy_map(map);
}

Test(resources, spread_resources_count_all)
{
    map_t *map = create_map(10, 10);
    size_t food_count, linemate_count, deraumere_count, sibur_count,
        mendiane_count, phiras_count, thystame_count;
    cr_assert_not_null(map, "Map should not be NULL");
    spread_resources(map);
    food_count = count_resource(map, FOOD);
    linemate_count = count_resource(map, LINEMATE);
    deraumere_count = count_resource(map, DERAUMERE);
    sibur_count = count_resource(map, SIBUR);
    mendiane_count = count_resource(map, MENDIANE);
    phiras_count = count_resource(map, PHIRAS);
    thystame_count = count_resource(map, THYSTAME);
    cr_assert_gt(food_count, 0, "Food count should be greater than 0");
    cr_assert_gt(linemate_count, 0, "Linemate count should be greater than 0");
    cr_assert_gt(
        deraumere_count, 0, "Deraumere count should be greater than 0");
    cr_assert_gt(sibur_count, 0, "Sibur count should be greater than 0");
    cr_assert_gt(mendiane_count, 0, "Mendiane count should be greater than 0");
    cr_assert_gt(phiras_count, 0, "Phiras count should be greater than 0");
    cr_assert_gt(thystame_count, 0, "Thystame count should be greater than 0");
    cr_assert_eq(food_count, get_minimum_resource_count(map, FOOD_DENSITY),
        "Food count should match minimum resource count for 0.5 density");
    cr_assert_eq(linemate_count,
        get_minimum_resource_count(map, LINEMATE_DENSITY),
        "Linemate count should match minimum resource count for 0.3 density");
    cr_assert_eq(deraumere_count,
        get_minimum_resource_count(map, DERAUMERE_DENSITY),
        "Deraumere count should match minimum resource count for 0.15 "
        "density");
    cr_assert_eq(sibur_count, get_minimum_resource_count(map, SIBUR_DENSITY),
        "Sibur count should match minimum resource count for 0.1 density");
    cr_assert_eq(mendiane_count,
        get_minimum_resource_count(map, MENDIANE_DENSITY),
        "Mendiane count should match minimum resource count for 0.1 density");
    cr_assert_eq(phiras_count, get_minimum_resource_count(map, PHIRAS_DENSITY),
        "Phiras count should match minimum resource count for 0.08 density");
    cr_assert_eq(thystame_count,
        get_minimum_resource_count(map, THYSTAME_DENSITY),
        "Thystame count should match minimum resource count for 0.05 density");
    destroy_map(map);
}

Test(resources, get_minimum_resource_count_null)
{
    size_t count = get_minimum_resource_count(NULL, FOOD_DENSITY);

    cr_assert_eq(count, 0, "Count should be 0 for NULL map");
}

Test(resources, count_resource_null)
{
    size_t count = count_resource(NULL, FOOD);

    cr_assert_eq(count, 0, "Count should be 0 for NULL map");
}

Test(resources, spread_resource_null)
{
    spread_resource(NULL, FOOD);
}

Test(resources, spread_resource_enough)
{
    map_t *map = create_map(10, 10);

    cr_assert_not_null(map, "Map should not be NULL");
    get_tile_by_index(map, 0)->resources[FOOD] = 1000;
    spread_resource(map, FOOD);
    cr_assert_eq(count_resource(map, FOOD), 1000,
        "Food count should not increase if already above minimum");
    destroy_map(map);
}

Test(resources, spread_resources_null)
{
    spread_resources(NULL);
}

Test(resources, minimum_resource_count_one)
{
    map_t *map = create_map(1, 1);
    size_t count = get_minimum_resource_count(map, FOOD_DENSITY);

    cr_assert_not_null(map, "Map should not be NULL");
    cr_assert_eq(
        count, 1, "Minimum resource count should be 1 for 0.5 density");
    destroy_map(map);
}

Test(resources, spread_resources_count_one)
{
    map_t *map = create_map(1, 1);
    cr_assert_not_null(map, "Map should not be NULL");
    spread_resources(map);
    cr_assert_eq(count_resource(map, FOOD), 1, "Food count should be 1");
    cr_assert_eq(
        count_resource(map, LINEMATE), 1, "Linemate count should be 1");
    cr_assert_eq(
        count_resource(map, DERAUMERE), 1, "Deraumere count should be 1");
    cr_assert_eq(count_resource(map, SIBUR), 1, "Sibur count should be 1");
    cr_assert_eq(
        count_resource(map, MENDIANE), 1, "Mendiane count should be 1");
    cr_assert_eq(count_resource(map, PHIRAS), 1, "Phiras count should be 1");
    cr_assert_eq(
        count_resource(map, THYSTAME), 1, "Thystame count should be 1");
    destroy_map(map);
}
