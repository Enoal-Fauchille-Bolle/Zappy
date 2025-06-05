/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_tests
*/

#include "map/map.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(map, get_tile_inside_size)
{
    map_t *map = create_map(10, 10);
    tile_t *tile = get_tile(map, 5, 5);

    cr_assert_not_null(tile, "Tile should not be NULL");
    cr_assert_eq(tile->food, 0, "Food should be initialized to 0");
    cr_assert_eq(tile->linemate, 0, "Linemate should be initialized to 0");
    destroy_map(map);
}