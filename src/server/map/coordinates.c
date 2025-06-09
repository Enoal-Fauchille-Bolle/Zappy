/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** coordinates
*/

#include "map/coordinates.h"
#include "map/map.h"

pos_t wrap_coordinates(const pos_t pos, size_t width, size_t height)
{
    if (width == 0 || height == 0)
        return (pos_t){0, 0};
    return (pos_t){pos.x % width, pos.y % height};
}

orientation_t turn_left(orientation_t orientation)
{
    orientation--;
    if (orientation == 0)
        orientation = WEST;
    return orientation;
}

orientation_t turn_right(orientation_t orientation)
{
    orientation++;
    if (orientation > WEST)
        orientation = NORTH;
    return orientation;
}

pos_t get_forward_position(
    const pos_t pos, orientation_t orientation, const map_t *map)
{
    pos_t new_pos = pos;

    if (map == NULL)
        return pos;
    switch (orientation) {
        case NORTH:
            new_pos.y++;
            break;
        case EAST:
            new_pos.x++;
            break;
        case SOUTH:
            new_pos.y = (new_pos.y == 0) ? map->height - 1 : new_pos.y - 1;
            break;
        case WEST:
            new_pos.x = (new_pos.x == 0) ? map->width - 1 : new_pos.x - 1;
    }
    return wrap_coordinates(new_pos, map->width, map->height);
}
