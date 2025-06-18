/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** coordinates
*/

#include "map/coordinates.h"
#include "map/map.h"
#include <stddef.h>

/**
 * @brief Wraps the coordinates to ensure they stay within the map boundaries.
 *
 * This function takes a position and wraps its coordinates based on the map's
 * width and height. If the width or height is zero, it returns (0, 0).
 *
 * @param pos The position to wrap.
 * @param width The width of the map.
 * @param height The height of the map.
 * @return Wrapped position.
 */
pos_t wrap_coordinates(pos_t pos, size_t width, size_t height)
{
    if (width == 0 || height == 0)
        return (pos_t){0, 0};
    while (pos.x < 0)
        pos.x += width;
    while (pos.y < 0)
        pos.y += height;
    return (pos_t){pos.x % width, pos.y % height};
}

/**
 * @brief Turns the orientation to the left (counter-clockwise).
 *
 * This function decrements the orientation value and wraps it around if it
 * goes below NORTH.
 *
 * @param orientation The current orientation.
 * @return The new orientation after turning left.
 */
orientation_t turn_left(orientation_t orientation)
{
    return (orientation + ORIENTATION_COUNT - 1) % ORIENTATION_COUNT;
}

/**
 * @brief Turns the orientation to the right (clockwise).
 *
 * This function increments the orientation value and wraps it around if it
 * goes above WEST.
 *
 * @param orientation The current orientation.
 * @return The new orientation after turning right.
 */
orientation_t turn_right(orientation_t orientation)
{
    return (orientation + 1) % ORIENTATION_COUNT;
}

/**
 * @brief Gets the new position after moving forward in the given orientation.
 *
 * This function calculates the new position based on the current position and
 * orientation. It wraps around the map boundaries if necessary.
 *
 * @param pos The current position.
 * @param orientation The direction to move in.
 * @param map Pointer to the map structure to check boundaries.
 * @return The new position after moving forward.
 */
pos_t get_forward_position(
    const pos_t pos, orientation_t orientation, const map_t *map)
{
    pos_t new_pos = pos;

    if (map == NULL)
        return pos;
    if (orientation == NORTH)
        new_pos.y = new_pos.y + map->height - 1;
    if (orientation == EAST)
        new_pos.x++;
    if (orientation == SOUTH)
        new_pos.y++;
    if (orientation == WEST)
        new_pos.x = new_pos.x + map->width - 1;
    return wrap_coordinates(new_pos, map->width, map->height);
}

/**
 * @brief Gets the position relative to the current position based on
 * orientation.
 *
 * This function calculates a new position based on the current position,
 * orientation, and an offset. The offset is applied in the direction of the
 * orientation. With a positive offset.y you go forward relative to the
 * direction and position, positive offset.x = right, negative offset.y =
 * backward, negative offset.x = left
 *
 * @param pos The current position.
 * @param orientation The current orientation.
 * @param offset The offset to apply in the direction of the orientation.
 * @param map Pointer to the map structure to check boundaries.
 * @return The new position after applying the offset in the given orientation.
 */
pos_t get_relative_position(
    const pos_t pos, orientation_t orientation, pos_t offset, const map_t *map)
{
    pos_t final_pos;

    if (orientation == NORTH)
        final_pos = (pos_t){pos.x + offset.x, pos.y - offset.y};
    if (orientation == EAST)
        final_pos = (pos_t){pos.x + offset.y, pos.y + offset.x};
    if (orientation == SOUTH)
        final_pos = (pos_t){pos.x - offset.x, pos.y + offset.y};
    if (orientation == WEST)
        final_pos = (pos_t){pos.x - offset.y, pos.y - offset.x};
    return wrap_coordinates(final_pos, map->width, map->height);
}
