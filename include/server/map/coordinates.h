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

typedef enum direction_e {
    MIDDLE,
    FRONT,
    FRONT_LEFT,
    LEFT,
    BACK_LEFT,
    BACK,
    BACK_RIGHT,
    RIGHT,
    FRONT_RIGHT,
    DIRECTION_COUNT
} direction_t;

typedef struct pos_s {
    int x;
    int y;
} pos_t;

typedef pos_t vec_t;

pos_t wrap_coordinates(pos_t pos, size_t width, size_t height);
orientation_t turn_left(orientation_t orientation);
orientation_t turn_right(orientation_t orientation);
pos_t get_forward_position(
    const pos_t pos, orientation_t orientation, const map_t *map);
pos_t get_relative_position(const pos_t pos, orientation_t orientation,
    pos_t offset, const map_t *map);
direction_t get_direction(const pos_t listen_pos,
    const orientation_t listen_orientation, const pos_t target_pos,
    const map_t *map);

#endif /* !COORDINATES_H_ */
