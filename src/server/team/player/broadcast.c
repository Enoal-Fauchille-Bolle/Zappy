/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** listen_broadcast
*/

#include "connection/server.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "team/player/player.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// [orientation][base_direction]
static const direction_t direction_map[4][9] = {
    {MIDDLE, FRONT, FRONT_RIGHT, RIGHT, BACK_RIGHT, BACK, BACK_LEFT, LEFT,
        FRONT_LEFT},
    {MIDDLE, RIGHT, BACK_RIGHT, BACK, BACK_LEFT, LEFT, FRONT_LEFT, FRONT,
        FRONT_RIGHT},
    {MIDDLE, BACK, BACK_LEFT, LEFT, FRONT_LEFT, FRONT, FRONT_RIGHT, RIGHT,
        BACK_RIGHT},
    {MIDDLE, LEFT, FRONT_LEFT, FRONT, FRONT_RIGHT, RIGHT, BACK_RIGHT, BACK,
        BACK_LEFT}};

/**
 * @brief Calculate the shortest delta between two positions on one axis.
 *
 * This function computes the shortest delta between two positions on an axis,
 * considering the map's size. It returns the delta that is closest to zero,
 * taking into account the wrap-around nature of the map.
 *
 * @param from The starting position.
 * @param to The target position.
 * @param size The size of the map (width or height).
 * @return The shortest delta between the two positions.
 */
static int shortest_delta(const int from, const int to, const int size)
{
    int delta = to - from;

    if (abs(delta) > size / 2) {
        if (delta > 0)
            return delta - size;
        else
            return delta + size;
    }
    return delta;
}

/**
 * @brief Get the shortest path vector from start to end positions.
 *
 * This function calculates the shortest path vector between two positions on a
 * map, taking into account the map's width and height. It returns a vector
 * representing the shortest delta in both x and y directions.
 *
 * @param start The starting position.
 * @param end The ending position.
 * @param map The map containing dimensions.
 * @return A vector representing the shortest path from start to end.
 */
static vec_t get_shortest_path(
    const pos_t start, const pos_t end, const map_t *map)
{
    return (vec_t){shortest_delta(start.x, end.x, map->width),
        shortest_delta(start.y, end.y, map->height)};
}

/**
 * @brief Get the base direction from a vector.
 *
 * This function determines the base direction based on the vector's x and y
 * components. It returns one of the predefined directions (FRONT, FRONT_RIGHT,
 * RIGHT, BACK_RIGHT, BACK, BACK_LEFT, LEFT, FRONT_LEFT) or MIDDLE if the
 * vector does not match any specific direction.
 *
 * @param v The vector to analyze.
 * @return The base direction corresponding to the vector.
 */
static direction_t get_base_direction(const vec_t v)
{
    if (v.x == 0 && v.y < 0)
        return FRONT;
    if (v.x > 0 && v.y < 0)
        return FRONT_RIGHT;
    if (v.x > 0 && v.y == 0)
        return RIGHT;
    if (v.x > 0 && v.y > 0)
        return BACK_RIGHT;
    if (v.x == 0 && v.y > 0)
        return BACK;
    if (v.x < 0 && v.y > 0)
        return BACK_LEFT;
    if (v.x < 0 && v.y == 0)
        return LEFT;
    if (v.x < 0 && v.y < 0)
        return FRONT_LEFT;
    return MIDDLE;
}

/**
 * @brief Get the direction from a listening position to a target position.
 *
 * This function calculates the direction from the listening position to the
 * target position based on the shortest path and the current orientation.
 *
 * @param listen_pos The position of the listener.
 * @param listen_orientation The orientation of the listener.
 * @param target_pos The target position to which the direction is calculated.
 * @param map The map containing the dimensions and other properties.
 * @return The direction from the listener to the target position.
 */
direction_t get_direction(const pos_t listen_pos,
    const orientation_t listen_orientation, const pos_t target_pos,
    const map_t *map)
{
    direction_t result = MIDDLE;
    vec_t shortest_path = {0, 0};

    if (!map) {
        fprintf(stderr, "Error: Map is NULL in get_direction\n");
        return DIRECTION_COUNT;
    }
    if (listen_pos.x == target_pos.x && listen_pos.y == target_pos.y)
        return MIDDLE;
    shortest_path = get_shortest_path(listen_pos, target_pos, map);
    result = get_base_direction(shortest_path);
    return direction_map[listen_orientation][result];
}

/**
 * @brief Get the broadcast direction from a listening player to a broadcasting
 * player.
 *
 * This function determines the direction from a listening player to a
 * broadcasting player based on their positions and orientations.
 *
 * @param listening_player The player who is listening for broadcasts.
 * @param broadcasting_player The player who is broadcasting.
 * @param map The map containing the dimensions and properties.
 * @return The direction from the listening player to the broadcasting player,
 * or DIRECTION_COUNT if an error occurs.
 */
direction_t get_broadcast_direction(player_t *listening_player,
    player_t *broadcasting_player, const map_t *map)
{
    if (!listening_player || !broadcasting_player || !map) {
        fprintf(
            stderr, "Error: NULL player or map in get_broadcast_direction\n");
        return DIRECTION_COUNT;
    }
    return get_direction(listening_player->pos, listening_player->orientation,
        broadcasting_player->pos, map);
}

/**
 * @brief Broadcast a message to a specific player.
 *
 * This function sends a broadcast message to a player, including the direction
 * of the broadcast. It checks for NULL pointers to avoid dereferencing errors.
 *
 * @param player Pointer to the player structure to whom the message is sent.
 * @param message The message to broadcast to the player.
 */
static void broadcast_to_player(
    player_t *sender, player_t *receiver, const char *message)
{
    if (!receiver || !message) {
        fprintf(stderr,
            "Error: NULL player or message in send_broadcast_message\n");
        return;
    }
    dprintf(receiver->client->sockfd, "message %d, %s\n",
        get_broadcast_direction(
            receiver, sender, receiver->client->server->game->map),
        message);
}

/**
 * @brief Broadcast a message to all players in the game.
 *
 * This function iterates through all teams and their players, sending the
 * specified message to each player. It checks for NULL pointers to avoid
 * dereferencing errors.
 *
 * @param game Pointer to the game structure containing teams and players.
 * @param message The message to broadcast to all players.
 */
void broadcast_to_all_players(
    game_t *game, player_t *sender, const char *message)
{
    const vector_vtable_t *vtable;
    player_t *receiver = NULL;

    if (!game || !message) {
        fprintf(stderr,
            "Error: NULL game or message in broadcast_to_all_players\n");
        return;
    }
    for (size_t i = 0; game->teams[i] != NULL; i++) {
        team_t *team = game->teams[i];
        vtable = vector_get_vtable(team->players);
        for (size_t j = 0; j < vtable->size(team->players); j++) {
            receiver = *(player_t **)vtable->at(team->players, j);
            broadcast_to_player(sender, receiver, message);
        }
    }
}
