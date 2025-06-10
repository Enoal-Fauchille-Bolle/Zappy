/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resources_tests
*/

#include "map/map.h"
#include "map/resources.h"
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
