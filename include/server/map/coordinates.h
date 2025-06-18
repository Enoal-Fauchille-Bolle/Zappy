/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** coordinates
*/
// clang-format Language: C

#ifndef COORDINATES_H_
    #define COORDINATES_H_

    #include <stddef.h>

typedef struct map_s map_t;

typedef enum orientation_e {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    ORIENTATION_COUNT
} orientation_t;

typedef struct pos_s {
    long x;
    long y;
} pos_t;

pos_t wrap_coordinates(pos_t pos, size_t width, size_t height);
orientation_t turn_left(orientation_t orientation);
orientation_t turn_right(orientation_t orientation);
pos_t get_forward_position(
    const pos_t pos, orientation_t orientation, const map_t *map);
pos_t get_relative_position(const pos_t pos, orientation_t orientation,
    pos_t offset, const map_t *map);

#endif /* !COORDINATES_H_ */
