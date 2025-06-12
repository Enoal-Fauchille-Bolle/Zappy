/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** movement
*/

#include "map/map.h"
#include "team/player/player.h"
#include <stdio.h>

/**
 * @brief Turn the player to the left.
 *
 * This function updates the player's orientation to the left
 * (counter-clockwise) and sets a cooldown for the next action.
 *
 * @param player Pointer to the player structure whose orientation will be
 * changed
 */
void turn_player_left(player_t *player)
{
    player->orientation = turn_left(player->orientation);
    player->tick_cooldown = 7;
}

/**
 * @brief Turn the player to the right.
 *
 * This function updates the player's orientation to the right
 * (clockwise) and sets a cooldown for the next action.
 *
 * @param player Pointer to the player structure whose orientation will be
 * changed
 */
void turn_player_right(player_t *player)
{
    player->orientation = turn_right(player->orientation);
    player->tick_cooldown = 7;
}

/**
 * @brief Move the player forward in the direction they are facing.
 *
 * This function updates the player's position based on their current
 * orientation and the map's dimensions. If the player or map pointer is NULL,
 * it prints an error message and returns without making any changes.
 *
 * @param player Pointer to the player structure to be moved
 * @param map Pointer to the map structure where the player is located
 */
void move_player_forward(player_t *player, map_t *map)
{
    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return;
    }
    remove_player_from_map(map, player);
    player->pos = get_forward_position(player->pos, player->orientation, map);
    player->tick_cooldown = 7;
    add_player_to_map(map, player);
}
